#include <ktl/api.hpp>

#include <kochou/entity/surface.hpp>

#define KOCHOU_WINDOW_BACKEND_METAL

namespace
{
#ifdef KOCHOU_WINDOW_BACKEND_METAL

ktl::api::surface_khr
create_surface() noexcept
{
}

void
destroy_surface() noexcept
{
}

#elifdef KOCHOU_WINDOW_BACKEND_WAYLAND

ktl::api::surface_khr
create_surface() noexcept
{
}

void
destroy_surface() noexcept
{
}

#elifdef KOCHOU_WINDOW_BACKEND_WIN32

ktl::api::surface_khr
create_surface() noexcept
{
}

void
destroy_surface() noexcept
{
}

#elifdef KOCHOU_WINDOW_BACKEND_XCB

ktl::api::surface_khr
create_surface() noexcept
{
}

void
destroy_surface() noexcept
{
}

#elifdef KOCHOU_WINDOW_BACKEND_XLIB

ktl::api::surface_khr
create_surface() noexcept
{
}

void
destroy_surface() noexcept
{
}

#else
static_assert(false && "window backend unsupported");
#endif
} // namespace

kochou::entity::surface::surface(kochou::shared_context _sctx) : sctx_(_sctx) {}
