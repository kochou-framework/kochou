#ifndef KOCHOU_ENTITY_SWAPCHAIN_HPP
#define KOCHOU_ENTITY_SWAPCHAIN_HPP

#include <ktl/errc.hpp>
#include <ktl/result.hpp>
#include <ktl/type.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/interface.hpp>
#include <kochou/entity/surface.hpp>

namespace kochou::entity
{
class swapchain;
using shared_swapchain = ktl::memory::sptr< kochou::entity::swapchain >;

class swapchain
{
    // structs
public:
    struct input_info
    {
        ktl::u32                   buffering;
        ktl::api::format           format;
        ktl::api::color_space_khr  color_space;
        ktl::api::present_mode_khr present_mode;
        bool                       is_strict;
    };
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
    swapchain(kochou::shared_context _sctx, kochou::entity::shared_surface _surface, ktl::api::swapchain_khr _swapchain,
              bool _is_need_destroy) noexcept;
    swapchain(const swapchain &) noexcept = delete;
    swapchain(swapchain &&) noexcept;
    swapchain &
    operator=(const swapchain &) noexcept = delete;
    swapchain &
    operator=(swapchain &&) noexcept;
    ~swapchain() noexcept;

    // raw
public:
    ktl::api::swapchain_khr raw;
    bool                    is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context         sctx_;
    kochou::entity::shared_surface surface_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::swapchain >);

#endif
