#pragma once

#include <GLFW/glfw3.h>

class GlfwCtx {
public:
  GlfwCtx() { glfwInit(); }

  GlfwCtx(GlfwCtx const&) noexcept = delete;
  GlfwCtx(GlfwCtx&&) noexcept = delete;

  auto operator=(GlfwCtx const&) -> GlfwCtx& = delete;
  auto operator=(GlfwCtx&&) noexcept -> GlfwCtx& = delete;

  ~GlfwCtx() { glfwTerminate(); }
};

struct GLFWwindowDeleter {
  auto operator()(GLFWwindow* window) const noexcept -> void { glfwDestroyWindow(window); }
};

