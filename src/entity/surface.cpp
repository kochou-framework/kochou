#include <ktl/api.hpp>

#include <kochou/entity/surface.hpp>
#include <kochou/requirements/allowed.hpp>
#include <kochou/requirements/ensure.hpp>
#include <kochou/requirements/extension.hpp>
#include <kochou/requirements/should.hpp>

ktl::errc
kochou::entity::surface::ensure(kochou::shared_context _sctx) noexcept
{
    return kochou::ensure< kochou::extension< ktl::api::extension::khr_surface > >(_sctx);
}

ktl::errc
kochou::entity::surface::should(kochou::shared_context _sctx) noexcept
{
    return kochou::should< kochou::extension< ktl::api::extension::khr_surface > >(_sctx);
}

bool
kochou::entity::surface::allowed(kochou::shared_context _sctx) noexcept
{
    return kochou::allowed< kochou::extension< ktl::api::extension::khr_surface > >(_sctx);
}

kochou::entity::surface::surface(ktl::api::surface_khr _surface, ktl::u32 _width, ktl::u32 _height) noexcept
    : surface_(_surface), size_(static_cast< ktl::u64 >(_height) << 32 | static_cast< ktl::u64 >(_width))
{
}

kochou::entity::surface::~surface() noexcept
{ // surface must be destroyed by source
}
