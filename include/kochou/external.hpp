#ifndef KOCHOU_EXTERNAL_HPP
#define KOCHOU_EXTERNAL_HPP

#include <concepts>

#include <ktl/errc.hpp>
#include <ktl/memory.hpp>
#include <ktl/result.hpp>

#include <kochou/log.hpp>

namespace kochou
{
enum class hold
{
    unique,
    shared
};

template < hold, typename >
class external
{
};

template < typename T >
struct external< hold::unique, T >
{
    using ptr_type    = ktl::memory::uptr< T >;
    using result_type = ktl::result< ptr_type, ktl::errc >;

    external() noexcept  = default;
    ~external() noexcept = default;

    external(const external &) = delete;
    external(external &&)      = delete;
    external &
    operator=(const external &) = delete;
    external &
    operator=(external &&) = delete;

    template < typename... ARGS >
    // requires std::constructible_from< T, ktl::errc &, ARGS... >
    static result_type
    make(ARGS &&... _args) noexcept
    {
        ktl::errc   rc     = ktl::errc::success;
        result_type result = ktl::memory::make_unique< T >(rc, std::forward< ARGS >(_args)...);
        if (!result.has_value())
        {
            return ktl::err(result.error());
        }
        if (rc != ktl::errc::success)
        {
            return ktl::err(rc);
        }
        return result.take_ok();
    }
};

template < typename T >
struct external< hold::shared, T >
{
    using ptr_type    = ktl::memory::sptr< T >;
    using result_type = ktl::result< ptr_type, ktl::errc >;

    external() noexcept  = default;
    ~external() noexcept = default;

    external(const external &) = delete;
    external(external &&)      = delete;
    external &
    operator=(const external &) = delete;
    external &
    operator=(external &&) = delete;

    template < typename... ARGS >
    // requires std::constructible_from< T, ktl::errc &, ARGS... >
    static result_type
    make(ARGS &&... _args) noexcept
    {
        ktl::errc   rc     = ktl::errc::success;
        result_type result = ktl::memory::make_shared< T >(rc, std::forward< ARGS >(_args)...);
        if (!result.has_value())
        {
            return ktl::err(result.error());
        }
        if (rc != ktl::errc::success)
        {
            return ktl::err(rc);
        }
        return result.take_value();
    }
};
} // namespace kochou

#endif
