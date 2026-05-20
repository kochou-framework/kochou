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

ktl::result< std::vector< kochou::entity::image_view >, ktl::errc >
kochou::entity::image_view::make(kochou::shared_context _sctx, std::span< kochou::entity::image > _images,
                                 ktl::api::format _format, ktl::api::image_aspect_flag_bits _aspect) noexcept
{
    auto device = kochou::view::device(_sctx);

    std::vector< image_view > image_views(_images.size());
    for (ktl::usize i = 0; i < _images.size(); ++i)
    {
        ktl::api::image_view_create_info create_info{};
        create_info.image                              = _images[i].raw;
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

        ktl::api::image_view view_raw = nullptr;
        ktl::api::result     rc       = ktl::api::create_image_view(device, &create_info, nullptr, &view_raw);
        if (rc != ktl::api::result::v_success)
        {
            for (auto & view : image_views)
            {
                if (view.raw)
                {
                    ktl::api::destroy_image_view(device, view.raw, nullptr);
                }
            }
            return ktl::err(ktl::cast_api_result(rc));
        }

        image_views[i] = image_view(_sctx, view_raw, true);
    }

    return image_views;
}

kochou::entity::image_view::image_view(kochou::shared_context _sctx, ktl::api::image_view _image_view,
                                       bool _is_need_destroy) noexcept
    : raw(_image_view), sctx_(_sctx), is_need_destroy_(_is_need_destroy)
{
}

kochou::entity::image_view::~image_view() noexcept
{
    if (raw && is_need_destroy_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_image_view(device, raw, nullptr);
        raw = ktl::api::image_view{};
    }
}
