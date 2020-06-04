#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "rane/glfw.h"
#include "rane/rane.h"
#include "rane/vulkan.h"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

auto constexpr inline window_width = 800U;
auto constexpr inline window_height = 600U;
auto constexpr inline window_title = "RANE";

int main() {
  try {
    spdlog::set_level(spdlog::level::debug);

    auto glfw_ctx = GlfwCtx{};
    auto vulkan_ctx = VulkanCtx{glfw_ctx, VK_MAKE_VERSION(0, 0, 1), "RANE"};

    auto rane = Rane{glfw_ctx, vulkan_ctx, window_width, window_height, window_title};

    while (!rane.done())
      rane.loop();

  } catch (std::exception const& e) {
    spdlog::error(e.what());
  }
}
