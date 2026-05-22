#ifndef KOCHOU_ENTITY_DESCRIPTOR_SET_HPP
#define KOCHOU_ENTITY_DESCRIPTOR_SET_HPP

#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/descriptor_pool.hpp>

namespace kochou::entity
{
class descriptor_set;
using shared_descriptor_set = ktl::memory::sptr< descriptor_set >;

class descriptor_set
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
    static ktl::result< shared_descriptor_set, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_descriptor_pool _descriptor_pool,
         const ktl::api::descriptor_set_layout & _layout) noexcept;

    // common
public:
    descriptor_set(kochou::shared_context _sctx, kochou::entity::shared_descriptor_pool _descriptor_pool,
                   ktl::api::descriptor_set _descriptor_set, bool _is_need_destroy) noexcept;
    descriptor_set(const descriptor_set &) noexcept = delete;
    descriptor_set(descriptor_set &&) noexcept;
    descriptor_set &
    operator=(const descriptor_set &) noexcept = delete;
    descriptor_set &
    operator=(descriptor_set &&) noexcept;
    ~descriptor_set() noexcept;

    // raw
public:
    ktl::api::descriptor_set raw;
    bool                     is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context                 sctx_;
    kochou::entity::shared_descriptor_pool descriptor_pool_;
};
} // namespace kochou::entity

#endif
