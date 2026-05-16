#ifndef KOCHOU_IMPL_LOADER_HPP
#define KOCHOU_IMPL_LOADER_HPP

#include <string_view>

#include <ktl/api.hpp>
#include <ktl/loader.hpp>

namespace kochou::details
{
ktl::loader::proc_type
system_load(ktl::loader::handle_type _handle, std::string_view _symbol) noexcept;

ktl::api::pfn_void_function
instance_load(ktl::api::instance _instance, std::string_view _symbol) noexcept;

ktl::api::pfn_void_function
device_load(ktl::api::device _device, std::string_view _symbol) noexcept;
} // namespace kochou::details

#endif
