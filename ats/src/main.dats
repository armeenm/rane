#include "share/atspre_staload.hats"
#include "glfw.hats"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

staload "libats/libc/SATS/stdlib.sats"
staload "glfw.sats"
staload "glew.sats"
staload "cimgui.sats"
staload "gui.sats"
staload "general.sats"
staload "main.sats"

implement main0 () = let
  (* GLFW Init *)
  val () = println!("Initializing...")
  val _ = glfw_set_error_callback(error_callback)
  val glfw_opt = glfw_init()
in (
  (case+ glfw_opt of
  | ~Some_vt(glfw) => let
      (* GLEW Init *)
      val () = println!("GLFW init succeeded!");
      val () = glfw.window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      val () = glfw.window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
      val win = glfw.create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "RANE", None(), None())
      val () = win.make_current()
      val gl_opt = win.glew_init()
    in (
      (case+ gl_opt of
      | Some(gl) =>
        let
          val () = println!("GLEW init succeeded!")
          val gui_opt = gui_create(win)
        in
          (case+ gui_opt of
          | ~Some_vt(gui) =>
            let
              val () = println!("GUI init succeeded!")
              val () = gl.viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
              val () = loop(glfw, gl, gui, win)
            in
              gui.destroy()
            end

          | ~None_vt() => prerrln!("GUI init failed!")
          )
        end

      | None() => prerrln!("GLEW init failed!")
      );

      glfw.destroy()
    ) end

  | ~None_vt() => prerrln!("GLFW init failed!")
  );

) end

implement error_callback (errc, errstr) = prerrln!("Error ", errc, ": ", errstr)

implement loop (glfw, gl, gui, win) = let
  val () = glfw.poll_events()
  val () = gui.update()
  val (width, height) = win.window_size()
  val () = gl.viewport(0, 0, width, height)
  val () = gl.clear(GL_COLOR_BUFFER_BIT)
  val () = gl.clear_color(0.0f, 0.0f, 0.0f, 0.0f)
  val () = gui.render()
  val () = win.swap_buffers()
in
  if win.should_close() then () else loop(glfw, gl, gui, win)
end
