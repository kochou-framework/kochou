#ifndef KOCHOU_ENTITY_IMAGE_HPP
#define KOCHOU_ENTITY_IMAGE_HPP

#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>
#include <kochou/entity/swapchain.hpp>

namespace kochou::entity
{
class image;
using shared_image = ktl::memory::sptr< kochou::entity::image >;

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
    static ktl::result< std::vector< shared_image >, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_swapchain _swapchain) noexcept;

    // common
public:
    image(kochou::shared_context _sctx, kochou::entity::shared_swapchain _swapchain, ktl::api::image _image,
          bool _is_need_destroy) noexcept;
    image(const image &) noexcept = delete;
    image(image &&) noexcept;
    image &
    operator=(const image &) noexcept = delete;
    image &
    operator=(image &&) noexcept;
    ~image() noexcept;

    // raw
public:
    ktl::api::image raw;
    bool            is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context           sctx_;
    kochou::entity::shared_swapchain swapchain_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::image >);

#endif
