#include "resolve_version.hpp"

#include <ktl/api.hpp>

namespace
{
ktl::result< ktl::api::version, ktl::errc >
get_allowed_instance_version() noexcept
{
    ktl::u32         version = {};
    ktl::api::result rc      = ktl::api::enumerate_instance_version(&version);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }
    return ktl::api::version(version);
}

ktl::api::version
round_version(ktl::api::version _version) noexcept
{
    return ktl::api::version(_version.variant, _version.major, _version.minor, 0);
}
} // namespace

ktl::result< ktl::api::version, ktl::errc >
kochou::details::resolve_version() noexcept
{
    auto version_rc = get_allowed_instance_version();
    if (!version_rc.has_value())
    {
        return ktl::err(version_rc.error());
    }
    return round_version(version_rc.take_value());
}
