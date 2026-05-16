#ifndef KOCHOU_DETAILS_RESOLVE_QUEUE_HPP
#define KOCHOU_DETAILS_RESOLVE_QUEUE_HPP

#include <ktl/api.hpp>
#include <ktl/result.hpp>

namespace kochou::details
{
ktl::result< std::tuple< ktl::u32, ktl::api::queue_flags >, ktl::errc >
get_queue_family_index(ktl::api::physical_device _physical_device, ktl::api::queue_flags _ensure,
                       ktl::api::queue_flags _should) noexcept;
}

#endif
