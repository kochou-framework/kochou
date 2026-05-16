#ifndef KOCHOU_REQUIREMENTS_FEATURE_HPP
#define KOCHOU_REQUIREMENTS_FEATURE_HPP

#include <ktl/api.hpp>
#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < ktl::api::feature FEATURE >
struct feature final
{
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou

template < ktl::api::feature FEATURE >
ktl::errc
kochou::feature< FEATURE >::ensure(kochou::shared_context _sctx) noexcept
{
    for (const auto & version : ktl::api::common_versions)
    {
        ktl::errc rc = _sctx->ensure({version, FEATURE, {}, {}});
        if (rc != ktl::errc::success)
        {
            return rc;
        }
    }
    return ktl::errc::success;
}

template < ktl::api::feature FEATURE >
ktl::errc
kochou::feature< FEATURE >::should(kochou::shared_context _sctx) noexcept
{
    for (const auto & version : ktl::api::common_versions)
    {
        ktl::errc rc = _sctx->should({version, FEATURE, {}, {}});
        if (rc != ktl::errc::success)
        {
            return rc;
        }
    }
    return ktl::errc::success;
}

template < ktl::api::feature FEATURE >
bool
kochou::feature< FEATURE >::allowed(kochou::shared_context _sctx) noexcept
{
    return _sctx->allowed(FEATURE);
}

#endif
