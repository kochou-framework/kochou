#include "resolve_queue.hpp"

#include <bit>

#include <kochou/log.hpp>

namespace
{
std::vector< ktl::api::queue_family_properties >
get_queue_family_properties(ktl::api::physical_device _physical_device) noexcept
{
    ktl::u32 amount = 0;
    ktl::api::get_physical_device_queue_family_properties(_physical_device, &amount, nullptr);

    std::vector< ktl::api::queue_family_properties > properties(amount);
    ktl::api::get_physical_device_queue_family_properties(_physical_device, &amount, properties.data());
    return properties;
}
} // namespace

ktl::result< std::tuple< ktl::u32, ktl::api::queue_flags >, ktl::errc >
kochou::details::get_queue_family_index(ktl::api::physical_device _physical_device, ktl::api::queue_flags _ensure,
                                        ktl::api::queue_flags _should) noexcept
{
    kochou::log::debug("ensure queue={}", _ensure);
    kochou::log::debug("should queue={}", _should);

    ktl::usize            best_score = 0;
    ktl::api::queue_flags allowed    = 0;
    ktl::u32              index      = 0;
    bool                  found      = false;
    auto                  properties = get_queue_family_properties(_physical_device);

    for (ktl::usize i = 0; i < properties.size(); ++i)
    {
        ktl::api::queue_flags available = properties[i].queue_flags;
        kochou::log::debug(
            "family[{}]: graphics={}, compute={}, transfer={}", i,
            !!(available & static_cast< ktl::api::queue_flags >(ktl::api::queue_flag_bits::v_graphics_bit)),
            !!(available & static_cast< ktl::api::queue_flags >(ktl::api::queue_flag_bits::v_compute_bit)),
            !!(available & static_cast< ktl::api::queue_flags >(ktl::api::queue_flag_bits::v_transfer_bit)));
        ktl::usize score = 0;

        if ((available & _ensure) == _ensure)
        {
            ++score;
        }
        score += std::popcount(available & _should);
        if (properties[i].queue_count > 1)
        {
            score += 1;
        }

        if (score > best_score)
        {
            allowed    = available;
            best_score = score;
            index      = i;
            found      = true;
        }
    }

    if (!found)
    {
        kochou::log::error("resolve queue failed");
        return ktl::err(ktl::errc::queue_not_provides);
    }
    kochou::log::debug("-> family[{}]: graphics={}, compute={}, transfer={}", index,
                       !!(allowed & static_cast< ktl::api::queue_flags >(ktl::api::queue_flag_bits::v_graphics_bit)),
                       !!(allowed & static_cast< ktl::api::queue_flags >(ktl::api::queue_flag_bits::v_compute_bit)),
                       !!(allowed & static_cast< ktl::api::queue_flags >(ktl::api::queue_flag_bits::v_transfer_bit)));
    return std::make_tuple(index, allowed);
}
