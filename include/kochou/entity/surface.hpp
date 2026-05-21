#ifndef KOCHOU_ENTITY_SURFACE_HPP
#define KOCHOU_ENTITY_SURFACE_HPP

#include <ktl/api.hpp>
#include <ktl/memory.hpp>

#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class surface;
using shared_surface = ktl::memory::sptr< surface >;

class surface
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
    static ktl::result< shared_surface, ktl::errc >
    make(kochou::shared_context _sctx, ktl::u32 _width, ktl::u32 _height) noexcept;
    static ktl::result< shared_surface, ktl::errc >
    make_from(kochou::shared_context _sctx, ktl::api::surface_khr _surface, ktl::u32 _width, ktl::u32 _height) noexcept;

    // common
public:
    surface(kochou::shared_context _sctx, ktl::api::surface_khr _surface, ktl::u32 _width, ktl::u32 _height,
            bool _is_need_destroy) noexcept;
    surface(const surface &) noexcept = delete;
    surface(surface &&) noexcept;
    surface &
    operator=(const surface &) noexcept = delete;
    surface &
    operator=(surface &&) noexcept;
    ~surface() noexcept;

    // methods
public:
    ktl::u32
    width() const noexcept;
    ktl::u32
    height() const noexcept;

private:
    ktl::u32 width_;
    ktl::u32 height_;

    // raw
public:
    ktl::api::surface_khr raw;
    bool                  is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::surface >);

#endif
