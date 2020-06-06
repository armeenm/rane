#pragma once

#include "rane/glfw.h"

#include <cstdint>
#include <fmt/core.h>
#include <magic_enum.hpp>
#include <optional>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/view/transform.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
auto constexpr inline debug_build = false;
#else
auto constexpr inline debug_build = true;
#endif

auto const inline validation_layers = std::vector{"VK_LAYER_KHRONOS_validation"};
auto const inline device_exts = std::vector{"VK_KHR_SWAPCHAIN_EXTENSION_NAME"};

struct QueueFamilyIndices {
  std::optional<std::uint32_t> graphics_family;
  std::optional<std::uint32_t> present_family;

  auto has_values() const noexcept -> bool { return graphics_family && present_family; }
};

[[nodiscard]] auto create_instance(GlfwCtx&, std::uint32_t const version, std::string_view app_name)
    -> vk::Instance {

  spdlog::debug("Creating Vulkan instance for {} version {}", app_name, version);

  if constexpr (debug_build) {
    auto const supported_layers = vk::enumerateInstanceLayerProperties();

    auto supported_layer_strs =
        supported_layers | ranges::views::transform([](vk::LayerProperties const& layer) {
          return std::string{layer.layerName};
        });

    for (auto const& layer : supported_layer_strs)
      spdlog::debug("Vulkan supports layer {}", layer);

    for (std::size_t i = 0; i < validation_layers.size(); ++i) {
      spdlog::debug("Validation layer {} requested", validation_layers[i]);

      auto const pos = ranges::find(supported_layer_strs, validation_layers[i]);

      if (pos == supported_layer_strs.end())
        throw std::runtime_error{fmt::format(
            "Vulkan does not support requested validation layer {}", validation_layers[i])};
    }
  }

  // Check extension support //
  auto const supported_exts = vk::enumerateInstanceExtensionProperties();

  auto supported_ext_strs =
      supported_exts | ranges::views::transform([](vk::ExtensionProperties const& ext) {
        return std::string{ext.extensionName};
      });

  for (auto const& ext : supported_ext_strs)
    spdlog::debug("Vulkan supports extension {}", ext);

  auto const [required_exts, required_ext_count] = [] {
    std::uint32_t count;
    return std::make_pair(glfwGetRequiredInstanceExtensions(&count), count);
  }();

  for (auto i = 0U; i < required_ext_count; ++i) {
    spdlog::debug("GLFW requires extension {}", required_exts[i]);

    auto const pos = ranges::find(supported_ext_strs, required_exts[i]);

    if (pos == supported_ext_strs.end())
      throw std::runtime_error{
          fmt::format("Vulkan does not support required extension {}", required_exts[i])};
  }

  // Construct Instance //
  auto const app_info =
      vk::ApplicationInfo{app_name.data(), version, "No Engine", version, VK_API_VERSION_1_2};

  auto const inst_info = [&] {
    if constexpr (debug_build)
      return vk::InstanceCreateInfo{{},
                                    &app_info,
                                    std::uint32_t(validation_layers.size()),
                                    validation_layers.data(),
                                    required_ext_count,
                                    required_exts};
    else
      return vk::InstanceCreateInfo{{}, &app_info, {}, {}, required_ext_count, required_exts};
  }();

  return vk::createInstance(inst_info);
}

[[nodiscard]] auto create_surface(vk::Instance inst, GlfwWindow const& window) -> vk::SurfaceKHR {

  auto surface = vk::SurfaceKHR{};
  auto const res = static_cast<vk::Result>(glfwCreateWindowSurface(
      inst, window.get(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)));

  if (res != vk::Result::eSuccess)
    throw std::runtime_error{
        fmt::format("Failed to create surface with error {}", magic_enum::enum_name(res))};

  return surface;
}

[[nodiscard]] auto find_queue_families(vk::SurfaceKHR surface, vk::PhysicalDevice phys_dev)
    -> QueueFamilyIndices {
  spdlog::debug("Finding queue families...");

  auto const queue_families = phys_dev.getQueueFamilyProperties2();

  for (std::size_t i = 0; i < queue_families.size(); ++i) {
    // auto const present_support = phys_dev.getSurfaceSupportKHR(std::uint32_t(i), surface);
    auto const present_support = true;

    if (queue_families[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
      return {i, present_support ? std::make_optional(i) : std::nullopt};
  }

  return {std::nullopt, std::nullopt};
}

[[nodiscard]] auto pick_phys_dev(vk::Instance inst, vk::SurfaceKHR surface)
    -> std::pair<vk::PhysicalDevice, QueueFamilyIndices> {
  spdlog::debug("Picking physical device...");

  auto const phys_devs = inst.enumeratePhysicalDevices();

  if (phys_devs.size() == 0)
    throw std::runtime_error{"No physical devices with Vulkan support found"};

  for (auto const& phys_dev : phys_devs) {
    auto const props = phys_dev.getProperties2();

    spdlog::debug("Physical device {} of type {} found", props.properties.deviceName,
                  magic_enum::enum_name(props.properties.deviceType));

    auto const queue_indices = find_queue_families(surface, phys_dev);

    auto const dev_exts = phys_dev.enumerateDeviceExtensionProperties();

    for (auto const& dev_ext : dev_exts)
      spdlog::debug("Device {} supports extension {}", props.properties.deviceName, dev_ext);

    if (queue_indices.has_values())
      return {phys_dev, queue_indices};
  }

  throw std::runtime_error{"Failed to find capable physical device"};
}

[[nodiscard]] auto make_logical_device(vk::PhysicalDevice const& phys_dev,
                                       QueueFamilyIndices queue_indices) -> vk::Device {
  spdlog::debug("Constructing logical device...");

  auto const priority = 1.0F;
  auto const dev_queue_create_info =
      vk::DeviceQueueCreateInfo{{}, *queue_indices.graphics_family, 1, &priority};
  auto const dev_features = vk::PhysicalDeviceFeatures{};
  auto const dev_create_info =
      vk::DeviceCreateInfo{{}, 1, &dev_queue_create_info, 0, {}, 0, {}, &dev_features};

  return phys_dev.createDevice(dev_create_info);
}
