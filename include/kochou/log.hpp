#ifndef KOCHOU_LOG_HPP
#define KOCHOU_LOG_HPP

#include <chrono>
#include <ctime>
#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include <ktl/type.hpp>

namespace kochou::log
{
namespace color
{
static constexpr std::string_view reset  = "\033[0m";
static constexpr std::string_view red    = "\033[31m";
static constexpr std::string_view green  = "\033[32m";
static constexpr std::string_view yellow = "\033[33m";
static constexpr std::string_view blue   = "\033[34m";
static constexpr std::string_view bold   = "\033[1m";
} // namespace color

[[nodiscard]] inline std::string
format_time_point(std::chrono::time_point< std::chrono::system_clock > _tp) noexcept
{
    auto    time = std::chrono::system_clock::to_time_t(_tp);
    std::tm tm_buf{};

#ifdef _WIN32
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

enum class lvl_type : ktl::u8
{
    info    = 1,
    warning = 2,
    error   = 3,
    fatal   = 4,
    debug   = 5,
};

[[maybe_unused, nodiscard]] consteval bool
exit_condition(lvl_type _lvl)
{
    return static_cast< ktl::u8 >(_lvl) > KOCHOU_LOG_LEVEL;
}

#ifdef KOCHOU_BUILD_DEBUG
template < typename... ARGS >
[[maybe_unused]] inline void
debug(std::format_string< ARGS... > _fmt, ARGS &&... _args)
{
    if constexpr (exit_condition(lvl_type::debug))
    {
        return;
    }
    std::cout << std::format("[KOCHOU] {} DEBUG   ", format_time_point(std::chrono::system_clock::now()))
              << std::format(_fmt, std::forward< ARGS >(_args)...) << '\n';
}

template < typename... ARGS >
[[maybe_unused]] inline void
info(std::format_string< ARGS... > _fmt, ARGS &&... _args)
{
    if constexpr (exit_condition(lvl_type::info))
    {
        return;
    }
    std::cout << std::format("[KOCHOU] {} INFO    ", format_time_point(std::chrono::system_clock::now()))
              << std::format(_fmt, std::forward< ARGS >(_args)...) << '\n';
}

template < typename... ARGS >
[[maybe_unused]] inline void
warning(std::format_string< ARGS... > _fmt, ARGS &&... _args)
{
    if constexpr (exit_condition(lvl_type::warning))
    {
        return;
    }
    std::cout << std::format("[KOCHOU] {} WARNING ", format_time_point(std::chrono::system_clock::now()))
              << std::format(_fmt, std::forward< ARGS >(_args)...) << '\n';
}

template < typename... ARGS >
[[maybe_unused]] inline void
error(std::format_string< ARGS... > _fmt, ARGS &&... _args)
{
    if constexpr (exit_condition(lvl_type::error))
    {
        return;
    }
    std::cout << std::format("[KOCHOU] {} {}ERROR {}  ", format_time_point(std::chrono::system_clock::now()),
                             kochou::log::color::red, kochou::log::color::reset)
              << std::format(_fmt, std::forward< ARGS >(_args)...) << '\n';
}

template < typename... ARGS >
[[maybe_unused]] inline void
fatal(std::format_string< ARGS... > _fmt, ARGS &&... _args)
{
    if constexpr (exit_condition(lvl_type::fatal))
    {
        return;
    }
    std::cout << std::format("[KOCHOU] {} {}{}FATAL {}  ", format_time_point(std::chrono::system_clock::now()),
                             kochou::log::color::red, kochou::log::color::bold, kochou::log::color::reset)
              << std::format(_fmt, std::forward< ARGS >(_args)...) << '\n';
}

#elifdef KOCHOU_BUILD_RELEASE
template < typename... ARGS >
[[maybe_unused]] inline void
debug(std::format_string< ARGS... >, ARGS const &...)
{
}

template < typename... ARGS >
[[maybe_unused]] inline void
info(std::format_string< ARGS... >, ARGS const &...)
{
}

template < typename... ARGS >
[[maybe_unused]] inline void
warning(std::format_string< ARGS... >, ARGS const &...)
{
}

template < typename... ARGS >
[[maybe_unused]] inline void
error(std::format_string< ARGS... >, ARGS const &...)
{
}

template < typename... ARGS >
[[maybe_unused]] inline void
fatal(std::format_string< ARGS... >, ARGS const &...)
{
}

#else
static_assert(false && "kochou build mode unspecified!");
#endif
} // namespace kochou::log

#endif
