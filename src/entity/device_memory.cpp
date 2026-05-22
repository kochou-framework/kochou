#include <limits>

#include <kochou/context/view.hpp>
#include <kochou/entity/device_memory.hpp>

ktl::errc
kochou::entity::device_memory::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::device_memory::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::device_memory::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_device_memory, ktl::errc >
kochou::entity::device_memory::make(kochou::shared_context _sctx, const ktl::api::memory_requirements & _requirements,
                                    ktl::api::memory_property_flags _flags) noexcept
{
    auto physical_device = kochou::view::physical_device(_sctx);
    auto device          = kochou::view::device(_sctx);

    ktl::api::physical_device_memory_properties memory;
    ktl::api::get_physical_device_memory_properties(physical_device, &memory);

    ktl::u32 memory_type_index = std::numeric_limits< ktl::u32 >::max();
    for (ktl::u32 i = 0; i < memory.memory_type_count; ++i)
    {
        if ((_requirements.memory_type_bits & (1 << i)) && (memory.memory_types[i].property_flags & _flags) == _flags)
        {
            memory_type_index = i;
            break;
        }
    }
    if (memory_type_index == std::numeric_limits< ktl::u32 >::max())
    {
        return ktl::err(ktl::errc::invalid_argument);
    }

    ktl::api::memory_allocate_info info;
    info.allocation_size   = _requirements.size;
    info.memory_type_index = memory_type_index;

    ktl::api::device_memory raw_memory = nullptr;
    ktl::api::result        rc         = ktl::api::allocate_memory(device, &info, nullptr, &raw_memory);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_memory_rc = ktl::memory::make_shared< device_memory >(std::move(_sctx), raw_memory, true);
    if (!shared_memory_rc.has_value())
    {
        return ktl::err(shared_memory_rc.error());
    }

    return shared_memory_rc.take_value();
}

kochou::entity::device_memory::device_memory(kochou::shared_context _sctx, ktl::api::device_memory _device_memory,
                                             bool _is_need_destroy) noexcept
    : raw(_device_memory), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx))
{
}

kochou::entity::device_memory::device_memory(device_memory && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::device_memory &
kochou::entity::device_memory::operator=(device_memory && _rhs) noexcept
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

kochou::entity::device_memory::~device_memory() noexcept
{
    clean();
}

void
kochou::entity::device_memory::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::free_memory(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
