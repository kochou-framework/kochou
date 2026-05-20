#ifndef KOCHOU_ENTITY_IMAGE_HPP
#define KOCHOU_ENTITY_IMAGE_HPP

#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>
#include <kochou/entity/swapchain.hpp>

namespace kochou::entity
{
class image
{
    // requirements
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;

    // fabrics
public:
    static ktl::result< std::vector< image >, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_swapchain _swapchain) noexcept;

public:
    image(kochou::entity::shared_swapchain _swapchain, ktl::api::image _image) noexcept;
    image(const image &) noexcept = delete;
    image(image &&) noexcept      = default;
    image &
    operator=(const image &) noexcept = delete;
    image &
    operator=(image &&) noexcept = default;
    ~image() noexcept;

private:
    kochou::entity::shared_swapchain swapchain_;
    ktl::api::image                  image_;
};
using shared_image = ktl::memory::sptr< kochou::entity::image >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::image >);

#endif
