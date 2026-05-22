#ifndef KOCHOU_ENTITY_PIPELINE_LAYOUT_HPP
#define KOCHOU_ENTITY_PIPELINE_LAYOUT_HPP

#include <span>

#include <ktl/api.hpp>
#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/descriptor_layout.hpp>

namespace kochou::entity
{
class pipeline_layout;
using shared_pipeline_layout = ktl::memory::sptr< pipeline_layout >;

class pipeline_layout
{
    // interface
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;
    // fabrics
public:
    static ktl::result< shared_pipeline_layout, ktl::errc >
    make(kochou::shared_context _sctx, std::span< kochou::entity::shared_descriptor_layout > _layouts) noexcept;

    // common
public:
    pipeline_layout(kochou::shared_context _sctx, std::span< kochou::entity::shared_descriptor_layout > _layouts,
                    ktl::api::pipeline_layout _layout, bool _is_need_destroy);
    pipeline_layout(const pipeline_layout &) noexcept = delete;
    pipeline_layout(pipeline_layout &&) noexcept;
    pipeline_layout &
    operator=(const pipeline_layout &) noexcept = delete;
    pipeline_layout &
    operator=(pipeline_layout &&) noexcept;
    ~pipeline_layout();

    // raw
public:
    ktl::api::pipeline_layout raw;
    bool                      is_need_destroy;

private:
    void
    clean() noexcept;

private:
    // shared raii
private:
    kochou::shared_context                                  sctx_;
    std::vector< kochou::entity::shared_descriptor_layout > layouts_;
};
} // namespace kochou::entity

#endif
