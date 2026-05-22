#include <kochou/context/view.hpp>
#include <kochou/entity/shader_module.hpp>

ktl::errc
kochou::entity::shader_module::ensure(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

ktl::errc
kochou::entity::shader_module::should(kochou::shared_context _sctx) noexcept
{
    return ktl::errc::success;
}

bool
kochou::entity::shader_module::allowed(kochou::shared_context _sctx) noexcept
{
    return true;
}

ktl::result< kochou::entity::shared_shader_module, ktl::errc >
kochou::entity::shader_module::make(kochou::shared_context _sctx, std::span< ktl::u32 > _spv) noexcept
{
    auto device = kochou::view::device(_sctx);

    ktl::api::shader_module_create_info info;
    info.flags     = 0;
    info.code_size = _spv.size();
    info.p_code    = _spv.data();

    ktl::api::shader_module raw_shader_module = nullptr;
    ktl::api::result        rc = ktl::api::create_shader_module(device, &info, nullptr, &raw_shader_module);
    if (rc != ktl::api::result::v_success)
    {
        return ktl::err(ktl::cast_api_result(rc));
    }

    auto shared_shader_module_rc = ktl::memory::make_shared< shader_module >(std::move(_sctx), raw_shader_module, true);
    if (!shared_shader_module_rc.has_value())
    {
        return ktl::err(shared_shader_module_rc.error());
    }

    return shared_shader_module_rc.take_value();
}

kochou::entity::shader_module::shader_module(kochou::shared_context _sctx, ktl::api::shader_module _shader_module,
                                             bool _is_need_destroy) noexcept
    : raw(_shader_module), is_need_destroy(_is_need_destroy), sctx_(std::move(_sctx))
{
}

kochou::entity::shader_module::shader_module(shader_module && _rhs) noexcept
    : raw(std::exchange(_rhs.raw, nullptr)), is_need_destroy(std::exchange(_rhs.is_need_destroy, false)),
      sctx_(std::exchange(_rhs.sctx_, nullptr))
{
}

kochou::entity::shader_module &
kochou::entity::shader_module::operator=(shader_module && _rhs) noexcept
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

kochou::entity::shader_module::~shader_module() noexcept
{
    clean();
}

void
kochou::entity::shader_module::clean() noexcept
{
    if (raw && is_need_destroy && sctx_)
    {
        auto device = kochou::view::device(sctx_);
        ktl::api::destroy_shader_module(device, raw, nullptr);
        raw             = nullptr;
        is_need_destroy = false;
    }
    sctx_ = nullptr;
}
