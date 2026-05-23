#include <format>
#include <ranges>
#include <set>
#include <vector>

#include <ktl/meta.hpp>

#include <kochou/base/instance.hpp>

kochou::instance::~instance() noexcept
{
    kochou::log::debug("~instance");
}

void
kochou::instance::clean() noexcept
{
    if (handle_)
    {
        ktl::api::destroy_instance(handle_, nullptr);
    }
}
