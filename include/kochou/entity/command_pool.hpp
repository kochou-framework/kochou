#ifndef KOCHOU_ENTITY_COMMAND_POOL_HPP
#define KOCHOU_ENTITY_COMMAND_POOL_HPP

#include <ktl/errc.hpp>
#include <ktl/result.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class command_pool
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
    static ktl::result< command_pool, ktl::errc >
    make(kochou::shared_context _sctx) noexcept;

    // common
public:
    command_pool(kochou::shared_context _sctx, ktl::api::command_pool _command_pool, bool _is_need_destroy) noexcept;
    command_pool(const command_pool &) noexcept = delete;
    command_pool(command_pool &&) noexcept      = default;
    command_pool &
    operator=(const command_pool &) noexcept = delete;
    command_pool &
    operator=(command_pool &&) noexcept = default;
    ~command_pool() noexcept;

    // raw
public:
    ktl::api::command_pool raw;
    // shared raii
private:
    kochou::shared_context sctx_;
    bool                   is_need_destroy_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::command_pool >);

#endif
