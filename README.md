![logo](https://github.com/urlagushka/kochou/blob/main/pics/kochou2.png)
# kochou
## C++ 23 | VulkanAPI

### Compilation
```sh
xmake build
```

```
1.2 - descriptor_indexing
1.3 - mesh_shader, pipeline_callback, dynamic_rendering
```

### Color space priority
1. VK_COLOR_SPACE_HDR10_ST2084_EXT
2. VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT
3. VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT
4. VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT
5. VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
6. VK_COLOR_SPACE_BT709_NONLINEAR_EXT

git ls-files --exclude-standard -- ':!**/*.png' | xargs wc -l
xmake project -k compile_commands
