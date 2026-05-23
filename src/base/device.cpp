#include <kochou/base/device.hpp>

kochou::device::~device() noexcept
{
    kochou::log::debug("~device");
    if (handle_)
    {
        ktl::api::destroy_device(handle_, nullptr);
    }
}
