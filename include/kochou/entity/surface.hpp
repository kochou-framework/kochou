#ifndef KOCHOU_ENTITY_SURFACE_HPP
#define KOCHOU_ENTITY_SURFACE_HPP

#include <atomic>

#include <ktl/api.hpp>
#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
struct surface
{
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;

    ktl::api::surface_khr   raw; // readonly after create
    std::atomic< ktl::u64 > size;

    inline static ktl::u32
    width(ktl::u64 _size)
    {
        return static_cast< ktl::u32 >(_size & 0xFFFFFFFF);
    }

    inline static ktl::u32
    height(ktl::u64 _size)
    {
        return static_cast< ktl::u32 >(_size >> 32);
    }
};

using shared_surface = ktl::memory::sptr< surface >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::surface >);

#endif
