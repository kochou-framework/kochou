#include <ktl/api.hpp>

#include <kochou/context/view.hpp>
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

ktl::result< kochou::entity::shared_surface, ktl::errc >
kochou::entity::surface::make(kochou::shared_context _sctx, ktl::u32 _width, ktl::u32 _height) noexcept
{
    // TODO ?
    return ktl::err(ktl::errc::incomplete);
}

ktl::result< kochou::entity::shared_surface, ktl::errc >
kochou::entity::surface::make_from(kochou::shared_context _sctx, ktl::api::surface_khr _surface, ktl::u32 _width,
                                   ktl::u32 _height) noexcept
{
    auto shared_surface_rc = ktl::memory::make_shared< surface >(_sctx, _surface, _width, _height, true);
    if (!shared_surface_rc.has_value())
    {
        return ktl::err(shared_surface_rc.error());
    }

    return shared_surface_rc.take_value();
}

kochou::entity::surface::surface(kochou::shared_context _sctx, ktl::api::surface_khr _surface, ktl::u32 _width,
                                 ktl::u32 _height, bool _is_need_destroy) noexcept
    : width_(_width), height_(_height), raw(_surface), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx))
{
}

kochou::entity::surface::surface(surface && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::surface &
kochou::entity::surface::operator=(surface && _rhs) noexcept
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

kochou::entity::surface::~surface() noexcept
{
    kochou::log::debug("~surface");
    clean();
}

ktl::u32
kochou::entity::surface::width() const noexcept
{
    return width_;
}

ktl::u32
kochou::entity::surface::height() const noexcept
{
    return height_;
}

void
kochou::entity::surface::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto instance = kochou::view::instance(sctx_);
        ktl::api::destroy_surface_khr(instance, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
