#include "share/atspre_staload.hats"
#include "main.hats"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

staload "libats/libc/SATS/stdlib.sats"
staload "glew.sats"
staload "glfw.sats"
staload "general.sats"
staload "main.sats"

implement error_callback (errc, errstr) =
  println!("Error ", errc, ": ", errstr)

implement main0 () = (
  print "Initializing...\n";
  
  let
    val _ = glfw_set_error_callback(error_callback)
    val glfw_ctx = glfw_init()
  in (
  /*
    (case+ glfw_ctx of
    | GlfwCtxOk() => let
        val () = print "Success!\n"
        val () = glfw_window_hint(glfw_ctx, GLFW_CONTEXT_VERSION_MAJOR, 3)
        val () = glfw_window_hint(glfw_ctx, GLFW_CONTEXT_VERSION_MINOR, 3)
        var win = glfw_create_window(glfw_ctx, WINDOW_WIDTH, WINDOW_HEIGHT, "RANE", None(), None())
        val () = glfw_make_context_current(win)
        val () = glew_init(view@(win) | unit())
      in
      end
    | GlfwCtxErr() => print "GLFW init failed!\n"
    );
  */

    if 3 = 3 then print "Wow\n";
    (case+ glfw_ctx of
     | GlfwCtxOk() => glfw_window_hint(glfw_ctx, GLFW_CONTEXT_VERSION_MAJOR, 3)
     | GlfwCtxErr() => print "Oops\n"
    );

    glfw_terminate(glfw_ctx);
    print "Done!\n"
  ) end
)
