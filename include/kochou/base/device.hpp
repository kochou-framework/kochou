#ifndef KOCHOU_BASE_DEVICE_HPP
#define KOCHOU_BASE_DEVICE_HPP

#include <ranges>

#include <ktl/api.hpp>

#include <kochou/base/feature_chain.hpp>
#include <kochou/external.hpp>

namespace kochou
{
struct view;

class device : public kochou::external< kochou::hold::shared, kochou::device >
{
private:
    friend struct kochou::view;

public:
    template < std::ranges::input_range EXTENSION_RANGE, std::ranges::input_range FEATURE_RANGE >
        requires std::same_as< std::ranges::range_value_t< EXTENSION_RANGE >, ktl::api::extension > &&
                 std::same_as< std::ranges::range_value_t< FEATURE_RANGE >, ktl::api::feature >
    device(ktl::errc & _errc, const ktl::api::version & _version, ktl::api::physical_device _physical_device,
           EXTENSION_RANGE && _extensions, FEATURE_RANGE && _features, ktl::u32 _family_index,
           ktl::api::queue_flags _queue) noexcept
        : handle_(nullptr)
    {
        auto chain_rc = kochou::make_feature_chain_filled(_version, _features);
        if (!chain_rc.has_value())
        {
            _errc = chain_rc.error();
        }
        auto chain = chain_rc.take_value();
        auto view  = std::forward< EXTENSION_RANGE >(_extensions) |
                    std::views::transform(
                        [](ktl::api::extension ext)
                        {
                            auto raw_name = ktl::meta::extension_cast(ext).raw_name;
                            kochou::log::debug("device enable {}", raw_name);
                            return raw_name.data();
                        });
        std::vector< const char * > extensions_vec(std::ranges::begin(view), std::ranges::end(view));

        // TODO
        // сделать какую-нибудь нормальную конфигурацию очередей
        float                              queue_priority[1] = {1.0f};
        ktl::api::device_queue_create_info queue             = {
                        .queue_family_index = _family_index, .queue_count = 1, .p_queue_priorities = queue_priority};
        ktl::api::device_create_info info = {.flags                   = 0,
                                             .queue_create_info_count = 0,
                                             .p_queue_create_infos    = &queue,
                                             .enabled_layer_count     = 1,
                                             .pp_enabled_layer_names  = nullptr,
                                             .enabled_extension_count = static_cast< ktl::u32 >(extensions_vec.size()),
                                             .pp_enabled_extension_names = extensions_vec.data(),
                                             .p_enabled_features         = &chain.core};
        if (chain.head)
        {
            info.pnext              = chain.head;
            info.p_enabled_features = nullptr;
        };

        ktl::api::result rc = ktl::api::create_device(_physical_device, &info, nullptr, &handle_);
        if (rc != ktl::api::result::v_success)
        {
            kochou::log::error("failed create device, rc={}", static_cast< ktl::i32 >(rc));
            _errc = ktl::cast_api_result(rc);
            return;
        }
        _errc = ktl::errc::success;
    }
    ~device() noexcept;

public:
    ktl::api::device handle_ = nullptr;
}; // namespace kochou
} // namespace kochou

#endif
