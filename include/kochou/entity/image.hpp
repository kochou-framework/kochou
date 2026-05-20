#ifndef KOCHOU_ENTITY_IMAGE_HPP
#define KOCHOU_ENTITY_IMAGE_HPP

#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class image
{
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
using shared_image = ktl::memory::sptr< kochou::entity::image >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::image >);

#endif
