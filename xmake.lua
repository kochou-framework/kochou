set_xmakever("3.0.0")
set_project("kochou")
set_languages("c++23")

add_rules("mode.debug", "mode.release")

-- log level
option("kochou_log_level")
    set_description("log level: info=1, warning=2, error=3, fatal=4, debug=5")
    set_values("0", "1", "2", "3", "4", "5")
    after_check(function (opt)
        local val = opt:value()
        if not val or val == "" then
            raise("kochou_log_level is invalid (maybe null)")
        end
    end)
option_end()

-- build mode
option("kochou_build_mode")
    set_description("build mode: debug, release")
    set_values("debug", "release")
    after_check(function (opt)
        local val = opt:value()
        if not val or val == "" then
            raise("kochou_build_mode is invalid (maybe null)")
        end
    end)
option_end()

-- vulkan dynamic library path
option("kochou_dylib_path")
    set_description("vulkan dynamic library path")
    after_check(function (opt)
        local val = opt:value()
        if not val or val == "" then
            raise("kochou_dylib_path is invalid (maybe null)")
        end
    end)
option_end()

-- target platform
option("kochou_platform")
    set_description("target platform: macos, linux, windows")
    set_values("macos", "linux", "windows")
    after_check(function (opt)
        local val = opt:value()
        if not val or val == "" then
            raise("kochou_platform is invalid (maybe null)")
        end
    end)
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
        "src/details/resolve_version.cpp"
    )

    add_includedirs("include", {public = true})

    on_config(function (target)
        local log_level = get_config("kochou_log_level")
        target:add('defines', 'KOCHOU_LOG_LEVEL="' .. log_level .. '"', {public=true})

        local build_mode = get_config("kochou_build_mode")
        target:add('defines', 'KOCHOU_BUILD_' .. build_mode:upper(), {public=true})

        local dylib_path = get_config("kochou_dylib_path")
        target:add('defines', 'KOCHOU_DYLIB_PATH="' .. dylib_path .. '"', {public=true})

        local platform = get_config("kochou_platform")
        target:add('defines', 'KOCHOU_PLATFORM_' .. platform:upper(), {public=true})
    end)
