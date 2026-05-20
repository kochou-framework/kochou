set_xmakever("3.0.0")
set_project("kochou")
set_languages("c++23")

add_rules("mode.debug", "mode.release")

-- log level
option("kochou_log_level")
    set_description("log level: info=1, warning=2, error=3, fatal=4, debug=5")
    set_values("0", "1", "2", "3", "4", "5")
option_end()

-- build mode
option("kochou_build_mode")
    set_description("build mode: debug, release")
    set_values("debug", "release")
option_end()

includes("third_party/ktl")

target("kochou")
    add_deps("ktl")

    set_kind("static")

    add_files(
        "src/context.cpp",
        "src/resolver.cpp",

        "src/base/instance.cpp",
        "src/base/physical_device.cpp",
        "src/base/device.cpp",
        "src/base/feature_chain.cpp",

        "src/details/loader.cpp",
        "src/details/resolve_device.cpp",
        "src/details/resolve_instance.cpp",
        "src/details/resolve_queue.cpp",
        "src/details/resolve_version.cpp",

        "src/entity/image_view.cpp",
        "src/entity/image.cpp",
        "src/entity/surface.cpp",
        "src/entity/swapchain.cpp",
        "src/entity/fence.cpp",
        "src/entity/command_pool.cpp"
    )

    add_includedirs("include", {public = true})

    on_config(function (target)
        local function get_required_config(name)
            local val = get_config(name)
            if not val or val == "" then
                raise("error: option '" .. name .. "' is not set! Define it via set_config() or 'xmake f --" .. name .. "=value'")
            end
            return val
        end

        local log_level = get_required_config("kochou_log_level")
        target:add('defines', 'KOCHOU_LOG_LEVEL=' .. log_level, {public=true})

        local build_mode = get_required_config("kochou_build_mode")
        target:add('defines', 'KOCHOU_BUILD_' .. build_mode:upper(), {public=true})
    end)
