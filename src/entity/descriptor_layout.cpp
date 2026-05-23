#include <kochou/context/view.hpp>
#include <kochou/entity/descriptor_layout.hpp>

ktl::errc
kochou::entity::descriptor_layout::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::descriptor_layout::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::descriptor_layout::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_descriptor_layout, ktl::errc >
kochou::entity::descriptor_layout::make(kochou::shared_context                               _sctx,
                                        std::span< ktl::api::descriptor_set_layout_binding > _bindings) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::descriptor_set_layout_create_info info;
    info.flags         = 0;
    info.binding_count = _bindings.size();
    info.p_bindings    = _bindings.data();

    ktl::api::descriptor_set_layout raw_layout = nullptr;
    ktl::api::result                rc = ktl::api::create_descriptor_set_layout(device, &info, nullptr, &raw_layout);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto layout_rc = ktl::memory::make_shared< descriptor_layout >(std::move(_sctx), raw_layout, true);
    if (!layout_rc.has_value())
    {
        return ktl::err(layout_rc.error());
    }

    return layout_rc.take_value();
}

kochou::entity::descriptor_layout::descriptor_layout(kochou::shared_context          _sctx,
                                                     ktl::api::descriptor_set_layout _descriptor_layout,
                                                     bool                            _is_need_destroy) noexcept
    : raw(_descriptor_layout), is_need_destroy(_is_need_destroy), sctx_(_sctx)
{
}

kochou::entity::descriptor_layout::descriptor_layout(descriptor_layout && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::descriptor_layout &
kochou::entity::descriptor_layout::operator=(descriptor_layout && _rhs) noexcept
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

kochou::entity::descriptor_layout::~descriptor_layout() noexcept
{
    kochou::log::debug("~descriptor_layout");
    clean();
}

void
kochou::entity::descriptor_layout::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_descriptor_set_layout(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
