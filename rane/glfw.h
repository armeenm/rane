#pragma once

#define GLFW_INCLUDE_VULKAN

#include "rane/coord.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string_view>

auto glfw_error_callback(int const err, char const* const desc) -> void {
  spdlog::error("GLFW error {}: {}", err, desc);
}

class GlfwCtx {
public:
  GlfwCtx() {
    glfwSetErrorCallback(glfw_error_callback);

    auto const res = glfwInit();
    if (!res)
      throw std::runtime_error{fmt::format("Failed to initialize GLFW: {}", res)};

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (!glfwVulkanSupported())
      throw std::runtime_error{"GLFW: Vulkan not supported"};
  }

  GlfwCtx(GlfwCtx const&) = delete;
  GlfwCtx(GlfwCtx&&) noexcept {};

  auto operator=(GlfwCtx const&) -> GlfwCtx& = delete;
  auto operator=(GlfwCtx&&) noexcept -> GlfwCtx& { return *this; };

  ~GlfwCtx() { glfwTerminate(); }
};

class GlfwWindow {
public:
  GlfwWindow(GlfwCtx& ctx, Coord<int> dims, std::string_view title, GLFWmonitor* monitor)
      : ctx_{&ctx}, window_{glfwCreateWindow(dims.x, dims.y, title.data(), monitor, nullptr)} {}

  GlfwWindow(GlfwWindow const&) = delete;
  GlfwWindow(GlfwWindow&&) noexcept = default;

  auto operator=(GlfwWindow const&) -> GlfwWindow& = delete;
  auto operator=(GlfwWindow&&) noexcept -> GlfwWindow& = default;

  ~GlfwWindow() = default;

  [[nodiscard]] auto ctx() const noexcept -> GlfwCtx* { return ctx_; }
  [[nodiscard]] auto get() const noexcept -> GLFWwindow* { return window_.get(); }
  [[nodiscard]] auto dims() const -> std::pair<int, int> {
    int w, h;
    glfwGetFramebufferSize(window_.get(), &w, &h);
    return std::make_pair(w, h);
  }
  [[nodiscard]] auto should_close() const -> bool { return glfwWindowShouldClose(window_.get()); }

  auto set(GLFWwindow* window) noexcept -> void { window_.reset(window); }

private:
  struct GLFWwindowDeleter {
    auto operator()(GLFWwindow* window) const noexcept -> void { glfwDestroyWindow(window); }
  };

  GlfwCtx* ctx_;
  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window_;
};
