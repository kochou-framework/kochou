#include <kochou/context/view.hpp>
#include <kochou/entity/image.hpp>

ktl::errc
kochou::entity::image::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::image::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::image::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< std::vector< kochou::entity::shared_image >, ktl::errc >
kochou::entity::image::make(kochou::shared_context _sctx, kochou::entity::shared_swapchain _swapchain) noexcept
{
    auto device    = kochou::view::device(_sctx);
    auto swapchain = _swapchain->raw;

    ktl::u32 amount = 0;
    auto     rc     = ktl::api::get_swapchain_images_khr(device, swapchain, &amount, nullptr);
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("get_swapchain_images_khr failed, rc={}", static_cast< ktl::i32 >(rc));
        return ktl::err(ktl::cast_api_result(rc));
    }

    if (amount == 0)
    {
        kochou::log::error("images amount is zero failed");
        return ktl::err(ktl::errc::unknown);
    }

    std::vector< ktl::api::image > raw_images(amount);
    rc = ktl::api::get_swapchain_images_khr(device, swapchain, &amount, raw_images.data());
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("get_swapchain_images_khr failed, rc={}", static_cast< ktl::i32 >(rc));
        return ktl::err(ktl::cast_api_result(rc));
    }

    std::vector< shared_image > shared_images(amount);
    for (ktl::usize i = 0; i < raw_images.size(); ++i)
    {
        auto shared_image_rc = ktl::memory::make_shared< image >(_swapchain, raw_images[i], false);
        if (!shared_image_rc.has_value())
        {
            return ktl::err(shared_image_rc.error());
        }
        shared_images[i] = shared_image_rc.take_value();
    }

    return std::move(shared_images);
}

kochou::entity::image::image(kochou::shared_context _sctx, kochou::entity::shared_swapchain _swapchain,
                             ktl::api::image _image, bool _is_need_destroy) noexcept
    : raw(_image), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx)), swapchain_(std::move(_swapchain))
{
}

kochou::entity::image::image(image && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), swapchain_(std::exchange(_rhs.swapchain_, nullptr))
{
}

kochou::entity::image &
kochou::entity::image::operator=(image && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);
    swapchain_      = std::exchange(_rhs.swapchain_, nullptr);

    return *this;
}

kochou::entity::image::~image() noexcept
{
    clean();
}

void
kochou::entity::image::clean() noexcept
{
    if (raw && is_need_destroy && sctx_ && swapchain_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_image(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_      = nullptr;
    swapchain_ = nullptr;
}
