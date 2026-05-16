#ifndef KOCHOU_CORE_REQUIREMENTS_VERSION_HPP
#define KOCHOU_CORE_REQUIREMENTS_VERSION_HPP

#include <ktl/api/version.hpp>
#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < ktl::api::version VERSION >
struct version final
{
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou

template < ktl::api::version VERSION >
ktl::errc
kochou::version< VERSION >::ensure(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < ktl::api::version VERSION >
ktl::errc
kochou::version< VERSION >::should(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < ktl::api::version VERSION >
bool
kochou::version< VERSION >::allowed(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

#endif
