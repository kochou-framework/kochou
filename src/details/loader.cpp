#include "loader.hpp"

#include <ktl/meta.hpp>

#include <kochou/log.hpp>

ktl::loader::proc_type
kochou::details::system_load(ktl::loader::handle_type _handle, std::string_view _symbol) noexcept
{
    auto result = ktl::loader::proc(_handle, _symbol.data());
    if (!result.has_value()) [[unlikely]]
    {
        kochou::log::warning("failed to load: handle={}, symbol={}", _handle, _symbol);
        return ktl::loader::proc_null;
    }
    kochou::log::debug("loaded: handle={}, symbol={}", _handle, _symbol);
    return result.take_value();
}

ktl::api::pfn_void_function
kochou::details::instance_load(ktl::api::instance _instance, std::string_view _symbol) noexcept
{
    ktl::api::pfn_void_function pfn = ktl::api::get_instance_proc_addr(_instance, _symbol.data());
    if (pfn == nullptr) [[unlikely]]
    {
        kochou::log::warning("failed to load: instance={}, symbol={}", _instance, _symbol);
        return nullptr;
    }
    kochou::log::debug("loaded: instance={}, symbol={}", _instance, _symbol);
    return pfn;
}

ktl::api::pfn_void_function
kochou::details::device_load(ktl::api::device _device, std::string_view _symbol) noexcept
{
    ktl::api::pfn_void_function pfn = ktl::api::get_device_proc_addr(_device, _symbol.data());
    if (pfn == nullptr) [[unlikely]]
    {
        kochou::log::warning("failed to load: device={}, symbol={}", _device, _symbol);
        return nullptr;
    }
    kochou::log::debug("loaded: device={}, symbol={}", _device, _symbol);
    return pfn;
}
