#include <kochou/context/view.hpp>
#include <kochou/entity/fence.hpp>

ktl::errc
kochou::entity::fence::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::fence::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::fence::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::fence, ktl::errc >
kochou::entity::fence::make(kochou::shared_context _sctx, ktl::api::fence_create_flags _bits) noexcept
{
    auto                        device = kochou::view::device(_sctx);
    ktl::api::fence_create_info info{};
    info.flags = _bits;

    ktl::api::fence fence_raw = {};
    auto            rc        = ktl::api::create_fence(device, &info, nullptr, &fence_raw);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    return fence(std::move(_sctx), fence_raw);
}

kochou::entity::fence::fence(kochou::shared_context _sctx, ktl::api::fence _fence) noexcept : raw(_fence), sctx_(_sctx)
{
}

kochou::entity::fence::~fence() noexcept {}

ktl::errc
kochou::entity::fence::wait(ktl::u64 _timeout) noexcept
{
    return ktl::cast_api_result(ktl::api::wait_for_fences(kochou::view::device(sctx_), 1, &raw, true, _timeout));
}

ktl::errc
kochou::entity::fence::reset() noexcept
{
    return ktl::cast_api_result(ktl::api::reset_fences(kochou::view::device(sctx_), 1, &raw));
}
