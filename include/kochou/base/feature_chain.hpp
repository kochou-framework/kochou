#ifndef KOCHOU_BASE_FEATURE_CHAIN_HPP
#define KOCHOU_BASE_FEATURE_CHAIN_HPP

#include <concepts>
#include <ranges>
#include <set>

#include <ktl/api.hpp>
#include <ktl/errc.hpp>
#include <ktl/meta.hpp>
#include <ktl/result.hpp>

#include <kochou/log.hpp>

namespace
{
inline ktl::api::version
match_stype_version(ktl::api::structure_type _stype) noexcept
{
    switch (_stype)
    {
    case ktl::api::structure_type::v_physical_device_vulkan_11features:
        return ktl::api::version_1_1;
    case ktl::api::structure_type::v_physical_device_vulkan_12features:
        return ktl::api::version_1_2;
    case ktl::api::structure_type::v_physical_device_vulkan_13features:
        return ktl::api::version_1_3;
    case ktl::api::structure_type::v_physical_device_vulkan_14features:
        return ktl::api::version_1_4;
    default:
        break;
    }

    return ktl::api::version_1_0;
}

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
inline ktl::result< std::unordered_map< ktl::api::structure_type, std::vector< ktl::meta::any_feature > >, ktl::errc >
filter_features(ktl::api::version _version, FEATURE_RANGE && _ensure, FEATURE_RANGE && _should)
{
    std::unordered_map< ktl::api::structure_type, std::vector< ktl::meta::any_feature > > features;
    for (auto feature : _ensure)
    {
        auto meta = ktl::meta::match(feature);
        if (_version < match_stype_version(meta.stype))
        {
            return ktl::err(ktl::errc::feature_not_present);
        }
        features[meta.stype].push_back(meta);
    }
    for (auto feature : _should)
    {
        auto meta = ktl::meta::match(feature);
        if (_version < match_stype_version(meta.stype))
        {
            continue;
        }
        features[meta.stype].push_back(meta);
    }
    return features;
}
} // namespace

namespace kochou
{
struct feature_chain
{
    struct description
    {
        void *                   mapped;
        ktl::api::structure_type stype;
        ktl::usize               size;
        ktl::usize               offsetof_stype;
        ktl::usize               offsetof_pnext;
    };
    void *                             head  = {};
    ktl::api::physical_device_features core  = {};
    std::vector< description >         chain = {};

    feature_chain() noexcept = default;
    ~feature_chain() noexcept;
    feature_chain(const feature_chain &) = delete;
    feature_chain(feature_chain &&)      = default;
    feature_chain &
    operator=(const feature_chain &) = delete;
    feature_chain &
    operator=(feature_chain &&) = default;
};

ktl::result< kochou::feature_chain::description, ktl::errc >
calloc_struct(ktl::meta::any_feature _meta) noexcept;

void
free_struct(kochou::feature_chain::description _description) noexcept;

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
ktl::result< kochou::feature_chain, ktl::errc >
make_feature_chain(ktl::api::version _version, FEATURE_RANGE && _ensure, FEATURE_RANGE && _should) noexcept;

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
std::set< ktl::api::feature >
match_feature_chain(ktl::api::version _version, const kochou::feature_chain & _feature_chain, FEATURE_RANGE && _ensure,
                    FEATURE_RANGE && _should) noexcept;

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
ktl::result< kochou::feature_chain, ktl::errc >
make_feature_chain_filled(ktl::api::version _version, FEATURE_RANGE && _features) noexcept;
} // namespace kochou

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
ktl::result< kochou::feature_chain, ktl::errc >
kochou::make_feature_chain(ktl::api::version _version, FEATURE_RANGE && _ensure, FEATURE_RANGE && _should) noexcept
{
    if (_ensure.empty() && _should.empty())
    {
        kochou::log::debug("no features required");
        return feature_chain{};
    }
    auto features_rc = filter_features(_version, _ensure, _should);
    if (!features_rc.has_value())
    {
        return ktl::err(features_rc.error());
    }
    auto features = features_rc.take_value();

    feature_chain chain = {};
    for (auto & [stype, vec] : features)
    {
        if (vec[0].is_core)
        {
            continue;
        }
        auto calloc_rc = kochou::calloc_struct(vec[0]);
        if (!calloc_rc.has_value())
        {
            kochou::log::fatal("failed alloc, stype={}", static_cast< ktl::u32 >(stype));
            return ktl::err(calloc_rc.error());
        }
        chain.chain.push_back(calloc_rc.take_value());
    }
    if (chain.chain.size() == 0)
    {
        return chain;
    }
    if (chain.chain.size() == 1)
    {
        chain.head = chain.chain[0].mapped;
        return chain;
    }
    for (ktl::usize i = 0; i < chain.chain.size() - 1; ++i)
    {
        auto * mapped                                                                   = chain.chain[i].mapped;
        auto   offsetof_pnext                                                           = chain.chain[i].offsetof_pnext;
        *reinterpret_cast< void ** >(static_cast< ktl::u8 * >(mapped) + offsetof_pnext) = chain.chain[i + 1].mapped;
    }
    chain.head = chain.chain[0].mapped;
    return chain;
}

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
std::set< ktl::api::feature >
kochou::match_feature_chain(ktl::api::version _version, const kochou::feature_chain & _feature_chain,
                            FEATURE_RANGE && _ensure, FEATURE_RANGE && _should) noexcept
{
    if (_ensure.empty() && _should.empty())
    {
        kochou::log::debug("no features required");
        return {};
    }
    auto features_rc = filter_features(_version, _ensure, _should);
    if (!features_rc.has_value())
    {
        return {};
    }
    auto features = features_rc.take_value();

    std::set< ktl::api::feature > result;
    for (const auto & chainlet : _feature_chain.chain)
    {
        auto * mapped = chainlet.mapped;
        for (const auto & feature : features[chainlet.stype])
        {
            auto offsetof_field = feature.offsetof_field;
            if (*reinterpret_cast< ktl::api::bool32 * >(static_cast< ktl::u8 * >(mapped) + offsetof_field) == 1)
            {
                kochou::log::debug("{} allowed", static_cast< ktl::u32 >(feature.stype));
                result.insert(feature.value);
                continue;
            }
            kochou::log::debug("{} not allowed", static_cast< ktl::u32 >(feature.stype));
        }
    }
    {
        for (const auto & feature : features[static_cast< ktl::api::structure_type >(0)])
        {
            auto offsetof_field = feature.offsetof_field;
            if (*reinterpret_cast< const ktl::api::bool32 * >(
                    reinterpret_cast< const ktl::u8 * >(&_feature_chain.core) + offsetof_field) == 1)
            {
                kochou::log::debug("{} allowed", 0);
                result.insert(feature.value);
                continue;
            }
            kochou::log::debug("{} not allowed", 0);
        }
    }

    return result;
}

template < std::ranges::input_range FEATURE_RANGE >
    requires std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
ktl::result< kochou::feature_chain, ktl::errc >
kochou::make_feature_chain_filled(ktl::api::version _version, FEATURE_RANGE && _features) noexcept
{
    auto chain_rc = make_feature_chain(_version, _features, _features);
    if (!chain_rc.has_value())
    {
        return ktl::err(chain_rc.error());
    }
    auto chain       = chain_rc.take_value();
    auto features_rc = filter_features(_version, _features, _features);
    if (!features_rc.has_value())
    {
        return ktl::err(features_rc.error());
    }
    auto features = features_rc.take_value();
    for (const auto & chainlet : chain.chain)
    {
        auto * mapped = chainlet.mapped;
        for (const auto & feature : features[chainlet.stype])
        {
            auto offsetof_field = feature.offsetof_field;
            *reinterpret_cast< ktl::api::bool32 * >(static_cast< ktl::u8 * >(mapped) + offsetof_field) = 1;
        }
    }
    {
        for (const auto & feature : features[static_cast< ktl::api::structure_type >(0)])
        {
            auto offsetof_field = feature.offsetof_field;
            *reinterpret_cast< ktl::api::bool32 * >(reinterpret_cast< ktl::u8 * >(&chain.core) + offsetof_field) = 1;
        }
    }
    return chain;
}

#endif
