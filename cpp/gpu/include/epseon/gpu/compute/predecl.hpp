#pragma once

#include "epseon/gpu/compute/layout.hpp"
#include "vk_mem_alloc.h"
#include <vulkan/vulkan_core.h>

#include <concepts>
#include <cstdint>
#include <memory>
#include <vulkan/vulkan_enums.hpp>

namespace epseon::gpu::cpp {

    namespace environment {
        class Device;
    }

    namespace layout {
        template <typename T>
        concept Concept = requires(T t) {
            { t.getItemCount() } -> std::same_as<uint64_t>;
            { t.getItemSize() } -> std::same_as<uint64_t>;
            { t.getTotalSizeBytes() } -> std::same_as<uint64_t>;
            { t.getSet() } -> std::same_as<uint32_t>;
            { t.getBinding() } -> std::same_as<uint32_t>;
        };

        class Base;

        template <typename contentT>
        class Static;

        class Dynamic;
    } // namespace layout

    namespace scaling {
        class Base;
        class BufferArray;
        class LargeBuffer;
    } // namespace scaling

    namespace resources {
        template <typename T>
        concept Concept = requires(T                                     t,
                                   std::shared_ptr<environment::Device>& deviceRef,
                                   std::shared_ptr<scaling::Base>&       scalingRef) {
            { t.prepare(deviceRef, scalingRef) } -> std::same_as<void>;
        };

        class Base; // NOLINT

        class Dynamic; // NOLINT

        class Static;
    } // namespace resources

    namespace allocation {
        template <typename T>
        concept Concept = requires(T t) {
            { t.getDescriptorType() } -> std::same_as<vk::DescriptorType>;
            { T::getBufferUsageFlags() } -> std::same_as<vk::BufferUsageFlags>;
            { T::getAllocationFlags() } -> std::same_as<vma::AllocationCreateFlags>;
        };

        template <VkBufferUsageFlags       bufferUsageFlags,
                  VmaAllocationCreateFlags allocationFlags,
                  layout::Concept          layoutT>
        class Allocation;
    } // namespace allocation

    namespace buffer {
        template <layout::Concept layoutT, allocation::Concept boundAllocationT>
        class Base;

        template <layout::Concept layoutT>
        class DeviceLocal;

        template <typename sourceBufferT,
                  typename destinationBufferT,
                  allocation::Concept boundAllocationT,
                  layout::Concept     layoutT>
        class Transferable;

        template <layout::Concept layoutT>
        class HostToDevice;

        template <layout::Concept layoutT>
        class DeviceToHost;
    } // namespace buffer

    namespace shader {

        template <resources::Concept resourceT>
        class Base;

        class Dynamic; // NOLINT

        template <resources::Concept resourceT>
        class Static;
    } // namespace shader

    struct DescriptorSetWrite;
} // namespace epseon::gpu::cpp