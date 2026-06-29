# kochou
Framework core provides:
* graphics subsystem context (kochou::context)
* requirements system
* base Vulkan entities with optional lifetime control

## Requirements system
User can request some hardware feature, in Vulkan there are:
* ```ktl::api::extension::*```
* ```ktl::api::feature::*```
* ```ktl::api::queue_flag_bits::*```
* (in dev) ```ktl::api::format::*```
* (in dev) ```ktl::api::layer::*```
* (in dev) ```ktl::api::version::*```
* (in dev) memory requirements

### ```kochou::ensure```
If requirement not satisfied, context initialization will return error.
### ```kochou::should```
If requirement not satisfied, context initialization will return no error.
### ```kochou::allowed```
After context initialization, you can verify is requirement satisfied in facts.
