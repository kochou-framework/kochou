#ifndef KOCHOU_CONTEXT_SETUP_HPP
#define KOCHOU_CONTEXT_SETUP_HPP

#include <ktl/api.hpp>
#include <ktl/memory/pointers.hpp>

#include <kochou/context/view.hpp>
#include <kochou/log.hpp>

#include "context.hpp"

namespace kochou
{
class setup
{
public:
    explicit setup(ktl::memory::sptr< kochou::context > _sctx) : sctx_(_sctx), ptable_(nullptr)
    {
#ifndef KOCHOU_MULTI_CONTEXT
        if (ktl::api::ptable != nullptr) [[unlikely]]
        {
            kochou::log::fatal("invalid usase, ktl::api::ptable is not null!");
            std::abort();
        }
        ptable_          = kochou::view::ptable(_sctx);
        ktl::api::ptable = ptable_;
#else
        kochou::log::fatal("invalid usase, KOCHOU_MULTI_CONTEXT not defined!");
        std::abort();
#endif
    }
    ~setup()
    {
        if (ktl::api::ptable != ptable_)
        {
            kochou::log::fatal("invalid usase, ktl::api::ptable is not equal!");
            std::abort();
        }
        ktl::api::ptable = nullptr;
    }

private:
    ktl::memory::sptr< kochou::context > sctx_;
    ktl::api::pfn_table *                ptable_;
};
} // namespace kochou

#endif
