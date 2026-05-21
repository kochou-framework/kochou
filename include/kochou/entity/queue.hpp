#ifndef KOCHOU_ENTITY_QUEUE_HPP
#define KOCHOU_ENTITY_QUEUE_HPP

#include <kochou/entity/fence.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class queue;
using shared_queue = ktl::memory::sptr< queue >;

class queue
{
public:
    enum queue_type
    {
        present,
        graphics,
        compute,
        transfer
    };
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
    static ktl::result< shared_queue, ktl::errc >
    make(kochou::shared_context _sctx, queue_type _type) noexcept;

    // common
public:
    queue(kochou::shared_context _sctx, ktl::api::queue _queue, bool _is_need_destroy) noexcept;
    queue(const queue &) noexcept = delete;
    queue(queue &&) noexcept;
    queue &
    operator=(const queue &) noexcept = delete;
    queue &
    operator=(queue &&) noexcept;
    ~queue() noexcept;

    // methods
public:
    ktl::errc
    submit(const ktl::api::submit_info & info, kochou::entity::shared_fence _fence) noexcept;

    // raw
public:
    ktl::api::queue raw;
    bool            is_need_destroy;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::queue >);

#endif
