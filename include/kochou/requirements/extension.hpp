#ifndef KOCHOU_REQUIREMENTS_EXTENSION_HPP
#define KOCHOU_REQUIREMENTS_EXTENSION_HPP

#include <set>
#include <string_view>

#include <ktl/errc.hpp>
#include <ktl/meta.hpp>
#include <ktl/result.hpp>

#include <kochou/context/context.hpp>

namespace kochou
{
template < ktl::api::extension >
class extension
{
public:
    static ktl::errc
    ensure(kochou::shared_context _sctx) noexcept;
    static ktl::errc
    should(kochou::shared_context _sctx) noexcept;

    static bool
    allowed(kochou::shared_context _sctx) noexcept;

private:
    template < ktl::api::extension, ktl::usize MAX_PAIRS = 256, ktl::usize MAX_VISITED = 64 >
    static consteval ktl::result< std::pair< std::array< kochou::resolver::dependency, MAX_PAIRS >, ktl::usize >,
                                  ktl::errc >
    apply() noexcept;
};
} // namespace kochou

template < ktl::api::extension EXTENSION >
ktl::errc
kochou::extension< EXTENSION >::ensure(kochou::shared_context _sctx) noexcept
{

    auto result = apply< EXTENSION >();
    if (!result.has_value())
    {
        return result.error();
    }

    auto [deps, size] = result.take_value();
    kochou::log::debug("ensure extension {}, size={}", static_cast< ktl::u32 >(EXTENSION), size);
    for (ktl::usize i = 0; i < size; ++i)
    {
        auto rc = _sctx->ensure(deps[i]);
        if (rc != ktl::errc::success)
        {
            return rc;
        }
    }

    return ktl::errc::success;
}

template < ktl::api::extension EXTENSION >
ktl::errc
kochou::extension< EXTENSION >::should(kochou::shared_context _sctx) noexcept
{
    auto result = apply< EXTENSION >();
    if (!result.has_value())
    {
        return result.error();
    }

    auto [deps, size] = result.take_value();
    for (ktl::usize i = 0; i < size; ++i)
    {
        auto rc = _sctx->should(deps[i]);
        if (rc != ktl::errc::success)
        {
            return rc;
        }
    }

    return ktl::errc::success;
}

template < ktl::api::extension EXTENSION >
bool
kochou::extension< EXTENSION >::allowed(kochou::shared_context _sctx) noexcept
{
    return _sctx->allowed(EXTENSION);
}

template < ktl::api::extension DUMMY >
template < ktl::api::extension START, ktl::usize MAX_PAIRS, ktl::usize MAX_VISITED >
consteval ktl::result< std::pair< std::array< kochou::resolver::dependency, MAX_PAIRS >, ktl::usize >, ktl::errc >
kochou::extension< DUMMY >::apply() noexcept
{
    std::array< kochou::resolver::dependency, MAX_PAIRS > result{};
    std::array< ktl::api::extension, MAX_VISITED >        visited{};
    ktl::usize                                            result_count  = 0;
    ktl::usize                                            visited_count = 0;

    auto copy_commands = [](std::span< ktl::api::command const > src)
    {
        kochou::resolver::cmdarray dst{};
        ktl::usize                 count = 0;
        for (auto cmd : src)
        {
            if (count >= kochou::resolver::cmdarray_size)
            {
                break;
            }
            dst[count++] = cmd;
        }
        return std::pair{dst, count};
    };

    auto process = [&](auto && _self, ktl::api::extension _extension)
    {
        for (ktl::usize i = 0; i < visited_count; ++i)
        {
            if (visited[i] == _extension)
            {
                return ktl::errc::success;
            }
        }
        if (visited_count >= MAX_VISITED)
        {
            return ktl::errc::compute_failed;
        }
        visited[visited_count++] = _extension;

        auto spec                = ktl::meta::extension_cast(_extension);
        auto [ext_cmds, ext_cnt] = copy_commands(spec.commands);
        for (const auto & version : ktl::api::common_versions)
        {
            if (spec.promoted.is_version())
            {
                if (spec.promoted.version() <= version)
                {
                    continue;
                }
            }
            result[result_count++] = {version, _extension, ext_cmds, ext_cnt};
        }

        for (auto const & vd : spec.deps)
        {
            if (result_count < MAX_PAIRS)
            {
                result[result_count++] = {vd.version, _extension, ext_cmds, ext_cnt};
            }
            else
            {
                return ktl::errc::compute_failed;
            }

            for (std::size_t i = 0; i < vd.deps_size; ++i)
            {
                if (result_count >= MAX_PAIRS)
                {
                    return ktl::errc::compute_failed;
                }
                auto const &               dep = vd.deps[i];
                kochou::resolver::cmdarray dep_cmds{};
                ktl::usize                 dep_cnt = 0;

                if (dep.is_extension())
                {
                    auto sub_ext    = dep.extension();
                    auto sub_spec   = ktl::meta::extension_cast(sub_ext);
                    auto [arr, cnt] = copy_commands(sub_spec.commands);
                    dep_cmds        = arr;
                    dep_cnt         = cnt;
                }
                result[result_count++] = {vd.version, dep, dep_cmds, dep_cnt};

                if (dep.is_extension())
                {
                    auto sub_rc = _self(_self, dep.extension());
                    if (sub_rc != ktl::errc::success)
                    {
                        return sub_rc;
                    }
                }
            }
        }
        return ktl::errc::success;
    };

    auto rc = process(process, START);
    if (rc != ktl::errc::success)
    {
        return ktl::err(rc);
    }
    return std::pair{result, result_count};
}

#endif
