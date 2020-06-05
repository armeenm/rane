#pragma once

#include "rane/glfw.h"
#include "rane/vulkan.h"

#include <cstdint>
#include <memory>
#include <spdlog/spdlog.h>
#include <string_view>
#include <vulkan/vulkan.hpp>

class Rane {
public:
  explicit Rane() {
    spdlog::debug("Constructing Rane instance");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  }

  Rane(Rane const&) = delete;
  Rane(Rane&&) = delete;

  auto operator=(Rane const&) -> Rane& = delete;
  auto operator=(Rane &&) -> Rane& = delete;

  ~Rane() { inst_.destroy(); }

  auto loop() -> void { glfwPollEvents(); }
  [[nodiscard]] auto done() -> bool { return glfwWindowShouldClose(window_.get()); }

private:
  auto constexpr static inline window_width = 800U;
  auto constexpr static inline window_height = 600U;
  auto constexpr static inline window_title = "RANE";

  GlfwCtx glfw_ctx_ = GlfwCtx{};
  GlfwWindow window_ = GlfwWindow{glfw_ctx_, window_width, window_height, window_title, nullptr};
  vk::Instance inst_ = create_instance(glfw_ctx_, VK_MAKE_VERSION(0, 0, 1), "RANE");
  vk::SurfaceKHR surface_ = create_surface(inst_, window_);
  std::pair<vk::PhysicalDevice, QueueFamilyIndices> phys_dev_ = pick_phys_dev(inst_);
  vk::Device dev_ = make_logical_device(phys_dev_.first, phys_dev_.second);
};
