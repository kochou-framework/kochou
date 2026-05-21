#ifndef KOCHOU_ENTITY_SEMAPHORE_HPP
#define KOCHOU_ENTITY_SEMAPHORE_HPP

#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class semaphore;
using shared_semaphore = ktl::memory::sptr< semaphore >;

class semaphore
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
    static ktl::result< shared_semaphore, ktl::errc >
    make(kochou::shared_context _sctx) noexcept;

    // common
public:
    semaphore(kochou::shared_context _sctx, ktl::api::semaphore _semaphore, bool _is_need_destroy) noexcept;
    semaphore(const semaphore &) noexcept = delete;
    semaphore(semaphore &&) noexcept;
    semaphore &
    operator=(const semaphore &) noexcept = delete;
    semaphore &
    operator=(semaphore &&) noexcept;
    ~semaphore() noexcept;

    // raw
public:
    ktl::api::semaphore raw;
    bool                is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::semaphore >);

#endif
