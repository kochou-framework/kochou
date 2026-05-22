#include <kochou/context/view.hpp>
#include <kochou/entity/descriptor_set.hpp>

ktl::errc
kochou::entity::descriptor_set::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::descriptor_set::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::descriptor_set::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_descriptor_set, ktl::errc >
kochou::entity::descriptor_set::make(kochou::shared_context                  _sctx,
                                     kochou::entity::shared_descriptor_pool  _descriptor_pool,
                                     const ktl::api::descriptor_set_layout & _layout) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::descriptor_set_allocate_info alloc_info;
    alloc_info.descriptor_pool      = _descriptor_pool->raw;
    alloc_info.descriptor_set_count = 1;
    alloc_info.p_set_layouts        = &_layout;

    ktl::api::descriptor_set raw_set = nullptr;
    ktl::api::result         rc      = ktl::api::allocate_descriptor_sets(device, &alloc_info, &raw_set);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_set_rc = ktl::memory::make_shared< descriptor_set >(std::move(_sctx), _descriptor_pool, raw_set, true);
    if (!shared_set_rc.has_value())
    {
        return ktl::err(shared_set_rc.error());
    }

    return shared_set_rc.take_value();
}

kochou::entity::descriptor_set::descriptor_set(kochou::shared_context                 _sctx,
                                               kochou::entity::shared_descriptor_pool _descriptor_pool,
                                               ktl::api::descriptor_set _descriptor_set, bool _is_need_destroy) noexcept
    : raw(_descriptor_set), is_need_destroy(_is_need_destroy), sctx_(_sctx), descriptor_pool_(_descriptor_pool)
{
}

kochou::entity::descriptor_set::descriptor_set(descriptor_set && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), descriptor_pool_(std::exchange(_rhs.descriptor_pool_, nullptr))
{
}

kochou::entity::descriptor_set &
kochou::entity::descriptor_set::operator=(descriptor_set && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw              = std::exchange(_rhs.raw, nullptr);
    is_need_destroy  = std::exchange(_rhs.is_need_destroy, false);
    sctx_            = std::exchange(_rhs.sctx_, nullptr);
    descriptor_pool_ = std::exchange(_rhs.descriptor_pool_, nullptr);

    return *this;
}

kochou::entity::descriptor_set::~descriptor_set() noexcept
{
    clean();
}

void
kochou::entity::descriptor_set::clean() noexcept
{
    if (raw && is_need_destroy && sctx_ && descriptor_pool_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::free_descriptor_sets(device, descriptor_pool_->raw, 1, &raw);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_            = nullptr;
    descriptor_pool_ = nullptr;
}
