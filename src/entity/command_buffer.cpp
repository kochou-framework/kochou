#include <kochou/context/view.hpp>
#include <kochou/entity/command_buffer.hpp>

ktl::errc
kochou::entity::command_buffer::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::command_buffer::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::command_buffer::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_command_buffer, ktl::errc >
kochou::entity::command_buffer::make(kochou::shared_context              _sctx,
                                     kochou::entity::shared_command_pool _shared_command_pool,
                                     ktl::api::command_buffer_level      _level) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::command_buffer_allocate_info alloc_info;
    alloc_info.command_pool         = _shared_command_pool->raw;
    alloc_info.level                = _level;
    alloc_info.command_buffer_count = 1;

    ktl::api::command_buffer raw_command_buffer = nullptr;
    ktl::api::result         rc = ktl::api::allocate_command_buffers(device, &alloc_info, &raw_command_buffer);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_command_buffer_rc = ktl::memory::make_shared< command_buffer >(
        std::move(_sctx), std::move(_shared_command_pool), raw_command_buffer, true);
    if (!shared_command_buffer_rc.has_value())
    {
        return ktl::err(shared_command_buffer_rc.error());
    }

    return shared_command_buffer_rc.take_value();
}

kochou::entity::command_buffer::command_buffer(kochou::shared_context              _sctx,
                                               kochou::entity::shared_command_pool _command_pool,
                                               ktl::api::command_buffer _command_buffer, bool _is_need_destroy) noexcept
    : raw(_command_buffer), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx)),
      command_pool_(std::move(_command_pool))
{
}

kochou::entity::command_buffer::command_buffer(command_buffer && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), command_pool_(std::exchange(_rhs.command_pool_, nullptr))
{
}

kochou::entity::command_buffer &
kochou::entity::command_buffer::operator=(command_buffer && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);
    command_pool_   = std::exchange(_rhs.command_pool_, nullptr);

    return *this;
}

kochou::entity::command_buffer::~command_buffer() noexcept
{
    clean();
}

ktl::errc
kochou::entity::command_buffer::begin(ktl::api::command_buffer_usage_flags _flags) noexcept
{
    ktl::api::command_buffer_begin_info begin_info;
    begin_info.flags              = _flags;
    begin_info.p_inheritance_info = nullptr;

    ktl::api::result rc = ktl::api::begin_command_buffer(raw, &begin_info);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::cast_api_result(rc);
    }
    return ktl::errc::success;
}

ktl::errc
kochou::entity::command_buffer::end() noexcept
{
    return ktl::cast_api_result(ktl::api::end_command_buffer(raw));
}

ktl::errc
kochou::entity::command_buffer::reset(ktl::api::command_buffer_reset_flags _flags) noexcept
{
    return ktl::cast_api_result(ktl::api::reset_command_buffer(raw, _flags));
}

void
kochou::entity::command_buffer::clean() noexcept
{
    if (raw && is_need_destroy && sctx_ && command_pool_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::free_command_buffers(device, command_pool_->raw, 1, &raw);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_         = nullptr;
    command_pool_ = nullptr;
}
