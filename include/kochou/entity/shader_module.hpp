#ifndef KOCHOU_ENTITY_SHADER_MODULE_HPP
#define KOCHOU_ENTITY_SHADER_MODULE_HPP

#include <span>

#include <ktl/memory.hpp>

#include <kochou/context/context.hpp>
#include <kochou/entity/interface.hpp>

namespace kochou::entity
{
class shader_module;
using shared_shader_module = ktl::memory::sptr< shader_module >;

class shader_module
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
    static ktl::result< shared_shader_module, ktl::errc >
    make(kochou::shared_context _sctx, std::span< const ktl::u32 > _spv) noexcept;

    // common
public:
    shader_module(kochou::shared_context _sctx, ktl::api::shader_module _shader_module, bool _is_need_destroy) noexcept;
    shader_module(const shader_module &) noexcept = delete;
    shader_module(shader_module &&) noexcept;
    shader_module &
    operator=(const shader_module &) noexcept = delete;
    shader_module &
    operator=(shader_module &&) noexcept;
    ~shader_module() noexcept;

    // raw
public:
    ktl::api::shader_module raw;
    bool                    is_need_destroy;

private:
    void
    clean() noexcept;

    // shared raii
private:
    kochou::shared_context sctx_;
};

} // namespace kochou::entity

#endif
