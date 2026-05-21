#ifndef KOCHOU_ENTITY_COMMAND_BUFFER_HPP
#define KOCHOU_ENTITY_COMMAND_BUFFER_HPP

#include <kochou/context/context.hpp>
#include <kochou/entity/command_pool.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class command_buffer
{
    // requirements
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;

public:
    static ktl::result< command_buffer, ktl::errc >
    make(kochou::shared_context _sctx, kochou::entity::shared_command_pool _shared_command_pool,
         ktl::api::command_buffer_level = ktl::api::command_buffer_level::v_primary) noexcept;

    // common
public:
    command_buffer(kochou::shared_context _sctx, ktl::api::command_buffer _command_buffer,
                   bool is_need_destroy) noexcept;
    command_buffer(const command_buffer &) noexcept = delete;
    command_buffer(command_buffer &&) noexcept;
    command_buffer &
    operator=(const command_buffer &) noexcept = delete;
    command_buffer &
    operator=(command_buffer &&) noexcept;
    ~command_buffer() noexcept;

    // methods
public:
    ktl::errc
    begin(ktl::api::command_buffer_usage_flags _flags = static_cast< ktl::api::command_buffer_usage_flags >(
              ktl::api::command_buffer_usage_flag_bits::v_one_time_submit_bit)) noexcept;
    ktl::errc
    end() noexcept;
    ktl::errc
    reset(ktl::api::command_buffer_reset_flags _flags = 0) noexcept;

    // raw
public:
    ktl::api::command_buffer raw;
    bool                     is_need_destroy;

    // shared raii
private:
    kochou::shared_context sctx_;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::command_buffer >);

#endif
