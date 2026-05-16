#include "resolve_instance.hpp"

#include <algorithm>

namespace
{
ktl::result< std::set< ktl::api::extension >, ktl::errc >
get_available_instance_extensions() noexcept
{
    ktl::u32         amount = 0;
    ktl::api::result rc     = ktl::api::enumerate_instance_extension_properties(nullptr, &amount, nullptr);
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("failed to ktl::api::enumerate_instance_extension_properties, rc={}",
                           static_cast< ktl::i32 >(rc));
        return ktl::err(ktl::cast_api_result(rc));
    }

    std::vector< ktl::api::extension_properties > properties(amount);
    rc = ktl::api::enumerate_instance_extension_properties(nullptr, &amount, properties.data());
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("failed to ktl::api::enumerate_instance_extension_properties, rc={}",
                           static_cast< ktl::i32 >(rc));
        return ktl::err(ktl::cast_api_result(rc));
    }

    return properties |
           std::views::transform(
               [](ktl::api::extension_properties _properties)
               {
                   return ktl::meta::extension_from_raw(_properties.extension_name);
               }) |
           std::ranges::to< std::set< ktl::api::extension > >();
}
} // namespace

ktl::result< std::set< ktl::api::extension >, ktl::errc >
kochou::details::resolve_instance(const std::set< ktl::meta::dependency > & _ensure,
                                  const std::set< ktl::meta::dependency > & _should) noexcept
{
    auto extension_from_dependency = [](const ktl::meta::dependency & _dependency)
    {
        return _dependency.extension();
    };
    auto instance_extension_filter = [](const ktl::meta::dependency & _dependency)
    {
        return _dependency.is_extension() && ktl::meta::extension_cast(_dependency.extension()).is_instance;
    };

    if (_ensure.contains({}))
    {
        return ktl::err(ktl::errc::bad_vk_api_version);
    }
    auto extensions_rc = get_available_instance_extensions();
    if (!extensions_rc.has_value())
    {
        return ktl::err(extensions_rc.error());
    }
    auto extensions          = extensions_rc.take_value();
    auto available_extension = [&extensions](ktl::api::extension _extension)
    {
        return extensions.contains(_extension);
    };

    auto ensure_filtered =
        _ensure | std::views::filter(instance_extension_filter) | std::views::transform(extension_from_dependency);
    auto should_filtered = _should | std::views::filter(instance_extension_filter) |
                           std::views::transform(extension_from_dependency) | std::views::filter(available_extension);
    if (!std::all_of(std::begin(ensure_filtered), std::end(ensure_filtered), available_extension))
    {
        kochou::log::error("ensure extension not satisfied");
        return ktl::err(ktl::errc::extension_not_present);
    }
    std::set< ktl::api::extension > allowed_extensions; // TODO C++ 26
    allowed_extensions.insert_range(ensure_filtered);
    allowed_extensions.insert_range(should_filtered);

    return allowed_extensions;
}
