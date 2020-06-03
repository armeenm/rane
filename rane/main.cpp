#include <stdexcept>
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_TYPESAFE_CONVERSION
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <cassert>
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <vulkan/vulkan.hpp>

struct GLFWwindowDeleter {
  auto operator()(GLFWwindow* window) const noexcept -> void {
    glfwDestroyWindow(window);
  }
};

class GlfwCtx {
 public:
  GlfwCtx() { glfwInit(); }

  GlfwCtx(GlfwCtx const&) noexcept = delete;
  GlfwCtx(GlfwCtx&&) noexcept = delete;

  auto operator=(GlfwCtx const&) -> GlfwCtx& = delete;
  auto operator=(GlfwCtx&&) noexcept -> GlfwCtx& = delete;

  ~GlfwCtx() { glfwTerminate(); }
};

class VulkanCtx {
 public:
  vk::Instance inst;

  explicit VulkanCtx(GlfwCtx&, std::uint32_t const version,
                     std::string_view app_name) {
    auto const app_info =
        vk::ApplicationInfo{app_name.data(), version, "No Engine", version,
                            VK_MAKE_VERSION(1, 0, 39)};

    auto ext_count = uint32_t{};
    auto const ext = glfwGetRequiredInstanceExtensions(&ext_count);

    auto const inst_info =
        vk::InstanceCreateInfo{{}, &app_info, {}, {}, ext_count, ext};

    auto const res = vk::createInstance(&inst_info, nullptr, &inst);

    if (res != vk::Result::eSuccess)
      throw std::runtime_error("Failed to create Vulkan instance!");
  }

  VulkanCtx(VulkanCtx const&) = delete;
  VulkanCtx(VulkanCtx&&) noexcept = delete;

  auto operator=(VulkanCtx const&) -> VulkanCtx& = delete;
  auto operator=(VulkanCtx&&) noexcept -> VulkanCtx& = delete;
};

class Rane {
 public:
  Rane(GlfwCtx& glfw_ctx, VulkanCtx& vulkan_ctx)
      : glfw_ctx_{&glfw_ctx}, vulkan_ctx_{&vulkan_ctx} {
    init_window();
  }

  [[nodiscard]] auto loop() -> bool {
    glfwPollEvents();
    auto stay_open = !glfwWindowShouldClose(window_.get());

    return stay_open;
  }

 private:
  GlfwCtx* glfw_ctx_;
  VulkanCtx* vulkan_ctx_;

  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window_;

  auto init_window() -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_.reset(glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr));
  }
};

int main() {
  auto glfw_ctx = GlfwCtx{};
  auto vulkan_ctx = VulkanCtx{glfw_ctx, VK_MAKE_VERSION(0, 0, 1), "RANE"};

  auto rane = Rane{glfw_ctx, vulkan_ctx};

  while (rane.loop()) {
  }
}
