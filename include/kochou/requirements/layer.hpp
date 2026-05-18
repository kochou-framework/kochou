#ifndef KOCHOU_REQUIREMENTS_LAYER_HPP
#define KOCHOU_REQUIREMENTS_LAYER_HPP

#include <cassert>

#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < ktl::u32 LAYER >
struct layer
{
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou

template < ktl::u32 LAYER >
ktl::errc
kochou::layer< LAYER >::ensure(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < ktl::u32 LAYER >
ktl::errc
kochou::layer< LAYER >::should(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < ktl::u32 LAYER >
bool
kochou::layer< LAYER >::allowed(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

#endif
