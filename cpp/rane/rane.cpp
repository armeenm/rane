#include "rane/rane.h"

auto constexpr inline window_dims = Coord{800, 600};
auto constexpr inline app_name = "RANE";
auto constexpr inline version = VK_MAKE_VERSION(0, 0, 1);
auto const inline validation_layers = std::vector{"VK_LAYER_KHRONOS_validation"};
auto const inline required_dev_exts = std::vector{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

Rane::Rane() : window_{glfw_ctx_, window_dims, app_name, nullptr} {
  spdlog::debug("Constructing Rane instance");
}

Rane::~Rane() {
  /*
  for (auto&& image_view : swapchain_image_views_)
    vkDestroyImageView(dev_, image_view, nullptr);

  vkDestroySwapchainKHR(dev_, std::get<0>(swapchain_), nullptr);
  */
  dev_.destroyDescriptorPool(desc_pool_);
  dev_.destroy();
  inst_.destroySurfaceKHR(surface_);
  inst_.destroy();
}

auto Rane::loop() const -> void { glfwPollEvents(); }
auto Rane::done() const -> bool { return window_.should_close(); }

auto Rane::QueueFamilyIndices::has_values() const noexcept -> bool {
  return graphics_family && present_family;
}

auto Rane::SwapChainSupportDetails::compatible() const noexcept -> bool {
  return !formats.empty() && !present_modes.empty();
}
