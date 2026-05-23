#include <kochou/context/view.hpp>
#include <kochou/entity/semaphore.hpp>

ktl::errc
kochou::entity::semaphore::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::semaphore::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::semaphore::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_semaphore, ktl::errc >
kochou::entity::semaphore::make(kochou::shared_context _sctx) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::semaphore_create_info info;

    ktl::api::semaphore raw_semaphore;
    ktl::api::result    rc = ktl::api::create_semaphore(device, &info, nullptr, &raw_semaphore);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_semaphore_rc = ktl::memory::make_shared< semaphore >(std::move(_sctx), raw_semaphore, true);
    if (!shared_semaphore_rc.has_value())
    {
        return ktl::err(shared_semaphore_rc.error());
    }

    return shared_semaphore_rc.take_value();
}

kochou::entity::semaphore::semaphore(kochou::shared_context _sctx, ktl::api::semaphore _semaphore,
                                     bool _is_need_destroy) noexcept
    : raw(_semaphore), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx))
{
}

kochou::entity::semaphore::semaphore(semaphore && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::semaphore &
kochou::entity::semaphore::operator=(semaphore && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);

    return *this;
}

kochou::entity::semaphore::~semaphore() noexcept
{
    kochou::log::debug("~semaphore");
    clean();
}

void
kochou::entity::semaphore::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_semaphore(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
