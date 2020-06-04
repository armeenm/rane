#pragma once

#include "rane/glfw.h"
#include "rane/vulkan.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <memory>
#include <spdlog/spdlog.h>
#include <string_view>

class Rane {
public:
  Rane(GlfwCtx& glfw_ctx, VulkanCtx& vulkan_ctx, std::uint32_t width, std::uint32_t height,
       std::string_view title)
      : glfw_ctx_{&glfw_ctx}, vulkan_ctx_{&vulkan_ctx} {

    spdlog::debug("Constructing Rane instance");
    init_window(width, height, title);
  }

  auto loop() -> void { glfwPollEvents(); }

  [[nodiscard]] auto done() -> bool { return glfwWindowShouldClose(window_.get()); }

private:
  GlfwCtx* glfw_ctx_;
  VulkanCtx* vulkan_ctx_;

  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window_;

  auto init_window(std::uint32_t width, std::uint32_t height, std::string_view title) -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    spdlog::info("Creating window...");
    spdlog::debug("Window size {}x{}, titled {}", width, height, title);

    window_.reset(glfwCreateWindow(width, height, "RANE", nullptr, nullptr));
  }
};
