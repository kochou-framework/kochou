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

ktl::result< std::vector< kochou::entity::image >, ktl::errc >
kochou::entity::image::make(kochou::shared_context _sctx, kochou::entity::shared_swapchain _swapchain) noexcept
{
    auto device = kochou::view::device(_sctx);
    kochou::log::debug("device={}", device);
    auto swapchain = _swapchain->raw;
    kochou::log::debug("swapchain={}", swapchain);

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

    std::vector< image > images;
    images.reserve(amount);
    for (auto raw : raw_images)
    {
        images.push_back(image(_swapchain, raw));
    }

    return std::move(images);
}

kochou::entity::image::image(kochou::entity::shared_swapchain _swapchain, ktl::api::image _image) noexcept
    : raw(_image), swapchain_(_swapchain)
{
}

kochou::entity::image::~image() noexcept
{ // image should be destroyed by source
}
