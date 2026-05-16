# Core level
provides vulkan features

пользователь на вход дает список extensions/features с разными требованиями
- ensure (обязательно должно быть)
- should (по возможности)
1. построить дерево из всех предоставленных extensions/features
2. определить все расширения и возможности для каждой версии
3. узнать какую максимальную версию можно использовать
4. выбрать ветку с максимально доступной версией
5. если есть обязательные (ensure) extensions/features в ветке с недоступной версией, то вернуть ошибку и закончить программу
6. создать instance со всеми расширениями из выбранной ветки
- если создание вернуло ошибку VK_ERROR_INCOMPATIBLE_DRIVER, то выбрать ветку с предыдущей версией
- повторять пока не получится создать instance или пока не упремся в минимально возможную ветку
- если не удалось создать instance, то вернуть ошибку и закончить программу
7. получить список phisical_device для этого instance
8. отфильтровать их по внешним требованиям
9. отфильтровать их по требованиям из УЖЕ выбранной ветки
10. попробовать создать device, если успешно, то этап создания kochou::context завершен, можно продолжать работу программы, иначе вернуть ошибку и закончить программу

# loader design

```cpp
// context vtable
// hardcode index

// ktl

thread_local pnf_table * ptable = nullptr;

namespace ktl::api
{
    static constexpr pnf_table_size = 0;

    template < typename PROC_TYPE >
    using pnf_table = std::array< PROC_TYPE, pnf_table_size >;
    using pnf_id = ktl::u32;

    // generated
    using pnf_vk_create_instance = ktl::api::result (*)(const vk_instance_create_info *, const allocator_callbacks *,
                                                    ptr_instance *);

    ktl::api::result vk_create_instance(const vk_instance_create_info * _create_info, ptr_instance * _instance)
    {
        kochou::loader::proc_type ptr = &ptable[0]; // 0 for example
        if (ptr == kochou::loader::proc_null) [[unlikely]]
        {
            return ktl::api::result::v_error_unknown;
        }

        return ((pnf_vk_create_instance)ptr)(_create_info, nullptr, _instance);
    }
}

// kochou

#include <ktl/api.hpp>

namespace kochou
{
    // for example
    void draw_konata(kochou::context & _ctx /* or shared */, void * _konata)
    {
        kochou::setup_context(_ctx) ctx; // pnf_table = *_ctx

        ktl::api::draw_konata(/* ... */); // real vulkan-api function

        // pnf_table = nullptr
    }
}

```

std::errc sleep_konata_impl(std::size_t time)
{
    if (time > KONATA_SLEEP_MAX_VALUE)
    {
        return std::errc::invalid_argument;
    }

    ::sleep(time);

    return std::errc{};
}

{
    std::errc rc = sleep_konata_impl(10);
    if (rc != std::errc{})
    {
        std::cerr << "too high sleep value!" << std::endl;
        std::abort();
    }
    // process if success
}

ktl::result< float, std::errc > div_konata_impl(float a, float b)
{
    if (b == 0.0f)
    {
        return err(std::errc::invalid_argument);
    }
    return a / b;
}

{
    auto rc = div_konata_impl(10.0f, 0.1f);
    if (!rc.has_value())
    {
        std::cerr << "too high sleep value!" << std::endl;
        std::abort();
    }
    // process if success
    float value = rc.take_value();
}

#include <stdexcept>

void sleep_konata_impl(std::size_t time)
{
    if (time > KONATA_SLEEP_MAX_VALUE)
    {
        throw std::out_of_range("too high sleep value!");
    }

    ::sleep(time);
}

{
    try
    {
        sleep_konata_impl(10);
        // process if success
    }
    catch (const std::runtime_error & error)
    {
        std::cerr << "too high sleep value!" << std::endl;
        std::abort();
    }
}