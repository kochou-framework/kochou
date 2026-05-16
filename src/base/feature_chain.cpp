#include <set>
#include <unordered_map>

#include <ktl/memory.hpp>
#include <ktl/meta.hpp>

#include <kochou/base/feature_chain.hpp>

ktl::result< kochou::feature_chain::description, ktl::errc >
kochou::calloc_struct(ktl::meta::any_feature _meta) noexcept
{
    auto mapped_rc = ktl::memory::palloc< ktl::u8 >(_meta.sizeof_struct, 0);
    if (!mapped_rc.has_value()) [[unlikely]]
    {
        return ktl::err(mapped_rc.error());
    }
    auto * mapped = mapped_rc.take_value();

    *reinterpret_cast< ktl::api::structure_type * >(mapped + _meta.offsetof_stype) = _meta.stype;
    *reinterpret_cast< void ** >(mapped + _meta.offsetof_pnext)                    = nullptr;

    return kochou::feature_chain::description{mapped, _meta.stype, _meta.sizeof_struct, _meta.offsetof_stype,
                                              _meta.offsetof_pnext};
}

void
kochou::free_struct(kochou::feature_chain::description _description) noexcept
{
    ktl::memory::pfree< ktl::u8 >(static_cast< ktl::u8 * >(_description.mapped), _description.size);
}

kochou::feature_chain::~feature_chain() noexcept
{
    for (auto & chainlet : chain)
    {
        kochou::free_struct(chainlet);
    }
}
