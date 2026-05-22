#ifndef KOCHOU_KOCHOU_HPP
#define KOCHOU_KOCHOU_HPP

// base
#include <kochou/base/device.hpp>
#include <kochou/base/feature_chain.hpp>
#include <kochou/base/instance.hpp>
#include <kochou/base/physical_device.hpp>
#include <kochou/base/queue_chain.hpp>

// context
#include <kochou/context/configure.hpp>
#include <kochou/context/context.hpp>
#include <kochou/context/resolver.hpp>
#include <kochou/context/setup.hpp>
#include <kochou/context/view.hpp>

// entity
#include <kochou/entity/buffer.hpp>
#include <kochou/entity/command_buffer.hpp>
#include <kochou/entity/command_pool.hpp>
#include <kochou/entity/descriptor_layout.hpp>
#include <kochou/entity/descriptor_pool.hpp>
#include <kochou/entity/descriptor_set.hpp>
#include <kochou/entity/device_memory.hpp>
#include <kochou/entity/fence.hpp>
#include <kochou/entity/image.hpp>
#include <kochou/entity/image_view.hpp>
#include <kochou/entity/pipeline.hpp>
#include <kochou/entity/pipeline_layout.hpp>
#include <kochou/entity/queue.hpp>
#include <kochou/entity/semaphore.hpp>
#include <kochou/entity/shader_module.hpp>
#include <kochou/entity/surface.hpp>
#include <kochou/entity/swapchain.hpp>

// requirements
#include <kochou/requirements/allowed.hpp>
#include <kochou/requirements/ensure.hpp>
#include <kochou/requirements/extension.hpp>
#include <kochou/requirements/feature.hpp>
#include <kochou/requirements/format.hpp>
#include <kochou/requirements/layer.hpp>
#include <kochou/requirements/queue.hpp>
#include <kochou/requirements/should.hpp>
#include <kochou/requirements/version.hpp>

// common
#include <kochou/external.hpp>
#include <kochou/log.hpp>

#endif
