#ifndef KOCHOU_DETAILS_RESOLVE_VERSION_HPP
#define KOCHOU_DETAILS_RESOLVE_VERSION_HPP

#include <ktl/api/version.hpp>
#include <ktl/result.hpp>

namespace kochou::details
{
ktl::result< ktl::api::version, ktl::errc >
resolve_version() noexcept;
}

#endif
