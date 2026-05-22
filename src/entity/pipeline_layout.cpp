#include <kochou/context/view.hpp>
#include <kochou/entity/pipeline_layout.hpp>

ktl::errc
kochou::entity::pipeline_layout::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::pipeline_layout::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::pipeline_layout::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_pipeline_layout, ktl::errc >
kochou::entity::pipeline_layout::make(kochou::shared_context                                _sctx,
                                      std::span< kochou::entity::shared_descriptor_layout > _layouts) noexcept
{
    auto device = kochou::view::device(_sctx);

    std::vector< ktl::api::descriptor_set_layout > raw_layouts(_layouts.size());
    for (ktl::usize i = 0; i < _layouts.size(); ++i)
    {
        raw_layouts[i] = _layouts[i]->raw;
    }
    ktl::api::pipeline_layout_create_info info = {.set_layout_count = static_cast< ktl::u32 >(raw_layouts.size()),
                                                  .p_set_layouts    = raw_layouts.data(),
                                                  .push_constant_range_count = 0,
                                                  .p_push_constant_ranges    = nullptr};

    ktl::api::pipeline_layout raw_layout = nullptr;
    ktl::api::result          rc         = ktl::api::create_pipeline_layout(device, &info, nullptr, &raw_layout);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_layout_rc = ktl::memory::make_shared< pipeline_layout >(std::move(_sctx), _layouts, raw_layout, true);
    if (!shared_layout_rc.has_value())
    {
        return ktl::err(shared_layout_rc.error());
    }

    return shared_layout_rc.take_value();
}

kochou::entity::pipeline_layout::pipeline_layout(kochou::shared_context                                _sctx,
                                                 std::span< kochou::entity::shared_descriptor_layout > _layouts,
                                                 ktl::api::pipeline_layout _layout, bool _is_need_destroy)
    : raw(_layout), is_need_destroy(_is_need_destroy), sctx_(_sctx), layouts_(_layouts.begin(), _layouts.end())
{
}

kochou::entity::pipeline_layout::pipeline_layout(pipeline_layout && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), layouts_(std::exchange(_rhs.layouts_, {}))
{
}

kochou::entity::pipeline_layout &
kochou::entity::pipeline_layout::operator=(pipeline_layout && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);
    layouts_        = std::exchange(_rhs.layouts_, {});

    return *this;
}

kochou::entity::pipeline_layout::~pipeline_layout()
{
    clean();
}

void
kochou::entity::pipeline_layout::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_pipeline_layout(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_    = nullptr;
    layouts_ = {};
}
