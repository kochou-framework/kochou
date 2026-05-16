#include <kochou/base/device.hpp>

kochou::device::~device() noexcept
{
    if (handle_)
    {
        ktl::api::destroy_device(handle_, nullptr);
    }
}
