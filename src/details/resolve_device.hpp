#ifndef KOCHOU_DETAILS_RESOLVE_DEVICE_HPP
#define KOCHOU_DETAILS_RESOLVE_DEVICE_HPP

#include <set>
#include <tuple>

#include <ktl/api.hpp>
#include <ktl/api/version.hpp>
#include <ktl/meta.hpp>
#include <ktl/result.hpp>

namespace kochou::details
{
ktl::result< std::tuple< ktl::api::physical_device, std::set< ktl::api::feature >, std::set< ktl::api::extension >,
                         ktl::u32, ktl::api::queue_flags >,
             ktl::errc >
resolve_device(const ktl::api::version & _version, ktl::api::instance _instance,
               const std::set< ktl::meta::dependency > & _ensure, const std::set< ktl::meta::dependency > & _should,
               ktl::api::queue_flags _ensure_queue, ktl::api::queue_flags _should_queue) noexcept;
} // namespace kochou::details

#endif
