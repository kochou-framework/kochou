#include <kochou/base/device.hpp>

kochou::device::~device() noexcept
{
    kochou::log::debug("~device");
}

void
kochou::device::clean() noexcept
{
    if (handle_)
    {
        ktl::api::destroy_device(handle_, nullptr);
    }
}
