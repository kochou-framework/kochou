#ifndef KOCHOU_ENTITY_INTERFACE_HPP
#define KOCHOU_ENTITY_INTERFACE_HPP

#include <concepts>

#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace kochou::entity
{
template < typename ENTITY >
concept is_valid_entity = requires(kochou::shared_context _sctx) {
    { ENTITY::ensure(_sctx) } noexcept -> std::same_as< ktl::errc >;
    { ENTITY::should(_sctx) } noexcept -> std::same_as< ktl::errc >;
    { ENTITY::allowed(_sctx) } noexcept -> std::same_as< bool >;
};
} // namespace kochou::entity

#endif
