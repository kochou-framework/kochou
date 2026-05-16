#ifndef KOCHOU_REQUIREMENTS_ENSURE_HPP
#define KOCHOU_REQUIREMENTS_ENSURE_HPP

#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < typename T >
ktl::errc
ensure(kochou::shared_context _sctx) noexcept;
} // namespace kochou

template < typename T >
ktl::errc
kochou::ensure(kochou::shared_context _sctx) noexcept
{
    return T::ensure(_sctx);
}

#endif
