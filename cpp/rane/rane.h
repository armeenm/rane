#pragma once

#include "rane/coord.h"
#include "rane/glfw.h"

#include <cstdint>
#include <memory>
#include <spdlog/spdlog.h>
#include <string_view>
#include <utility>
#include <vulkan/vulkan.hpp>

class Rane {
public:
  explicit Rane();

  Rane(Rane const&) = delete;
  Rane(Rane&&) = delete;

  auto operator=(Rane const&) -> Rane& = delete;
  auto operator=(Rane &&) -> Rane& = delete;

  ~Rane();

  auto loop() const -> void;
  [[nodiscard]] auto done() const -> bool;

private:
  struct QueueFamilyIndices {
    std::optional<std::uint32_t> graphics_family;
    std::optional<std::uint32_t> present_family;

    [[nodiscard]] auto has_values() const noexcept -> bool;
  };

  struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> present_modes;

    [[nodiscard]] auto compatible() const noexcept -> bool;
  };

  GlfwCtx glfw_ctx_;
  GlfwWindow window_;
  vk::Instance inst_;
  vk::SurfaceKHR surface_;
  std::pair<vk::PhysicalDevice, QueueFamilyIndices> phys_dev_w_idx_;
  vk::Device dev_;
  vk::Queue queue_;
  vk::DescriptorPool desc_pool_;

  /*
  std::tuple<vk::SwapchainKHR, vk::Format, vk::Extent2D> swapchain_ =
      make_swapchain(window_width, window_height, surface_, phys_dev_, dev_);
  std::vector<vk::Image> swapchain_images_ = dev_.getSwapchainImagesKHR(std::get<0>(swapchain_));
  std::vector<vk::ImageView> swapchain_image_views_ =
      make_image_views(dev_, swapchain_images_, std::get<1>(swapchain_));
  */
};
