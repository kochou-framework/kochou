#ifndef KOCHOU_ENTITY_IMAGE_HPP
#define KOCHOU_ENTITY_IMAGE_HPP

#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class swapchain;
class image
{
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;

public:
    static ktl::result< std::vector< image >, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::swapchain & _swapchain) noexcept;

public:
    image(ktl::api::image _image) noexcept;
    image(const image &) noexcept = delete;
    image(image &&) noexcept      = default;
    image &
    operator=(const image &) noexcept = delete;
    image &
    operator=(image &&) noexcept = default;
    ~image() noexcept;

private:
    ktl::api::image image_;
};
using shared_image = ktl::memory::sptr< kochou::entity::image >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::image >);

#endif
