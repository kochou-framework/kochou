#ifndef KOCHOU_ENTITY_DESCRIPTOR_POOL_HPP
#define KOCHOU_ENTITY_DESCRIPTOR_POOL_HPP

#include <ktl/api.hpp>
#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>

namespace kochou::entity
{
class descriptor_pool;
using shared_descriptor_pool = ktl::memory::sptr< descriptor_pool >;

class descriptor_pool
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
    static ktl::result< shared_descriptor_pool, ktl::errc >
    make(kochou::shared_context _sctx, ktl::u32 max_sets, std::span< ktl::api::descriptor_pool_size > _sets) noexcept;

    // common
public:
    descriptor_pool(kochou::shared_context _sctx, ktl::api::descriptor_pool _descriptor_pool,
                    bool _is_need_destroy) noexcept;
    descriptor_pool(const descriptor_pool &) noexcept = delete;
    descriptor_pool(descriptor_pool &&) noexcept;
    descriptor_pool &
    operator=(const descriptor_pool &) noexcept = delete;
    descriptor_pool &
    operator=(descriptor_pool &&) noexcept;
    ~descriptor_pool() noexcept;

    // raw
public:
    ktl::api::descriptor_pool raw;
    bool                      is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

#endif
