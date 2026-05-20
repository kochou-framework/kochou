#ifndef KOCHOU_ENTITY_SWAPCHAIN_HPP
#define KOCHOU_ENTITY_SWAPCHAIN_HPP

#include <ktl/errc.hpp>
#include <ktl/result.hpp>
#include <ktl/type.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/image.hpp>
#include <kochou/entity/image_view.hpp>
#include <kochou/entity/interface.hpp>
#include <kochou/entity/surface.hpp>

namespace kochou::entity
{
class swapchain;
using shared_swapchain = ktl::memory::sptr< kochou::entity::swapchain >;

class swapchain
{
public:
    friend class image;
    friend class image_view;

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
    static ktl::result< std::tuple< shared_swapchain, output_info >, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_surface _surface, const input_info & _input) noexcept;

    // common
public:
    swapchain(kochou::shared_context _sctx, ktl::api::swapchain_khr _swapchain, bool _is_need_destroy) noexcept;
    swapchain(const swapchain &) noexcept = delete;
    swapchain(swapchain &&) noexcept      = delete;
    swapchain &
    operator=(const swapchain &) noexcept = delete;
    swapchain &
    operator=(swapchain &&) noexcept = delete;
    ~swapchain() noexcept;

private:
    kochou::shared_context  sctx_;
    ktl::api::swapchain_khr swapchain_;
    bool                    is_need_destroy_;

    std::vector< kochou::entity::shared_image >      images_;
    std::vector< kochou::entity::shared_image_view > image_views_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::swapchain >);

#endif
