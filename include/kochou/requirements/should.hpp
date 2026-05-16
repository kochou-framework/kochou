#ifndef KOCHOU_REQUIREMENTS_SHOULD_HPP
#define KOCHOU_REQUIREMENTS_SHOULD_HPP

#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < typename T >
ktl::errc
should(kochou::shared_context _sctx) noexcept;
} // namespace kochou

template < typename T >
ktl::errc
kochou::should(kochou::shared_context _sctx) noexcept
{
    return T::should(_sctx);
}

#endif
