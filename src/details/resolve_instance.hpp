#ifndef KOCHOU_DETAILS_RESOLVE_INSTANCE_HPP
#define KOCHOU_DETAILS_RESOLVE_INSTANCE_HPP

#include <ranges>
#include <set>

#include <ktl/api.hpp>
#include <ktl/api/version.hpp>
#include <ktl/errc.hpp>
#include <ktl/meta.hpp>
#include <ktl/result.hpp>

#include <kochou/log.hpp>

namespace kochou::details
{
ktl::result< std::set< ktl::api::extension >, ktl::errc >
resolve_instance(const std::set< ktl::meta::dependency > & _ensure,
                 const std::set< ktl::meta::dependency > & _should) noexcept;
} // namespace kochou::details

#endif
