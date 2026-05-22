#ifndef KOCHOU_ENTITY_DESCRIPTOR_LAYOUT_HPP
#define KOCHOU_ENTITY_DESCRIPTOR_LAYOUT_HPP

#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>

namespace kochou::entity
{
class descriptor_layout;
using shared_descriptor_layout = ktl::memory::sptr< descriptor_layout >;

class descriptor_layout
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
    static ktl::result< shared_descriptor_layout, ktl::errc >
    make(kochou::shared_context _sctx, std::span< ktl::api::descriptor_set_layout_binding > _bindings) noexcept;

    // common
public:
    descriptor_layout(kochou::shared_context _sctx, ktl::api::descriptor_set_layout _descriptor_layout,
                      bool _is_need_destroy) noexcept;
    descriptor_layout(const descriptor_layout &) noexcept = delete;
    descriptor_layout(descriptor_layout &&) noexcept;
    descriptor_layout &
    operator=(const descriptor_layout &) noexcept = delete;
    descriptor_layout &
    operator=(descriptor_layout &&) noexcept;
    ~descriptor_layout() noexcept;

    // raw
public:
    ktl::api::descriptor_set_layout raw;
    bool                            is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

#endif
