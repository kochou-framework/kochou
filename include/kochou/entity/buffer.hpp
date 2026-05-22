#ifndef KOCHOU_ENTITY_BUFFER_HPP
#define KOCHOU_ENTITY_BUFFER_HPP

#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/device_memory.hpp>

namespace kochou::entity
{
class buffer;
using shared_buffer = ktl::memory::sptr< buffer >;

class buffer
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
    static ktl::result< shared_buffer, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_device_memory _device_memory, ktl::api::dvsize _dvsize,
         ktl::api::buffer_usage_flags _flags, ktl::api::sharing_mode _sharing) noexcept;

    // common
public:
    buffer(kochou::shared_context _sctx, kochou::entity::shared_device_memory _device_memory, ktl::api::buffer _buffer,
           bool _is_need_destroy) noexcept;
    buffer(const buffer &) noexcept = delete;
    buffer(buffer &&) noexcept;
    buffer &
    operator=(const buffer &) noexcept = delete;
    buffer &
    operator=(buffer &&) noexcept;
    ~buffer() noexcept;

    // raw
public:
    ktl::api::buffer raw;
    bool             is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context               sctx_;
    kochou::entity::shared_device_memory device_memory_;
};
} // namespace kochou::entity

#endif
