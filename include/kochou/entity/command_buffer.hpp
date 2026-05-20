#ifndef KOCHOU_ENTITY_COMMAND_BUFFER_HPP
#define KOCHOU_ENTITY_COMMAND_BUFFER_HPP

#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class command_buffer
{
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;
    static bool
    allowed(kochou::shared_context _sctx) noexcept;
};
} // namespace kochou::entity

static_assert(kochou::entity::is_valid_entity< kochou::entity::command_buffer >);

#endif
