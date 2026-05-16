#ifndef KOCHOU_BASE_PHYSICAL_DEVICE_HPP
#define KOCHOU_BASE_PHYSICAL_DEVICE_HPP

#include <ktl/api.hpp>

#include <kochou/external.hpp>

namespace kochou
{
struct requirements;
struct view;

class physical_device : public kochou::external< kochou::hold::shared, kochou::physical_device >
{
private:
    friend struct kochou::view;

public: // TODO make private
    physical_device(ktl::errc & _errc, ktl::api::physical_device _physical_device) noexcept;
    physical_device(const physical_device &) = delete;
    physical_device(physical_device &&)      = delete;
    physical_device &
    operator=(const physical_device &) = delete;
    physical_device &
    operator=(physical_device &&) = delete;
    ~physical_device() noexcept;

private:
    ktl::api::physical_device handle_ = nullptr;
};
} // namespace kochou

#endif
