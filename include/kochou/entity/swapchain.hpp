#ifndef KOCHOU_ENTITY_SWAPCHAIN_HPP
#define KOCHOU_ENTITY_SWAPCHAIN_HPP

#include <ktl/errc.hpp>
#include <ktl/result.hpp>
#include <ktl/type.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/image.hpp>
#include <kochou/entity/image_view.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class swapchain
{
public:
    struct input_info
    {
        ktl::u32                   buffering;
        ktl::api::format           format;
        ktl::api::color_space_khr  color_space;
        ktl::api::present_mode_khr present_mode;
        bool                       is_strict;
    };
    // fallbacks if not is_strict
    // buffering -> any available
    // format -> any available
    // color_space -> any available
    // present_mode -> any available
    struct output_info
    {
        ktl::u32                   buffering;
        ktl::api::format           format;
        ktl::api::present_mode_khr present_mode;
    };

public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;

private:
    swapchain(ktl::errc & _errc, kochou::shared_context _sctx, const kochou::entity::surface & _surface,
              const input_info & _input, output_info & _output) noexcept;

public:
    ktl::result< std::tuple< swapchain, output_info >, ktl::errc >
    make(kochou::shared_context _sctx, const kochou::entity::surface & _surface, const input_info & _info) noexcept;
    swapchain(const swapchain &) noexcept = delete;
    swapchain(swapchain &&) noexcept;
    swapchain &
    operator=(const swapchain &) noexcept = delete;
    swapchain &
    operator=(swapchain &&) noexcept;
    ~swapchain() noexcept;

private:
    kochou::shared_context                           sctx_;
    ktl::api::swapchain_khr                          swapchain_;
    std::vector< kochou::entity::shared_image >      images_;
    std::vector< kochou::entity::shared_image_view > image_views;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::swapchain >);

#endif
