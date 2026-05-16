``` cpp
// SINGLE CONTEXT by default
// in kochou::context constructor:
    ktl::api::ptable = ptable_;

// MULTI CONTEXT define KOCHOU_MULTI_CONTEXT
// in kochou::context constructor:
    nothing

// in kochou::setup constructor:
    if (ktl::api::ptable != nullptr) [[unlikely]]
    {
        ktl::log_fatal("kochou::context invalid usage!");
    }
    ktl::api::ptable = kochou::view< ktl::api::pfn_table >{}(_sctx);

// in kochou::setup constructor:
    ktl::api::pfn_table ptable = kochou::view< ktl::api::pfn_table >{}(_sctx);
    if (ktl::api::ptable != ptable) [[unlikely]]
    {
        ktl::log_fatal("kochou::context invalid usage!");
    }
    ktl::api::ptable = nullptr;
```
