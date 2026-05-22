#include <kochou/context/view.hpp>
#include <kochou/entity/descriptor_pool.hpp>

ktl::errc
kochou::entity::descriptor_pool::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::descriptor_pool::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::descriptor_pool::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_descriptor_pool, ktl::errc >
kochou::entity::descriptor_pool::make(kochou::shared_context _sctx, ktl::u32 _max_sets,
                                      std::span< ktl::api::descriptor_pool_size > _sets) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::descriptor_pool_create_info info;
    info.flags           = 0;
    info.max_sets        = _max_sets;
    info.pool_size_count = _sets.size();
    info.p_pool_sizes    = _sets.data();

    ktl::api::descriptor_pool raw_pool = nullptr;
    ktl::api::result          rc       = ktl::api::create_descriptor_pool(device, &info, nullptr, &raw_pool);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_pool_rc = ktl::memory::make_shared< descriptor_pool >(std::move(_sctx), raw_pool, true);
    if (!shared_pool_rc.has_value())
    {
        return ktl::err(shared_pool_rc.error());
    }

    return shared_pool_rc.take_value();
}

kochou::entity::descriptor_pool::descriptor_pool(kochou::shared_context    _sctx,
                                                 ktl::api::descriptor_pool _descriptor_pool,
                                                 bool                      _is_need_destroy) noexcept
    : raw(_descriptor_pool), is_need_destroy(_is_need_destroy), sctx_(_sctx)
{
}

kochou::entity::descriptor_pool::descriptor_pool(descriptor_pool && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::descriptor_pool &
kochou::entity::descriptor_pool::operator=(descriptor_pool && _rhs) noexcept
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

kochou::entity::descriptor_pool::~descriptor_pool() noexcept
{
    clean();
}

void
kochou::entity::descriptor_pool::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_descriptor_pool(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
