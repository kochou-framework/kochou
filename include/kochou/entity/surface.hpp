#ifndef KOCHOU_ENTITY_SURFACE_HPP
#define KOCHOU_ENTITY_SURFACE_HPP

#include <kochou/context/context.hpp>

namespace kochou::entity
{
class surface
{
public:
    surface(kochou::shared_context _sctx) noexcept;

private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

#endif
