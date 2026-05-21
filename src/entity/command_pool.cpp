#include <kochou/context/view.hpp>
#include <kochou/entity/command_pool.hpp>

ktl::errc
kochou::entity::command_pool::ensure(shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::command_pool::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::command_pool::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_command_pool, ktl::errc >
kochou::entity::command_pool::make(kochou::shared_context _sctx) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::command_pool_create_info create_info;
    create_info.flags = static_cast< ktl::api::command_pool_create_flags >(
        ktl::api::command_pool_create_flag_bits::v_reset_command_buffer_bit);
    create_info.queue_family_index = kochou::view::family(_sctx);

    ktl::api::command_pool raw_command_pool;
    ktl::api::result       rc = ktl::api::create_command_pool(device, &create_info, nullptr, &raw_command_pool);
    if (rc != ktl::api::result::v_success)
    {
        kochou::log::error("create_command_pool failed: {}", static_cast< ktl::u32 >(rc));
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_command_pool_rc = ktl::memory::make_shared< command_pool >(std::move(_sctx), raw_command_pool, true);
    if (!shared_command_pool_rc.has_value())
    {
        return ktl::err(shared_command_pool_rc.error());
    }

    return shared_command_pool_rc.take_value();
}

kochou::entity::command_pool::command_pool(kochou::shared_context _sctx, ktl::api::command_pool _command_pool,
                                           bool _is_need_destroy) noexcept
    : raw(_command_pool), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx))
{
}

kochou::entity::command_pool::command_pool(command_pool && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::command_pool &
kochou::entity::command_pool::operator=(command_pool && _rhs) noexcept
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

kochou::entity::command_pool::~command_pool() noexcept
{
    clean();
}

void
kochou::entity::command_pool::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_command_pool(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
