#include <kochou/context/context.hpp>

#include "details/loader.hpp"

namespace
{
ktl::errc
load_basic(ktl::api::pfn_table & _ptable, ktl::loader::handle_type _handle) noexcept
{
    _ptable[static_cast< ktl::u32 >(ktl::api::command::enumerate_instance_version)] =
        kochou::details::system_load(_handle, "vkEnumerateInstanceVersion");
    _ptable[static_cast< ktl::u32 >(ktl::api::command::enumerate_instance_extension_properties)] =
        kochou::details::system_load(_handle, "vkEnumerateInstanceExtensionProperties");
    _ptable[static_cast< ktl::u32 >(ktl::api::command::enumerate_instance_layer_properties)] =
        kochou::details::system_load(_handle, "vkEnumerateInstanceLayerProperties");
    _ptable[static_cast< ktl::u32 >(ktl::api::command::create_instance)] =
        kochou::details::system_load(_handle, "vkCreateInstance");
    _ptable[static_cast< ktl::u32 >(ktl::api::command::get_instance_proc_addr)] =
        kochou::details::system_load(_handle, "vkGetInstanceProcAddr");

    return ktl::errc::success;
}
} // namespace

kochou::context::context(ktl::errc & _errc) noexcept
    : status_(ktl::errc::incomplete), instance_(nullptr), physical_device_(nullptr), device_(nullptr), handle_(nullptr),
      ptable_()
{
    auto result = ktl::loader::load();
    if (result.has_value())
    {
        handle_ = result.take_value();
    }
    else
    {
        kochou::log::error("{}", result.error().data);
        _errc = ktl::errc::pfn_load_failed;
        return;
    }
    _errc = load_basic(ptable_, handle_);

    ktl::api::ptable = &ptable_;
}

kochou::context::~context() noexcept
{
    kochou::log::debug("~context");
    auto result = ktl::loader::free(handle_);
    if (!result.has_value())
    {
        kochou::log::error("{}", result.error().data);
    }
};

ktl::errc
kochou::context::status() const noexcept
{
    return status_;
}
