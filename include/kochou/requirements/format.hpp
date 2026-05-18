#ifndef KOCHOU_REQUIREMENTS_FORMAT_HPP
#define KOCHOU_REQUIREMENTS_FORMAT_HPP

#include <cassert>

#include <ktl/api.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < ktl::api::format FORMAT >
struct format
{
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou

template < ktl::api::format FORMAT >
ktl::errc
kochou::format< FORMAT >::ensure(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < ktl::api::format FORMAT >
ktl::errc
kochou::format< FORMAT >::should(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < ktl::api::format FORMAT >
bool
kochou::format< FORMAT >::allowed(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

#endif
