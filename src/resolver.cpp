#include <algorithm>
#include <array>
#include <cassert>
#include <ranges>
#include <vector>

#include <kochou/base/feature_chain.hpp>
#include <kochou/context/resolver.hpp>
#include <kochou/context/view.hpp>
#include <kochou/log.hpp>

#include "details/loader.hpp"
#include "details/resolve_device.hpp"
#include "details/resolve_instance.hpp"
#include "details/resolve_version.hpp"

kochou::resolver::resolver() noexcept
    : ensure_queue_(), should_queue_(), allowed_queue_(), commands_(), ensure_(), should_(), allowed_()
{
}

kochou::resolver::~resolver() noexcept {}

ktl::errc
kochou::resolver::ensure(kochou::resolver::dependency _dependency) noexcept
{
    auto [version, dependency, commands, size] = _dependency;
    kochou::log::debug("version={}, dependency={}, cmd_size={}", version, dependency, size);

    ensure_[version].insert(dependency);
    commands_[dependency].insert_range(commands);

    return ktl::errc::success; // TODO noexcept set
}

ktl::errc
kochou::resolver::should(kochou::resolver::dependency _dependency) noexcept
{
    auto [version, dependency, commands, size] = _dependency;
    kochou::log::debug("version={}, dependency={}, cmd_size={}", version, dependency, size);

    should_[version].insert(dependency);
    commands_[dependency].insert_range(commands);

    return ktl::errc::success; // TODO noexcept set
}

bool
kochou::resolver::allowed(ktl::meta::dependency _dependency) noexcept
{
    if (_dependency.is_version())
    {
        return allowed_.first <= _dependency.version();
    }
    if (_dependency.is_extension() || _dependency.is_feature())
    {
        return allowed_.second.contains(_dependency);
    }
    return false;
}

ktl::errc
kochou::resolver::ensure(ktl::api::queue_flag_bits _bit) noexcept
{
    ensure_queue_ |= static_cast< ktl::api::queue_flags >(_bit);
    kochou::log::debug("queue={}", static_cast< ktl::api::queue_flags >(_bit));

    return ktl::errc::success;
}

ktl::errc
kochou::resolver::should(ktl::api::queue_flag_bits _bit) noexcept
{
    should_queue_ |= static_cast< ktl::api::queue_flags >(_bit);
    kochou::log::debug("queue={}", static_cast< ktl::api::queue_flags >(_bit));

    return ktl::errc::success;
}

bool
kochou::resolver::allowed(ktl::api::queue_flag_bits _bit) noexcept
{
    return (allowed_queue_ & static_cast< ktl::api::queue_flags >(_bit)) != 0;
}

kochou::resolver::resolve_result
kochou::resolver::resolve() noexcept
{
    auto version_rc = kochou::details::resolve_version();
    if (!version_rc.has_value())
    {
        return ktl::err(version_rc.error());
    }
    auto version = version_rc.take_value();
    kochou::log::debug("version={}", version);

    auto & ensure_branch = ensure_[version];
    auto & should_branch = should_[version];

    auto allowed_instance_rc = kochou::details::resolve_instance(ensure_branch, should_branch);
    if (!allowed_instance_rc.has_value())
    {
        ktl::errc rc = allowed_instance_rc.error();
        kochou::log::error("resolve_instance failed, rc={}", rc);
        return ktl::err(rc);
    }
    auto instance_extensions = allowed_instance_rc.take_value();
    auto instance_rc         = kochou::instance::make(version, instance_extensions);
    if (!instance_rc.has_value())
    {
        kochou::log::error("instance create failed");
        return ktl::err(instance_rc.error());
    }
    auto instance = instance_rc.take_value();
    kochou::log::debug("instance create successfully");

    for (auto vers : ktl::api::common_versions)
    {
        if (vers > version)
        {
            break;
        }
        for (auto command : ktl::meta::get_commands_by_version(vers))
        {
            auto name = ktl::meta::raw_command(command);
            (*ktl::api::ptable)[static_cast< ktl::u32 >(command)] =
                (void *)kochou::details::instance_load(instance->handle_, name);
        }
    }
    for (auto extension : instance_extensions)
    {
        for (auto command : commands_[extension])
        {
            auto name = ktl::meta::raw_command(command);
            (*ktl::api::ptable)[static_cast< ktl::u32 >(command)] =
                (void *)kochou::details::instance_load(instance->handle_, name);
        }
    }
    auto allowed_device_rc = kochou::details::resolve_device(version, instance->handle_, ensure_branch, should_branch,
                                                             ensure_queue_, should_queue_);
    if (!allowed_device_rc.has_value())
    {
        return ktl::err(allowed_device_rc.error());
    }
    auto [physical_device_api, device_features, device_extensions, family_index, allowed_queue] =
        allowed_device_rc.take_value();

    auto physical_device_rc = kochou::physical_device::make(physical_device_api);
    if (!physical_device_rc.has_value())
    {
        return ktl::err(physical_device_rc.error());
    }
    auto physical_device = physical_device_rc.take_value();
    kochou::log::info("physical create successfully");

    auto device_rc = kochou::device::make(version, physical_device_api, device_extensions, device_features,
                                          family_index, allowed_queue);
    if (!device_rc.has_value())
    {
        return ktl::err(device_rc.error());
    }
    auto device = device_rc.take_value();
    kochou::log::info("device create successfully");

    for (auto extension : device_extensions)
    {
        for (auto command : commands_[extension])
        {
            auto name = ktl::meta::raw_command(command);
            (*ktl::api::ptable)[static_cast< ktl::u32 >(command)] =
                (void *)kochou::details::device_load(device->handle_, name);
        }
    }

    // on exit
    allowed_.first = version;
    allowed_.second.insert_range(instance_extensions);
    allowed_.second.insert_range(device_extensions);
    allowed_.second.insert_range(device_features);
    allowed_queue_ = allowed_queue;

    return std::make_tuple(instance, physical_device, device);
}
