#pragma once

#include "rane/glfw.h"

#include <algorithm>
#include <cstdint>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <magic_enum.hpp>
#include <optional>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/view/transform.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>
#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
auto constexpr inline debug_build = false;
#else
auto constexpr inline debug_build = false;
#endif

[[nodiscard]] auto make_instance(GlfwCtx&, std::uint32_t const version, std::string_view app_name)
    -> vk::Instance {

  spdlog::debug("Creating Vulkan instance for {} version {}", app_name, version);

  // Check validation layer support //
  if constexpr (debug_build) {
    auto const supported_layers = vk::enumerateInstanceLayerProperties();

    auto supported_layer_strs =
        supported_layers | ranges::views::transform([](vk::LayerProperties const& layer) {
          return std::string{layer.layerName};
        });

    for (auto const& layer : supported_layer_strs)
      spdlog::debug("Vulkan supports layer {}", layer);

    for (auto const& validation_layer : validation_layers) {
      spdlog::debug("Validation layer {} requested", validation_layer);

      auto const pos = ranges::find(supported_layer_strs, validation_layer);

      if (pos == supported_layer_strs.end())
        throw std::runtime_error{
            fmt::format("Vulkan does not support requested validation layer {}", validation_layer)};
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

[[nodiscard]] auto make_surface(vk::Instance const& inst, GlfwWindow const& window)
    -> vk::SurfaceKHR {

  auto surface = vk::SurfaceKHR{};
  auto const res = static_cast<vk::Result>(glfwCreateWindowSurface(
      VkInstance(inst), window.get(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)));

  if (res != vk::Result::eSuccess)
    throw std::runtime_error{
        fmt::format("Failed to create surface with error {}", magic_enum::enum_name(res))};

  return surface;
}

[[nodiscard]] auto find_queue_families(vk::SurfaceKHR const& surface,
                                       vk::PhysicalDevice const& phys_dev) -> QueueFamilyIndices {

  spdlog::debug("Finding queue families...");

  auto const queue_families = phys_dev.getQueueFamilyProperties();

  spdlog::debug("# of queue families: {}", queue_families.size());

  for (std::size_t i = 0; i < queue_families.size(); ++i) {
    if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
      auto const present_support = phys_dev.getSurfaceSupportKHR(std::uint32_t(i), surface);

      return {i, present_support ? std::make_optional(i) : std::nullopt};
    }
  }

  return {std::nullopt, std::nullopt};
}

[[nodiscard]] auto query_swapchain_support(vk::SurfaceKHR const& surface,
                                           vk::PhysicalDevice const& phys_dev)
    -> SwapChainSupportDetails {

  return SwapChainSupportDetails{phys_dev.getSurfaceCapabilitiesKHR(surface),
                                 phys_dev.getSurfaceFormatsKHR(surface),
                                 phys_dev.getSurfacePresentModesKHR(surface)};
}

[[nodiscard]] auto make_phys_dev(vk::Instance const& inst, vk::SurfaceKHR const& surface)
    -> std::pair<vk::PhysicalDevice, QueueFamilyIndices> {

  spdlog::debug("Picking physical device...");

  auto const phys_devs = inst.enumeratePhysicalDevices();
  auto supports_exts = false;
  auto compatible_swapchains = false;

  if (phys_devs.size() == 0)
    throw std::runtime_error{"No physical devices with Vulkan support found"};

  for (auto const& phys_dev : phys_devs) {
    auto const props = phys_dev.getProperties2();

    spdlog::debug("Physical device {} of type {} found", props.properties.deviceName,
                  magic_enum::enum_name(props.properties.deviceType));

    auto const queue_indices = find_queue_families(surface, phys_dev);

    auto const dev_exts = phys_dev.enumerateDeviceExtensionProperties();

    for (auto const& dev_ext : dev_exts)
      spdlog::debug("Device {} supports extension {}", props.properties.deviceName,
                    dev_ext.extensionName);

    auto dev_ext_strs = dev_exts | ranges::views::transform([](vk::ExtensionProperties const& ext) {
                          return std::string{ext.extensionName};
                        });

    for (auto const& required_dev_ext : required_dev_exts) {
      auto const pos = ranges::find(dev_ext_strs, required_dev_ext);

      if (pos == dev_ext_strs.end()) {
        supports_exts = false;
        spdlog::debug("Device {} does not support required extension {}",
                      props.properties.deviceName, required_dev_ext);
        break;
      } else {
        supports_exts = true;

        auto const swapchain_support = query_swapchain_support(surface, phys_dev);
        compatible_swapchains = swapchain_support.compatible();
      }
    }

    if (queue_indices.has_values() && supports_exts && compatible_swapchains)
      return {phys_dev, queue_indices};
  }

  throw std::runtime_error{"Failed to find capable physical device"};
}

[[nodiscard]] auto make_dev(vk::PhysicalDevice const& phys_dev,
                            QueueFamilyIndices const& queue_indices) -> vk::Device {
  spdlog::debug("Constructing logical device...");

  auto const priority = 1.0F;
  auto const dev_queue_create_info =
      vk::DeviceQueueCreateInfo{{}, *queue_indices.graphics_family, 1, &priority};
  auto const dev_features = vk::PhysicalDeviceFeatures{};
  auto const dev_create_info = vk::DeviceCreateInfo{{},
                                                    1,
                                                    &dev_queue_create_info,
                                                    0,
                                                    {},
                                                    uint32_t(required_dev_exts.size()),
                                                    required_dev_exts.data(),
                                                    &dev_features};

  return phys_dev.createDevice(dev_create_info);
}

[[nodiscard]] auto make_desc_pool(vk::Device const& dev) -> vk::DescriptorPool {
  auto const pool_sizes = std::array{
      vk::DescriptorPoolSize{vk::DescriptorType::eSampler, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eCombinedImageSampler, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eSampledImage, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eStorageImage, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eUniformTexelBuffer, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eStorageTexelBuffer, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eUniformBufferDynamic, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eStorageBufferDynamic, 1000},
      vk::DescriptorPoolSize{vk::DescriptorType::eInputAttachment, 1000},
  };

  auto const pool_info = vk::DescriptorPoolCreateInfo{
      {}, 1000U * pool_sizes.size(), pool_sizes.size(), pool_sizes.data()};

  return dev.createDescriptorPool(pool_info);
}

[[nodiscard]] auto
choose_swap_surface_format(std::vector<vk::SurfaceFormatKHR> const& available_formats)
    -> vk::SurfaceFormatKHR {

  for (auto const& available_format : available_formats)
    if (available_format.format == vk::Format::eA8B8G8R8SrgbPack32 &&
        available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
      return available_format;

  return available_formats[0];
}

[[nodiscard]] auto
choose_swap_present_mode(std::vector<vk::PresentModeKHR> const& available_present_modes)
    -> vk::PresentModeKHR {

  for (auto const& available_present_mode : available_present_modes)
    if (available_present_mode == vk::PresentModeKHR::eMailbox)
      return available_present_mode;

  return vk::PresentModeKHR::eFifo;
}

[[nodiscard]] auto choose_swap_extent(std::uint32_t width, std::uint32_t height,
                                      vk::SurfaceCapabilitiesKHR const& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max())
    return capabilities.currentExtent;
  else {
    auto actual_extent = vk::Extent2D{width, height};

    actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width,
                                     capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height,
                                      capabilities.maxImageExtent.height);

    return actual_extent;
  }
}

[[nodiscard]] auto make_swapchain(std::uint32_t width, std::uint32_t height, vk::SurfaceKHR surface,
                                  std::pair<vk::PhysicalDevice, QueueFamilyIndices> const& phys_dev,
                                  vk::Device const& dev)
    -> std::tuple<vk::SwapchainKHR, vk::Format, vk::Extent2D> {

  auto const support_details = query_swapchain_support(surface, phys_dev.first);
  auto const surface_format = choose_swap_surface_format(support_details.formats);
  auto const present_mode = choose_swap_present_mode(support_details.present_modes);
  auto const extent = choose_swap_extent(width, height, support_details.capabilities);
  auto const image_count =
      (support_details.capabilities.maxImageCount &&
       support_details.capabilities.minImageCount == support_details.capabilities.maxImageCount)
          ? support_details.capabilities.maxImageArrayLayers
          : support_details.capabilities.minImageCount + 1;

  auto const& indices = phys_dev.second;
  auto const queue_family_array = std::array{*indices.graphics_family, *indices.present_family};

  auto const exclusive = indices.graphics_family == indices.present_family;

  auto const sharing_mode = exclusive ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent;
  auto const family_index_count = exclusive ? 0U : 2U;
  auto const queue_family_indices = exclusive ? nullptr : queue_family_array.data();

  auto const swapchain_info =
      vk::SwapchainCreateInfoKHR{{},
                                 surface,
                                 image_count,
                                 surface_format.format,
                                 surface_format.colorSpace,
                                 extent,
                                 1,
                                 vk::ImageUsageFlagBits::eColorAttachment,
                                 sharing_mode,
                                 family_index_count,
                                 queue_family_indices,
                                 support_details.capabilities.currentTransform,
                                 vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                 present_mode,
                                 true,
                                 {}};

  return std::make_tuple(dev.createSwapchainKHR(swapchain_info), surface_format.format, extent);
}

[[nodiscard]] auto make_image_views(vk::Device const& dev, std::vector<vk::Image> const& images,
                                    vk::Format format) -> std::vector<vk::ImageView> {

  auto imageviews = std::vector<vk::ImageView>{images.size()};

  for (std::size_t i = 0; i < imageviews.size(); ++i) {
    auto const image_view_info = vk::ImageViewCreateInfo{
        {},     images[i], vk::ImageViewType::e2D,
        format, {},        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

    imageviews[i] = dev.createImageView(image_view_info);
  }

  return imageviews;
}
