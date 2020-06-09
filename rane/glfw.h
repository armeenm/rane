#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <memory>
#include <string_view>

class GlfwCtx {
public:
  GlfwCtx() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  }

  GlfwCtx(GlfwCtx const&) = delete;
  GlfwCtx(GlfwCtx&&) noexcept = delete;

  auto operator=(GlfwCtx const&) -> GlfwCtx& = delete;
  auto operator=(GlfwCtx&&) noexcept -> GlfwCtx& = delete;

  ~GlfwCtx() { glfwTerminate(); }
};

class GlfwWindow {
public:
  GlfwWindow(GlfwCtx& ctx, int width, int height, std::string_view title, GLFWmonitor* monitor)
      : ctx_{&ctx}, window_{glfwCreateWindow(width, height, title.data(), monitor, nullptr)} {}

  GlfwWindow(GlfwWindow const&) = delete;
  GlfwWindow(GlfwWindow&&) noexcept = default;

  auto operator=(GlfwWindow const&) -> GlfwWindow& = delete;
  auto operator=(GlfwWindow&&) noexcept -> GlfwWindow& = default;

  ~GlfwWindow() = default;

  [[nodiscard]] auto ctx() const noexcept -> GlfwCtx* { return ctx_; }
  [[nodiscard]] auto get() const noexcept -> GLFWwindow* { return window_.get(); }

  auto set(GLFWwindow* window) noexcept -> void { window_.reset(window); }

private:
  struct GLFWwindowDeleter {
    auto operator()(GLFWwindow* window) const noexcept -> void { glfwDestroyWindow(window); }
  };

  GlfwCtx* ctx_;
  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window_;
};
