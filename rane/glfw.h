#pragma once

#include <GLFW/glfw3.h>
#include <memory>

struct GLFWwindowDeleter {
  auto operator()(GLFWwindow* window) const noexcept -> void { glfwDestroyWindow(window); }
};

class GlfwCtx {
public:
  GlfwCtx() { glfwInit(); }

  GlfwCtx(GlfwCtx const&) noexcept = delete;
  GlfwCtx(GlfwCtx&&) noexcept = delete;

  auto operator=(GlfwCtx const&) -> GlfwCtx& = delete;
  auto operator=(GlfwCtx&&) noexcept -> GlfwCtx& = delete;

  ~GlfwCtx() { glfwTerminate(); }

  [[nodiscard]] auto window() const noexcept -> GLFWwindow* { return window_.get(); }
  auto window(GLFWwindow* window) noexcept -> void { window_.reset(window); }

private:
  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window_;
};
