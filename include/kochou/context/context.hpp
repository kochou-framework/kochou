#ifndef KOCHOU_CONTEXT_CONTEXT_HPP
#define KOCHOU_CONTEXT_CONTEXT_HPP

#include <ktl/api.hpp>
#include <ktl/loader.hpp>
#include <ktl/meta.hpp>

#include <kochou/base/device.hpp>
#include <kochou/base/instance.hpp>
#include <kochou/base/physical_device.hpp>
#include <kochou/context/resolver.hpp>
#include <kochou/log.hpp>

namespace kochou
{
struct view;
class configure;

class context final : public kochou::external< kochou::hold::shared, context >, public resolver
{
    friend struct kochou::view;
    friend class kochou::configure;

public:
    context(ktl::errc & _errc) noexcept;
    ~context() noexcept;

    ktl::errc
    status() const noexcept;

private:
    ktl::errc                                    status_;
    ktl::memory::sptr< kochou::instance >        instance_;
    ktl::memory::sptr< kochou::physical_device > physical_device_;
    ktl::memory::sptr< kochou::device >          device_;
    ktl::loader::handle_type                     handle_;
    ktl::api::pfn_table                          ptable_;
};

using unique_context = ktl::memory::uptr< kochou::context >;
using shared_context = ktl::memory::sptr< kochou::context >;
} // namespace kochou

#endif
