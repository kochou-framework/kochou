#ifndef KOCHOU_ENTITY_DEVICE_MEMORY_HPP
#define KOCHOU_ENTITY_DEVICE_MEMORY_HPP

#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>

namespace kochou::entity
{
class device_memory;
using shared_device_memory = ktl::memory::sptr< device_memory >;

class device_memory
{
    // requirements
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;

    // fabrics
public:
    static ktl::result< shared_device_memory, ktl::errc >
    make(kochou::shared_context _sctx, const ktl::api::memory_requirements & _requirements,
         ktl::api::memory_property_flags _flags) noexcept;

    // common
public:
    device_memory(kochou::shared_context _sctx, ktl::api::device_memory _device_memory, bool _is_need_destroy) noexcept;
    device_memory(const device_memory &) noexcept = delete;
    device_memory(device_memory &&) noexcept;
    device_memory &
    operator=(const device_memory &) noexcept = delete;
    device_memory &
    operator=(device_memory &&) noexcept;
    ~device_memory() noexcept;

    // raw
public:
    ktl::api::device_memory raw;
    bool                    is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

#endif
