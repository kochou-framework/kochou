#ifndef KOCHOU_REQUIREMENTS_ALLOWED_HPP
#define KOCHOU_REQUIREMENTS_ALLOWED_HPP

#include <kochou/context/context.hpp>

namespace kochou
{
template < typename T >
bool
allowed(kochou::shared_context _sctx) noexcept;
} // namespace kochou

template < typename T >
bool
kochou::allowed(kochou::shared_context _sctx) noexcept
{
    return T::allowed(_sctx);
}

#endif
