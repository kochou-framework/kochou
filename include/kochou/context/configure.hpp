#ifndef KOCHOU_CONTEXT_CONFIGURE_HPP
#define KOCHOU_CONTEXT_CONFIGURE_HPP

#include <kochou/context/context.hpp>

namespace kochou
{
class configure
{
public:
    explicit configure(kochou::shared_context _sctx) noexcept : sctx_(_sctx) {}
    ~configure() noexcept
    {
        auto resolve_rc = sctx_->resolve();
        if (!resolve_rc.has_value())
        {
            sctx_->status_ = resolve_rc.error();
            return;
        }
        auto [instance, physical_device, device] = resolve_rc.take_value();
        sctx_->instance_                         = instance;
        sctx_->physical_device_                  = physical_device;
        sctx_->device_                           = device;
        sctx_->status_                           = ktl::errc::success;
    }

private:
    kochou::shared_context sctx_;
};
} // namespace kochou

#endif
