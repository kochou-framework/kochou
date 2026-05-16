#include "resolve_device.hpp"

#include <algorithm>
#include <format>
#include <ranges>

#include <kochou/base/feature_chain.hpp>
#include <kochou/log.hpp>

#include "resolve_queue.hpp"

namespace
{
ktl::result< std::vector< ktl::api::physical_device >, ktl::errc >
get_physical_devices(ktl::api::instance _instance) noexcept
{
    ktl::u32         amount = 0;
    ktl::api::result rc     = ktl::api::enumerate_physical_devices(_instance, &amount, nullptr);
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("failed enumerate physical devices");
        return ktl::err(ktl::cast_api_result(rc));
    }
    std::vector< ktl::api::physical_device > physical_devices(amount);
    rc = ktl::api::enumerate_physical_devices(_instance, &amount, physical_devices.data());
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("failed enumerate physical devices");
        return ktl::err(ktl::cast_api_result(rc));
    }
    return physical_devices;
}

ktl::result< std::tuple< ktl::api::physical_device, ktl::u32, ktl::api::queue_flags >, ktl::errc >
get_physical_device_info(ktl::api::instance _instance, ktl::api::queue_flags _ensure,
                         ktl::api::queue_flags _should) noexcept
{
    // TODO
    // выбирается первый подходящий, нужен лучший
    auto physical_devices_rc = get_physical_devices(_instance);
    if (!physical_devices_rc.has_value())
    {
        return ktl::err(physical_devices_rc.error());
    }
    auto physical_devices = physical_devices_rc.take_value();
    for (auto physical_device : physical_devices)
    {
        auto index_rc = kochou::details::get_queue_family_index(physical_device, _ensure, _should);
        if (!index_rc.has_value())
        {
            return ktl::err(index_rc.error());
        }
        auto [index, allowed] = index_rc.take_value();
        return std::make_tuple(physical_device, index, allowed);
    }
    return ktl::err(ktl::errc::queue_not_provides);
}

ktl::result< std::set< ktl::api::extension >, ktl::errc >
get_available_device_extensions(ktl::api::physical_device _physical_device) noexcept
{
    ktl::u32         amount = 0;
    ktl::api::result rc = ktl::api::enumerate_device_extension_properties(_physical_device, nullptr, &amount, nullptr);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    std::vector< ktl::api::extension_properties > properties(amount);
    rc = ktl::api::enumerate_device_extension_properties(_physical_device, nullptr, &amount, properties.data());
    if (rc != ktl::api::result::v_success)
    {
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

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
ktl::result< std::set< ktl::api::feature >, ktl::errc >
get_allowed_device_features(ktl::api::physical_device _physical_device, const ktl::api::version & _version,
                            FEATURE_RANGE && _ensure, FEATURE_RANGE && _should) noexcept
{
    auto chain_rc = kochou::make_feature_chain(_version, _ensure, _should);
    if (!chain_rc.has_value())
    {
        auto err = chain_rc.error();
        kochou::log::error("failed make_feature_chain, rc={}", err);
        return ktl::err(err);
    }
    auto chain = chain_rc.take_value();

    if (_version == ktl::api::version_1_0)
    {
        if (chain.head)
        {
            // TODO
            // нужно переместить проверку features до создания instance, и включать расширение
            // VK_KHR_get_physical_device_properties2 если version == 1.0
            kochou::log::fatal("TODO");
            std::abort();
        }
        ktl::api::get_physical_device_features(_physical_device, &chain.core);
    }
    else
    {
        ktl::api::physical_device_features_2 tmp = {.pnext = chain.head, .features = {}};
        ktl::api::get_physical_device_features_2(_physical_device, &tmp);
        chain.core = tmp.features;
    }

    return kochou::match_feature_chain(_version, chain, _ensure, _should);
}
} // namespace

ktl::result< std::tuple< ktl::api::physical_device, std::set< ktl::api::feature >, std::set< ktl::api::extension >,
                         ktl::u32, ktl::api::queue_flags >,
             ktl::errc >
kochou::details::resolve_device(const ktl::api::version & _version, ktl::api::instance _instance,
                                const std::set< ktl::meta::dependency > & _ensure,
                                const std::set< ktl::meta::dependency > & _should, ktl::api::queue_flags _ensure_queue,
                                ktl::api::queue_flags _should_queue) noexcept
{
    auto extension_from_dependency = [](const ktl::meta::dependency & _dependency)
    {
        return _dependency.extension();
    };
    auto device_extension_filter = [](const ktl::meta::dependency & _dependency)
    {
        return _dependency.is_extension() && !ktl::meta::extension_cast(_dependency.extension()).is_instance;
    };
    auto feature_from_dependency = [](const ktl::meta::dependency & _dependency)
    {
        return _dependency.feature();
    };
    auto feature_filter = [](const ktl::meta::dependency & _dependency)
    {
        return _dependency.is_feature();
    };

    auto physical_device_rc = get_physical_device_info(_instance, _ensure_queue, _should_queue);
    if (!physical_device_rc.has_value())
    {
        return ktl::err(physical_device_rc.error());
    }
    auto [physical_device, family_index, allowed_queue] = physical_device_rc.take_value();

    auto ensure_features =
        _ensure | std::views::filter(feature_filter) | std::views::transform(feature_from_dependency);
    auto should_features =
        _should | std::views::filter(feature_filter) | std::views::transform(feature_from_dependency);
    auto allowed_features_rc = get_allowed_device_features(physical_device, _version, ensure_features, should_features);
    if (!allowed_features_rc.has_value())
    {
        return ktl::err(allowed_features_rc.error());
    }
    auto allowed_features = allowed_features_rc.take_value();
    auto allowed_feature  = [&allowed_features](ktl::api::feature _feature)
    {
        return allowed_features.contains(_feature);
    };
    if (!std::all_of(std::begin(ensure_features), std::end(ensure_features), allowed_feature))
    {
        kochou::log::error("ensure features not satisfied for version={}", _version);
        return ktl::err(ktl::errc::feature_not_present);
    }

    auto extensions_rc = get_available_device_extensions(physical_device);
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
        _ensure | std::views::filter(device_extension_filter) | std::views::transform(extension_from_dependency);
    auto should_filtered = _should | std::views::filter(device_extension_filter) |
                           std::views::transform(extension_from_dependency) | std::views::filter(available_extension);
    if (!std::all_of(std::begin(ensure_filtered), std::end(ensure_filtered), available_extension))
    {
        return ktl::err(ktl::errc::extension_not_present);
    }
    std::set< ktl::api::extension > allowed_extensions; // TODO C++ 26
    allowed_extensions.insert_range(ensure_filtered);
    allowed_extensions.insert_range(should_filtered);

    return std::make_tuple(physical_device, allowed_features, allowed_extensions, family_index, allowed_queue);
}
