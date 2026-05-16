#ifndef KOCHOU_REQUIREMENTS_QUEUE_HPP
#define KOCHOU_REQUIREMENTS_QUEUE_HPP

#include <ktl/errc.hpp>

#include <kochou/context/context.hpp>

namespace
{
// TODO support all
constexpr bool
is_supported_bit(ktl::api::queue_flag_bits _bit)
{
    switch (_bit)
    {
    case ktl::api::queue_flag_bits::v_graphics_bit:
        return true;
    case ktl::api::queue_flag_bits::v_compute_bit:
        return true;
    case ktl::api::queue_flag_bits::v_transfer_bit:
        return true;
    default:
        break;
    }
    return false;
}
} // namespace

// TODO
// сейчас при создании устройства задается только одно семейство очередей,
// которое поддерживает все нужны флаги, если такое найдено
// нужно создавать устройство с несколькими семействами по необходимости или по требованию
namespace kochou
{
template < ktl::api::queue_flag_bits QUEUE_BIT >
struct queue final
{
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou

template < ktl::api::queue_flag_bits QUEUE_BIT >
ktl::errc
kochou::queue< QUEUE_BIT >::ensure(kochou::shared_context _sctx) noexcept
{
    if constexpr (is_supported_bit(QUEUE_BIT))
    {
        return _sctx->ensure(QUEUE_BIT);
    }
    static_assert(false && "unsupported now");
}

template < ktl::api::queue_flag_bits QUEUE_BIT >
ktl::errc
kochou::queue< QUEUE_BIT >::should(kochou::shared_context _sctx) noexcept
{
    if constexpr (is_supported_bit(QUEUE_BIT))
    {
        return _sctx->ensure(QUEUE_BIT);
    }
    static_assert(false && "unsupported now");
}

template < ktl::api::queue_flag_bits QUEUE_BIT >
bool
kochou::queue< QUEUE_BIT >::allowed(kochou::shared_context _sctx) noexcept
{
    if constexpr (is_supported_bit(QUEUE_BIT))
    {
        return _sctx->allowed(QUEUE_BIT);
    }
    return false;
}

#endif
