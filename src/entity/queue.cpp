#include <kochou/context/view.hpp>
#include <kochou/entity/fence.hpp>
#include <kochou/entity/queue.hpp>

ktl::errc
kochou::entity::queue::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::queue::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::queue::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_queue, ktl::errc >
kochou::entity::queue::make(kochou::shared_context _sctx, queue_type _type) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::u32 chosen_queue_type; // trash
    switch (_type)
    {
    case present:
    case graphics:
        chosen_queue_type = 0;
        break;
    case compute:
        chosen_queue_type = 1;
        break;
    case transfer:
        chosen_queue_type = 2;
        break;
    default:
        return ktl::err(ktl::errc::queue_wrong_value);
    }

    ktl::api::queue raw_queue = nullptr;
    ktl::api::get_device_queue(device, chosen_queue_type, 0, &raw_queue);
    if (!raw_queue)
    {
        return ktl::err(ktl::errc::unknown);
    }

    auto shared_queue_rc = ktl::memory::make_shared< queue >(std::move(_sctx), raw_queue, false);
    if (!shared_queue_rc.has_value())
    {
        return ktl::err(shared_queue_rc.error());
    }

    return shared_queue_rc.take_value();
}

kochou::entity::queue::queue(kochou::shared_context _sctx, ktl::api::queue _queue, bool) noexcept
    : raw(_queue), sctx_(std::move(_sctx))
{
}

kochou::entity::queue::queue(queue && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::queue &
kochou::entity::queue::operator=(queue && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    // not need to clean

    raw   = std::exchange(_rhs.raw, nullptr);
    sctx_ = std::exchange(_rhs.sctx_, nullptr);

    return *this;
}

kochou::entity::queue::~queue() noexcept
{
    // nothing to do
}

ktl::errc
kochou::entity::queue::submit(const ktl::api::submit_info & info, kochou::entity::shared_fence _fence) noexcept
{
    return ktl::cast_api_result(ktl::api::queue_submit(raw, 1, &info, _fence->raw));
}
