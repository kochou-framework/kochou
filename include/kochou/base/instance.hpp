#ifndef KOCHOU_BASE_INSTANCE_HPP
#define KOCHOU_BASE_INSTANCE_HPP

#include <ranges>

#include <ktl/api.hpp>
#include <ktl/api/version.hpp>
#include <ktl/meta.hpp>
#include <ktl/system.hpp>

#include <kochou/external.hpp>
#include <kochou/log.hpp>

namespace kochou
{
struct requirements;
struct view;
class resolver;

class instance : public kochou::external< kochou::hold::shared, instance >
{
private:
    friend struct kochou::view;
    friend class kochou::resolver;

public: // TODO make private
    template < std::ranges::input_range EXTENSION_RANGE >
        requires std::same_as< std::ranges::range_value_t< EXTENSION_RANGE >, ktl::api::extension >
    instance(ktl::errc & _errc, ktl::api::version _version, EXTENSION_RANGE && _extensions) noexcept : handle_(nullptr)
    {
        ktl::api::application_info application_info = {.p_application_name  = "kochou",
                                                       .application_version = 1,
                                                       .p_engine_name       = "kochou-framework",
                                                       .engine_version      = 1,
                                                       .api_version         = _version()};

        auto raw_cast = [](ktl::api::extension ext)
        {
            return ktl::meta::extension_cast(ext).raw_name.data();
        };
        auto view = std::forward< EXTENSION_RANGE >(_extensions) | std::views::transform(raw_cast);
        std::vector< const char * > extensions_vec(std::ranges::begin(view), std::ranges::end(view));
        for (const auto & raw_name : extensions_vec)
        {
            kochou::log::debug("instance enable {}", raw_name);
        }

        ktl::u32 lc = 0;
        ktl::api::enumerate_instance_layer_properties(&lc, nullptr);
        kochou::log::info("LAYERS COUNT = {}", lc);
        std::vector< ktl::api::layer_properties > lay(lc);
        ktl::api::enumerate_instance_layer_properties(&lc, lay.data());
        for (const auto & l : lay)
        {
            kochou::log::debug("name={}", l.layer_name);
        }

        std::vector< const char * >    layers        = {"VK_LAYER_KHRONOS_validation"};
        ktl::api::instance_create_info instance_info = {
            .flags                      = (ktl::os_defined == ktl::os_flag::macos)
                                              ? static_cast< ktl::u32 >(ktl::api::instance_create_flag_bits::v_enumerate_portability_bit_khr)
                                              : 0,
            .p_application_info         = &application_info,
            .enabled_layer_count        = 1,
            .pp_enabled_layer_names     = layers.data(),
            .enabled_extension_count    = static_cast< ktl::u32 >(extensions_vec.size()),
            .pp_enabled_extension_names = extensions_vec.data()};

        ktl::api::result rc = ktl::api::create_instance(&instance_info, nullptr, &handle_);
        if (rc != ktl::api::result::v_success)
        {
            kochou::log::error("create_instance failed, rc={}", static_cast< ktl::i32 >(rc));
            _errc = ktl::cast_api_result(rc);
            return;
        }
        _errc = ktl::errc::success;
    }

    instance(const instance &) = delete;
    instance(instance &&)      = delete;
    instance &
    operator=(const instance &) = delete;
    instance &
    operator=(instance &&) = delete;
    ~instance() noexcept;

private:
    ktl::api::instance handle_ = nullptr;
};
} // namespace kochou

#endif
