#include <kochou/context/view.hpp>
#include <kochou/entity/buffer.hpp>

ktl::errc
kochou::entity::buffer::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::buffer::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::buffer::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_buffer, ktl::errc >
kochou::entity::buffer::make(kochou::shared_context _sctx, kochou::entity::shared_device_memory _device_memory,
                             ktl::api::dvsize _dvsize, ktl::api::buffer_usage_flags _flags,
                             ktl::api::sharing_mode _sharing) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::buffer_create_info info;
    info.size         = _dvsize;
    info.usage        = _flags;
    info.sharing_mode = _sharing;

    ktl::api::buffer raw_buffer = nullptr;
    ktl::api::result rc         = ktl::api::create_buffer(device, &info, nullptr, &raw_buffer);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    rc = ktl::api::bind_buffer_memory(device, raw_buffer, _device_memory->raw, 0);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_buffer_rc =
        ktl::memory::make_shared< buffer >(std::move(_sctx), std::move(_device_memory), raw_buffer, true);
    if (!shared_buffer_rc.has_value())
    {
        return ktl::err(shared_buffer_rc.error());
    }

    return shared_buffer_rc.take_value();
}

ktl::result< ktl::api::memory_requirements, ktl::errc >
kochou::entity::buffer::memory_requirements(kochou::shared_context _sctx, ktl::api::dvsize _dvsize,
                                            ktl::api::buffer_usage_flags _flags,
                                            ktl::api::sharing_mode       _sharing) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::buffer_create_info info;
    info.size         = _dvsize;
    info.usage        = _flags;
    info.sharing_mode = _sharing;

    ktl::api::buffer raw_buffer = nullptr;
    ktl::api::result rc         = ktl::api::create_buffer(device, &info, nullptr, &raw_buffer);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    ktl::api::memory_requirements requirements;
    ktl::api::get_buffer_memory_requirements(device, raw_buffer, &requirements);
    ktl::api::destroy_buffer(device, raw_buffer, nullptr);

    return requirements;
}

kochou::entity::buffer::buffer(kochou::shared_context _sctx, kochou::entity::shared_device_memory _device_memory,
                               ktl::api::buffer _buffer, bool _is_need_destroy) noexcept
    : raw(_buffer), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx)),
      device_memory_(std::move(_device_memory))
{
}

kochou::entity::buffer::buffer(buffer && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), device_memory_(std::exchange(_rhs.device_memory_, nullptr))
{
}

kochou::entity::buffer &
kochou::entity::buffer::operator=(buffer && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);
    device_memory_  = std::exchange(_rhs.device_memory_, nullptr);

    return *this;
}

kochou::entity::buffer::~buffer() noexcept
{
    kochou::log::debug("~buffer");
    clean();
}

void
kochou::entity::buffer::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_buffer(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_          = nullptr;
    device_memory_ = nullptr;
}
