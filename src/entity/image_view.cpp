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
    kochou::log::debug("device={}", device);

    std::vector< image_view > image_views;
    image_views.reserve(_images.size());
    kochou::log::debug("images size={}", _images.size());
    kochou::log::debug("image_views size={}", image_views.size());

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
            kochou::log::error("create_image_view failed, rc={}", static_cast< ktl::u32 >(rc));
            for (auto & view : image_views)
            {
                if (view.raw)
                {
                    ktl::api::destroy_image_view(device, view.raw, nullptr);
                }
            }
            return ktl::err(ktl::cast_api_result(rc));
        }
        kochou::log::debug("image_view created");
        image_views.emplace_back(_sctx, view_raw, true);
        kochou::log::debug("image_view moved");
    }

    kochou::log::debug("ready to return image_views");
    return std::move(image_views);
}

kochou::entity::image_view::image_view(kochou::shared_context _sctx, ktl::api::image_view _image_view,
                                       bool _is_need_destroy) noexcept
    : raw(_image_view), is_need_destroy(_is_need_destroy), sctx_(_sctx)
{
}

kochou::entity::image_view::image_view(image_view && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
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

    return *this;
}

kochou::entity::image_view::~image_view() noexcept
{
    clean();
}

void
kochou::entity::image_view::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_image_view(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
