#ifndef KOCHOU_SYSTEM_HPP
#define KOCHOU_SYSTEM_HPP

#include <cassert>

#include <ktl/mask.hpp>

namespace kochou
{
enum class os_flag : ktl::mask_underlying_type
{
    linux,
    macos,
    windows
};

enum class api_flag : ktl::mask_underlying_type
{
    vulkan,
    metal,
    directx
};

#ifdef KOCHOU_PLATFORM_LINUX
static constexpr api_flag api_defined = api_flag::vulkan;
static constexpr os_flag  os_defined  = os_flag::linux;

#elifdef KOCHOU_PLATFORM_MACOS
static constexpr api_flag api_defined = api_flag::vulkan;
static constexpr os_flag  os_defined  = os_flag::macos;

#elifdef KOCHOU_PLATFORM_WIN32
static constexpr api_flag api_defined = api_flag::vulkan;
static constexpr os_flag  os_defined  = os_flag::windows;

#else
static_assert("unknown os! not supported!" && false);

#endif
} // namespace kochou

#endif
