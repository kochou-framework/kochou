#ifndef KOCHOU_ENTITY_IMAGE_VIEW_HPP
#define KOCHOU_ENTITY_IMAGE_VIEW_HPP

#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class image_view
{
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
using shared_image_view = ktl::memory::sptr< image_view >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::image_view >);

#endif
