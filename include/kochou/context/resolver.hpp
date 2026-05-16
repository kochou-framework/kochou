#ifndef KOCHOU_CONTEXT_RESOLVER_HPP
#define KOCHOU_CONTEXT_RESOLVER_HPP

#include <set>
#include <unordered_map>

#include <ktl/api.hpp>
#include <ktl/api/version.hpp>
#include <ktl/errc.hpp>
#include <ktl/memory.hpp>
#include <ktl/meta.hpp>
#include <ktl/result.hpp>

#include <kochou/base/device.hpp>
#include <kochou/base/instance.hpp>
#include <kochou/base/physical_device.hpp>

namespace kochou
{
class resolver
{
public:
    static constexpr ktl::usize cmdarray_size = 128;
    using cmdarray                            = std::array< ktl::api::command, cmdarray_size >;
    using dependency                          = std::tuple< ktl::api::version, ktl::meta::dependency,
                                                            std::array< ktl::api::command, cmdarray_size >, ktl::usize >;

public:
    resolver() noexcept;
    ~resolver() noexcept;

    ktl::errc
    ensure(kochou::resolver::dependency _dependency) noexcept;
    ktl::errc
    should(kochou::resolver::dependency _dependency) noexcept;
    bool
    allowed(ktl::meta::dependency _dependency) noexcept;

    ktl::errc
    ensure(ktl::api::queue_flag_bits _bit) noexcept;
    ktl::errc
    should(ktl::api::queue_flag_bits _bit) noexcept;
    bool
    allowed(ktl::api::queue_flag_bits _bit) noexcept;

    using shared_instance        = ktl::memory::sptr< kochou::instance >;
    using shared_physical_device = ktl::memory::sptr< kochou::physical_device >;
    using shared_device          = ktl::memory::sptr< kochou::device >;
    using vulkan_base_type       = std::tuple< shared_instance, shared_physical_device, shared_device >;
    using resolve_result         = ktl::result< vulkan_base_type, ktl::errc >;
    resolve_result
    resolve() noexcept;

private:
    // RANGES
    // extension < instance/device > filter
    // dependency -> extension
    // feature filter
    // dependency -> feature
    // version filter
    // dependency -> version

private:
    ktl::api::queue_flags                                                      ensure_queue_;
    ktl::api::queue_flags                                                      should_queue_;
    ktl::api::queue_flags                                                      allowed_queue_;
    std::unordered_map< ktl::meta::dependency, std::set< ktl::api::command > > commands_;
    std::unordered_map< ktl::api::version, std::set< ktl::meta::dependency > > ensure_;
    std::unordered_map< ktl::api::version, std::set< ktl::meta::dependency > > should_;
    std::pair< ktl::api::version, std::set< ktl::meta::dependency > >          allowed_;
};
} // namespace kochou

#endif
