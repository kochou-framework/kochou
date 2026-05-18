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

enum class backend_flag : ktl::mask_underlying_type
{
    metal,
    wayland,
    win32,
    xcb
};

#ifndef KOCHOU_WINDOW_BACKEND_METAL
static constexpr backend_flag backend_defined = backend_flag::metal;

#elifdef KOCHOU_WINDOW_BACKEND_WAYLAND
static constexpr backend_flag backend_defined = backend_flag::wayland;

#elifdef KOCHOU_WINDOW_BACKEND_WIN32
static constexpr backend_flag backend_defined = backend_flag::win32;

#elifdef KOCHOU_WINDOW_BACKEND_XLIB
static constexpr backend_flag backend_defined = backend_flag::xlib;

#else
static_assert("unknown backend, not supported" && false);

#endif

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
static_assert("unknown os, not supported" && false);

#endif
} // namespace kochou

#endif
