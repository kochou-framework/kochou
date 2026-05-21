#include <kochou/context/view.hpp>
#include <kochou/entity/image_view.hpp>

ktl::errc
kochou::entity::image_view::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::image_view::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::image_view::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< std::vector< kochou::entity::shared_image_view >, ktl::errc >
kochou::entity::image_view::make(kochou::shared_context _sctx, std::span< kochou::entity::shared_image > _shared_images,
                                 ktl::api::format _format, ktl::api::image_aspect_flag_bits _aspect) noexcept
{
    auto device = kochou::view::device(_sctx);

    std::vector< shared_image_view > shared_image_views(_shared_images.size());
    for (ktl::usize i = 0; i < _shared_images.size(); ++i)
    {
        ktl::api::image_view_create_info create_info{};
        create_info.image                              = _shared_images[i]->raw;
        create_info.view_type                          = ktl::api::image_view_type::v_2d;
        create_info.format                             = _format;
        create_info.components.r                       = ktl::api::component_swizzle::v_identity;
        create_info.components.g                       = ktl::api::component_swizzle::v_identity;
        create_info.components.b                       = ktl::api::component_swizzle::v_identity;
        create_info.components.a                       = ktl::api::component_swizzle::v_identity;
        create_info.subresource_range.aspect_mask      = static_cast< ktl::api::image_aspect_flags >(_aspect);
        create_info.subresource_range.base_mip_level   = 0;
        create_info.subresource_range.level_count      = 1;
        create_info.subresource_range.base_array_layer = 0;
        create_info.subresource_range.layer_count      = 1;

        ktl::api::image_view raw_image_view = nullptr;
        ktl::api::result     rc = ktl::api::create_image_view(device, &create_info, nullptr, &raw_image_view);
        if (rc != ktl::api::result::v_success)
        {
            return ktl::err(ktl::cast_api_result(rc));
        }

        auto shared_image_view_rc = ktl::memory::make_shared< image_view >(_sctx, raw_image_view, true);
        if (!shared_image_view_rc.has_value())
        {
            return ktl::err(shared_image_view_rc.error());
        }
        shared_image_views[i] = shared_image_view_rc.take_value();
    }

    return std::move(shared_image_views);
}

kochou::entity::image_view::image_view(kochou::shared_context _sctx, kochou::entity::shared_image _shared_image,
                                       ktl::api::image_view _image_view, bool _is_need_destroy) noexcept
    : raw(_image_view), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx)), image_(std::move(_shared_image))
{
}

kochou::entity::image_view::image_view(image_view && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), image_(std::exchange(_rhs.image_, nullptr))
{
}

kochou::entity::image_view &
kochou::entity::image_view::operator=(image_view && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);
    image_          = std::exchange(_rhs.image_, nullptr);

    return *this;
}

kochou::entity::image_view::~image_view() noexcept
{
    clean();
}

void
kochou::entity::image_view::clean() noexcept
{
    if (raw && is_need_destroy && sctx_ && image_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_image_view(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_  = nullptr;
    image_ = nullptr;
}
