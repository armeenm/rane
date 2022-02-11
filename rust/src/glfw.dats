#define ATS_DYNLOADFLAG 0

#include "share/atspre_staload.hats"
#include "main.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "glfw.sats"
staload "general.sats"

implement glfw_set_error_callback (callback) =
  $extfcall(glfw_error_fun, "glfwSetErrorCallback", callback)

implement glfw_init () =
  let
    val err = ~int2bool($extfcall(int, "glfwInit"))
  in
    if err then GlfwCtxErr() else GlfwCtxOk()
  end

implement glfw_terminate ctx =
  case+ ctx of
    | ~GlfwCtxOk() => $extfcall(void, "glfwTerminate")
    | ~GlfwCtxErr() => ()

implement glfw_window_hint (ctx, hint, value) =
  $extfcall(void, "glfwWindowHint", hint, value)

implement glfw_create_window (ctx, width, height, title, monitor_opt, share_opt) =
  let
    val monitor = case+ monitor_opt of
      | Some(x) => x
      | None()  => the_null_ptr

    val share = case+ share_opt of
      | Some(x) => x
      | None()  => the_null_ptr
  in
    $extfcall(glfw_window, "glfwCreateWindow", width, height, title, monitor, share)
  end

implement glfw_make_context_current (win) =
  $extfcall(void, "glfwMakeContextCurrent", win)
