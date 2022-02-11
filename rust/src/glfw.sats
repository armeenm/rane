#include "share/atspre_staload.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "general.sats"

macdef GLFW_CONTEXT_VERSION_MAJOR = $extval(int, "GLFW_CONTEXT_VERSION_MAJOR")
macdef GLFW_CONTEXT_VERSION_MINOR = $extval(int, "GLFW_CONTEXT_VERSION_MINOR")

typedef glfw_error_fun = (int, string) -> void

dataviewtype glfw_ctx (bool) =
  | GlfwCtxOk (true)
  | GlfwCtxErr (false)

abst@ype glfw_monitor = ptr
abst@ype glfw_window = ptr

fn glfw_set_error_callback (glfw_error_fun): glfw_error_fun
fn glfw_init (): [b: bool] glfw_ctx(b)
fn glfw_terminate {b: bool} (glfw_ctx(b)): void
fn glfw_window_hint {b: bool} (!glfw_ctx(true), hint: int, value: int): void
fn glfw_create_window {b: bool}
  ( !glfw_ctx(b)
  , width:   Nat
  , height:  Nat
  , title:   string
  , monitor: Option(glfw_monitor)
  , share:   Option(glfw_window)
  ): glfw_window
fn glfw_make_context_current (glfw_window): void
