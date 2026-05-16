#include <kochou/base/physical_device.hpp>

kochou::physical_device::physical_device(ktl::errc & _errc, ktl::api::physical_device _physical_device) noexcept
    : handle_(_physical_device)
{
    _errc = ktl::errc::success;
};

kochou::physical_device::~physical_device() noexcept {}
