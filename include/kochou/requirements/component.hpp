#ifndef KOCHOU_REQUIREMENTS_COMPONENT_HPP
#define KOCHOU_REQUIREMENTS_COMPONENT_HPP

#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < typename T >
struct component
{
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou

template < typename T >
ktl::errc
kochou::component< T >::ensure(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < typename T >
ktl::errc
kochou::component< T >::should(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

template < typename T >
bool
kochou::component< T >::allowed(kochou::shared_context _sctx) noexcept
{
    assert(false && "unsupported now");
}

#endif
