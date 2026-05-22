#ifndef KOCHOU_ENTITY_PIPELINE_HPP
#define KOCHOU_ENTITY_PIPELINE_HPP

#include <ktl/api.hpp>
#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/pipeline_layout.hpp>
#include <kochou/entity/shader_module.hpp>

namespace kochou::entity
{
class pipeline;
using shared_pipeline = ktl::memory::sptr< pipeline >;

class pipeline
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
    static ktl::result< shared_pipeline, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_pipeline_layout _layout,
         std::span< kochou::entity::shared_shader_module > _shaders,
         ktl::api::graphics_pipeline_create_info           _info) noexcept;

    // common
public:
    pipeline(kochou::shared_context _sctx, kochou::entity::shared_pipeline_layout _layout,
             std::span< kochou::entity::shared_shader_module > _shaders, ktl::api::pipeline _pipeline,
             bool _is_need_destroy) noexcept;
    pipeline(const pipeline &) noexcept = delete;
    pipeline(pipeline &&) noexcept;
    pipeline &
    operator=(const pipeline &) noexcept = delete;
    pipeline &
    operator=(pipeline &&) noexcept;
    ~pipeline() noexcept;

    // raw
public:
    ktl::api::pipeline raw;
    bool               is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context                              sctx_;
    kochou::entity::shared_pipeline_layout              layout_;
    std::vector< kochou::entity::shared_shader_module > shaders_;
};
} // namespace kochou::entity

#endif
