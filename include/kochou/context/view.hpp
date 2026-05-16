#ifndef KOCHOU_CONTEXT_VIEW_HPP
#define KOCHOU_CONTEXT_VIEW_HPP

#include <ktl/api.hpp>
#include <ktl/memory/pointers.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
struct view
{
    [[nodiscard]]
    inline static ktl::api::pfn_table *
    ptable(kochou::shared_context & _sctx);

    [[nodiscard]]
    inline static ktl::api::instance
    instance(kochou::shared_context & _sctx);

    [[nodiscard]]
    inline static ktl::api::physical_device
    physical_device(kochou::shared_context & _sctx);

    [[nodiscard]]
    inline static ktl::api::device
    device(kochou::shared_context & _sctx);
};
} // namespace kochou

inline ktl::api::pfn_table *
kochou::view::ptable(kochou::shared_context & _sctx)
{
    return &_sctx->ptable_;
}

ktl::api::instance
kochou::view::instance(kochou::shared_context & _sctx)
{
    return _sctx->instance_->handle_;
}

ktl::api::physical_device
kochou::view::physical_device(kochou::shared_context & _sctx)
{
    return _sctx->physical_device_->handle_;
}

ktl::api::device
kochou::view::device(kochou::shared_context & _sctx)
{
    return _sctx->device_->handle_;
}

#endif
