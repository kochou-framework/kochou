#ifndef KOCHOU_ENTITY_IMAGE_VIEW_HPP
#define KOCHOU_ENTITY_IMAGE_VIEW_HPP

#include <span>

#include <ktl/memory.hpp>

#include <kochou/entity/image.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class image_view
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
    static ktl::result< std::vector< kochou::entity::image_view >, ktl::errc >
    make(kochou::shared_context _sctx, std::span< kochou::entity::image > _images, ktl::api::format _format,
         ktl::api::image_aspect_flag_bits _aspect = ktl::api::image_aspect_flag_bits::v_color_bit) noexcept;

    // common
public:
    image_view(kochou::shared_context _sctx, ktl::api::image_view _image_view, bool _is_need_destroy) noexcept;
    image_view(const image_view &) noexcept = delete;
    image_view(image_view &&) noexcept;
    image_view &
    operator=(const image_view &) noexcept = delete;
    image_view &
    operator=(image_view &&) noexcept;
    ~image_view() noexcept;

    // raw
public:
    ktl::api::image_view raw;
    bool                 is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};

using shared_image_view = ktl::memory::sptr< image_view >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::image_view >);

#endif
