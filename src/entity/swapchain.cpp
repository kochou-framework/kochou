#include <algorithm>
#include <limits>

#include <ktl/api.hpp>

#include <kochou/context/view.hpp>
#include <kochou/entity/surface.hpp>
#include <kochou/entity/swapchain.hpp>
#include <kochou/log.hpp>
#include <kochou/requirements/allowed.hpp>
#include <kochou/requirements/ensure.hpp>
#include <kochou/requirements/extension.hpp>
#include <kochou/requirements/should.hpp>

/*
struct swapchain_create_info_khr
{
    ktl::api::structure_type                  stype = ktl::api::structure_type::v_swapchain_create_info_khr;
    const void *                              pnext = {};
    ktl::api::swapchain_create_flags_khr      flags = {};
    ktl::api::surface_khr                     surface;
    ktl::u32                                  min_image_count;
    ktl::api::format                          image_format;
    ktl::api::color_space_khr                 image_color_space;
    ktl::api::extent_2d                       image_extent;
    ktl::u32                                  image_array_layers;
    ktl::api::image_usage_flags               image_usage;
    ktl::api::sharing_mode                    image_sharing_mode;
    ktl::u32                                  queue_family_index_count = {};
    const ktl::u32 *                          p_queue_family_indices;
    ktl::api::surface_transform_flag_bits_khr pre_transform;
    ktl::api::composite_alpha_flag_bits_khr   composite_alpha;
    ktl::api::present_mode_khr                present_mode;
    ktl::api::bool32                          clipped;
    ktl::api::swapchain_khr                   old_swapchain = {};
};
*/

namespace
{
ktl::result< std::tuple< std::vector< ktl::api::format >, std::vector< ktl::api::color_space_khr > >, ktl::errc >
get_colors(ktl::api::physical_device _physical_device, ktl::api::surface_khr _surface) noexcept
{
    ktl::u32         amount = 0;
    ktl::api::result rc =
        ktl::api::get_physical_device_surface_formats_khr(_physical_device, _surface, &amount, nullptr);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    std::vector< ktl::api::surface_format_khr > surface_formats(amount);
    rc = ktl::api::get_physical_device_surface_formats_khr(_physical_device, _surface, &amount, surface_formats.data());
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    std::vector< ktl::api::format >          formats(amount);
    std::vector< ktl::api::color_space_khr > color_spaces(amount);
    for (ktl::u32 i = 0; i < amount; ++i)
    {
        formats.push_back(surface_formats[i].format);
        color_spaces.push_back(surface_formats[i].color_space);
    }

    return std::make_tuple(formats, color_spaces);
}

ktl::result< std::vector< ktl::api::present_mode_khr >, ktl::errc >
get_present_modes(ktl::api::physical_device _physical_device, ktl::api::surface_khr _surface) noexcept
{
    ktl::u32         amount = 0;
    ktl::api::result rc =
        ktl::api::get_physical_device_surface_present_modes_khr(_physical_device, _surface, &amount, nullptr);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    std::vector< ktl::api::present_mode_khr > present_modes(amount);
    rc = ktl::api::get_physical_device_surface_present_modes_khr(_physical_device, _surface, &amount,
                                                                 present_modes.data());
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    return present_modes;
}

ktl::result< ktl::api::surface_capabilities_khr, ktl::errc >
get_capabilities(ktl::api::physical_device _physical_device, ktl::api::surface_khr _surface) noexcept
{
    ktl::api::surface_capabilities_khr capabilities;

    ktl::api::result rc =
        ktl::api::get_physical_device_surface_capabilities_khr(_physical_device, _surface, &capabilities);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    return capabilities;
}
} // namespace

ktl::errc
kochou::entity::swapchain::ensure(kochou::shared_context _sctx) noexcept
{
    return kochou::ensure< kochou::extension< ktl::api::extension::khr_swapchain > >(_sctx);
}

ktl::errc
kochou::entity::swapchain::should(kochou::shared_context _sctx) noexcept
{
    return kochou::should< kochou::extension< ktl::api::extension::khr_swapchain > >(_sctx);
}

bool
kochou::entity::swapchain::allowed(kochou::shared_context _sctx) noexcept
{
    return kochou::allowed< kochou::extension< ktl::api::extension::khr_swapchain > >(_sctx);
}

// TODO debug properties resolve
kochou::entity::swapchain::swapchain(ktl::errc & _errc, kochou::shared_context _sctx,
                                     kochou::entity::shared_surface _surface, const input_info & _input,
                                     output_info & _output) noexcept
    : sctx_(_sctx), swapchain_()
{
    auto physical_device = kochou::view::physical_device(_sctx);
    kochou::log::debug("physical_device={}", physical_device);
    auto surface = _surface->raw();
    kochou::log::debug("surface={}", surface);
    auto width = _surface->width();
    kochou::log::debug("width={}", width);
    auto height = _surface->height();
    kochou::log::debug("height={}", height);

    auto colors_rc = get_colors(physical_device, surface);
    if (!colors_rc.has_value())
    {
        kochou::log::error("get_colors failed");
        _errc = colors_rc.error();
        return;
    }
    auto [formats, color_spaces] = colors_rc.take_value();

    auto present_modes_rc = get_present_modes(physical_device, surface);
    if (!present_modes_rc.has_value())
    {
        kochou::log::error("get_present_modes failed");
        _errc = present_modes_rc.error();
        return;
    }
    auto present_modes = present_modes_rc.take_value();

    auto capabilities_rc = get_capabilities(physical_device, surface);
    if (!capabilities_rc.has_value())
    {
        kochou::log::error("get_capabilities failed");
        _errc = capabilities_rc.error();
        return;
    }
    auto capabilities = capabilities_rc.take_value();

    ktl::api::format          chosen_format = _input.format;
    ktl::api::color_space_khr chosen_cs     = _input.color_space;
    bool                      found         = false;
    for (ktl::u32 i = 0; i < formats.size(); ++i)
    {
        if (formats[i] == chosen_format && color_spaces[i] == chosen_cs)
        {
            found = true;
            break;
        }
    }
    if (!found && !_input.is_strict && !formats.empty())
    {
        chosen_format = formats[0];
        chosen_cs     = color_spaces[0];
    }
    else if (!found)
    {
        _errc = ktl::errc::unsupported_format;
    }
    kochou::log::debug("format resolved, chosen={}", static_cast< ktl::u32 >(chosen_format));
    kochou::log::debug("color_space resolved, chosen={}", static_cast< ktl::u32 >(chosen_cs));

    ktl::api::present_mode_khr chosen_mode = _input.present_mode;
    if (std::find(present_modes.begin(), present_modes.end(), chosen_mode) == present_modes.end())
    {
        if (!_input.is_strict && !present_modes.empty())
        {
            chosen_mode = present_modes[0];
        }
        else
        {
            _errc = ktl::errc::unsupported_present_mode;
        }
    }
    kochou::log::debug("present mode resolved, chosen={}", static_cast< ktl::u32 >(chosen_mode));

    ktl::api::extent_2d actual_extent{};
    if (capabilities.current_extent.width == std::numeric_limits< ktl::u32 >::max())
    {
        actual_extent.width =
            std::clamp(width, capabilities.min_image_extent.width, capabilities.max_image_extent.width);
        actual_extent.height =
            std::clamp(height, capabilities.min_image_extent.height, capabilities.max_image_extent.height);
    }
    else
    {
        actual_extent = capabilities.current_extent;
    }
    kochou::log::debug("extend2d resolved, chosen={}:{}", actual_extent.width, actual_extent.height);

    ktl::api::swapchain_create_info_khr create_info;
    create_info.surface = surface;
    create_info.min_image_count =
        std::clamp(_input.buffering, capabilities.min_image_count, capabilities.max_image_count);
    create_info.image_format       = chosen_format;
    create_info.image_color_space  = chosen_cs;
    create_info.image_extent       = actual_extent;
    create_info.image_array_layers = 1;
    create_info.image_usage =
        static_cast< ktl::api::image_usage_flags >(ktl::api::image_usage_flag_bits::v_color_attachment_bit);
    create_info.image_sharing_mode = ktl::api::sharing_mode::v_exclusive;
    create_info.pre_transform      = capabilities.current_transform;
    create_info.composite_alpha    = ktl::api::composite_alpha_flag_bits_khr::v_opaque_bit_khr;
    create_info.present_mode       = chosen_mode;
    create_info.clipped            = true;
    create_info.old_swapchain      = {};

    ktl::api::swapchain_khr raw_handle{};
    auto rc = ktl::api::create_swapchain_khr(kochou::view::device(_sctx), &create_info, nullptr, &raw_handle);
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::debug("create_swapchain_khr failed");
        _errc = ktl::cast_api_result(rc);
    }

    _output = {create_info.min_image_count, chosen_format, chosen_mode};
    _errc   = ktl::errc::success;
    kochou::log::debug("swapchain creation success");
}

ktl::result< std::tuple< kochou::entity::swapchain, kochou::entity::swapchain::output_info >, ktl::errc >
kochou::entity::swapchain::make(kochou::shared_context _sctx, kochou::entity::shared_surface _surface,
                                const input_info & _info) noexcept
{
    ktl::errc   rc;
    output_info output;
    kochou::log::debug("ready to construct swapchain");
    swapchain sw(rc, _sctx, _surface, _info, output);
    if (rc != ktl::errc::success)
    {
        kochou::log::error("swapchain creation failed");
        return ktl::err(rc);
    }
    return std::make_tuple(std::move(sw), output);
}

kochou::entity::swapchain::swapchain(swapchain && rhs) noexcept : sctx_(rhs.sctx_), swapchain_(rhs.swapchain_)
{
    if (std::addressof(rhs) == this)
    {
        return;
    }

    rhs.sctx_      = nullptr;
    rhs.swapchain_ = nullptr;
}

kochou::entity::swapchain &
kochou::entity::swapchain::operator=(swapchain && rhs) noexcept
{
    if (std::addressof(rhs) == this)
    {
        return *this;
    }
    sctx_          = rhs.sctx_;
    swapchain_     = rhs.swapchain_;
    rhs.sctx_      = nullptr;
    rhs.swapchain_ = nullptr;

    return *this;
}

kochou::entity::swapchain::~swapchain() noexcept
{
    if (swapchain_)
    {
        ktl::api::destroy_swapchain_khr(kochou::view::device(sctx_), swapchain_, nullptr);
    }
}
