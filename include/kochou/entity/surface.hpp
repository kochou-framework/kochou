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

public:
    surface(ktl::api::surface_khr _surface, ktl::u32 _width, ktl::u32 _height) noexcept;
    surface(const surface &) noexcept = delete;
    surface(surface &&) noexcept      = default;
    surface &
    operator=(const surface &) = delete;
    surface &
    operator=(surface &&) = default;
    ~surface() noexcept;

public:
    inline ktl::api::surface_khr
    raw() const noexcept
    {
        return surface_;
    }

    inline ktl::u32
    width() const noexcept
    {
        return static_cast< ktl::u32 >(size_ & 0xFFFFFFFF);
    }

    inline ktl::u32
    height() const noexcept
    {
        return static_cast< ktl::u32 >(size_ >> 32);
    }

private:
    ktl::api::surface_khr   surface_; // readonly after create
    std::atomic< ktl::u64 > size_;
};

using shared_surface = ktl::memory::sptr< surface >;
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::surface >);

#endif
