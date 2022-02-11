#define ATS_DYNLOADFLAG 0

#include "share/atspre_staload.hats"
#include "glfw.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "glfw.sats"

assume Glfw = unit

implement glfw_set_error_callback callback =
  $extfcall(glfw_error_fun, "glfwSetErrorCallback", callback)

implement glfw_init () = let
  val err = ~int2bool($extfcall(int, "glfwInit"))
in
  if err then None_vt() else Some_vt(unit())
end

implement glfw_terminate _ = $extfcall(void, "glfwTerminate")

implement glfw_window_hint (glfw, hint, value) =
  $extfcall(void, "glfwWindowHint", hint, value)

implement glfw_poll_events glfw =
  $extfcall(void, "glfwPollEvents")

implement glfw_create_window (glfw, width, height, title, monitor_opt, share_opt) = let
  val monitor = case+ monitor_opt of
    | Some(x) => x
    | None()  => the_null_ptr

  val share = case+ share_opt of
    | Some(x) => x
    | None()  => the_null_ptr
in
  $extfcall(GlfwWindow, "glfwCreateWindow", width, height, title, monitor, share)
end

implement glfw_make_context_current win =
  $extfcall(void, "glfwMakeContextCurrent", win)

implement glfw_get_window_size win = let
  var width: Nat = 0
  var height: Nat = 0
  val () = $extfcall(void, "glfwGetWindowSize", win, addr@width, addr@height)
in
  (width, height)
end

implement glfw_window_should_close win =
  $extfcall(bool, "glfwWindowShouldClose", win)

implement glfw_swap_buffers win =
  $extfcall(void, "glfwSwapBuffers", win)
