#ifndef KOCHOU_ENTITY_FENCE_HPP
#define KOCHOU_ENTITY_FENCE_HPP

#include <ktl/api.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class fence
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
    static ktl::result< fence, ktl::errc >
    make(kochou::shared_context _sctx, ktl::api::fence_create_flags _bits = static_cast< ktl::api::fence_create_flags >(
                                           ktl::api::fence_create_flag_bits::v_signaled_bit)) noexcept;

    // common
public:
    fence(kochou::shared_context _sctx, ktl::api::fence _fence) noexcept;
    fence(const fence &) = delete;
    fence(fence &&)      = default;
    fence &
    operator=(const fence &) = delete;
    fence &
    operator=(fence &&) = default;
    ~fence() noexcept;

    // methods
public:
    ktl::errc
    wait(ktl::u64 _timeout) noexcept;
    ktl::errc
    reset() noexcept;

    // raw
public:
    ktl::api::fence raw;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::fence >);

#endif
