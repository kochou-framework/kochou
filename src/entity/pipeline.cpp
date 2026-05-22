#include <kochou/context/view.hpp>
#include <kochou/entity/pipeline.hpp>

ktl::errc
kochou::entity::pipeline::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::pipeline::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::pipeline::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_pipeline, ktl::errc >
kochou::entity::pipeline::make(kochou::shared_context _sctx, kochou::entity::shared_pipeline_layout _layout,
                               std::span< kochou::entity::shared_shader_module > _shaders,
                               ktl::api::graphics_pipeline_create_info           _info) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::pipeline raw_pipeline = nullptr;
    ktl::api::result   rc = ktl::api::create_graphics_pipelines(device, nullptr, 1, &_info, nullptr, &raw_pipeline);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_pipeline_rc =
        ktl::memory::make_shared< pipeline >(std::move(_sctx), std::move(_layout), _shaders, raw_pipeline, true);
    if (!shared_pipeline_rc.has_value())
    {
        return ktl::err(shared_pipeline_rc.error());
    }

    return shared_pipeline_rc.take_value();
}

kochou::entity::pipeline::pipeline(kochou::shared_context _sctx, kochou::entity::shared_pipeline_layout _layout,
                                   std::span< kochou::entity::shared_shader_module > _shaders,
                                   ktl::api::pipeline _pipeline, bool _is_need_destroy) noexcept
    : raw(_pipeline), is_need_destroy(_is_need_destroy), sctx_(_sctx), layout_(_layout),
      shaders_(_shaders.begin(), _shaders.end())
{
}

kochou::entity::pipeline::pipeline(pipeline && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr)), layout_(std::exchange(_rhs.layout_, nullptr)),
      shaders_(std::exchange(_rhs.shaders_, {}))
{
}

kochou::entity::pipeline &
kochou::entity::pipeline::operator=(pipeline && _rhs) noexcept
{
    if (std::addressof(_rhs) == this)
    {
        return *this;
    }

    clean();

    raw             = std::exchange(_rhs.raw, nullptr);
    is_need_destroy = std::exchange(_rhs.is_need_destroy, false);
    sctx_           = std::exchange(_rhs.sctx_, nullptr);
    layout_         = std::exchange(_rhs.layout_, nullptr);
    shaders_        = std::exchange(_rhs.shaders_, {});

    return *this;
}

kochou::entity::pipeline::~pipeline() noexcept
{
    clean();
}

void
kochou::entity::pipeline::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_pipeline(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_    = nullptr;
    layout_  = nullptr;
    shaders_ = {};
}
