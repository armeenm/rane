#define ATS_DYNLOADFLAG 0

#include "share/atspre_staload.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "cimgui.sats"
staload "gui.sats"

assume Gui = 
  @{ ig_ctx         = ImGuiContext
   , igimpl_glfw    = ImGuiImplGlfw
   , igimpl_opengl3 = ImGuiImplOpenGL3
   }

implement gui_create win = let
  val ig_ctx_opt = ig_create_context(None())
in
  (case+ ig_ctx_opt of
  | ~Some_vt(ig_ctx) =>
    let
      val glsl_version = "#version 330 core"

      val- ~Some_vt(igimpl_glfw) = ig_ctx.init_glfw_opengl(win, true)
      val- ~Some_vt(igimpl_opengl3) = igimpl_glfw.init_opengl3(glsl_version)
    in
      Some_vt(@{ig_ctx=ig_ctx, igimpl_glfw=igimpl_glfw, igimpl_opengl3=igimpl_opengl3})
    end

  | ~None_vt() => (
    print("ImGui init failed!\n");
    None_vt()
    )
  );
end

implement gui_destroy gui = (
  gui.igimpl_opengl3.destroy();
  gui.igimpl_glfw.destroy();
  gui.ig_ctx.destroy()
)

implement gui_render gui = (
  gui.ig_ctx.render();
  gui.igimpl_opengl3.render_draw_data(gui.ig_ctx.draw_data())
)

implement gui_update gui = let
  val () = gui.igimpl_opengl3.new_frame()
  val () = gui.igimpl_glfw.new_frame()
  val () = gui.ig_ctx.new_frame()
  val win = gui.ig_ctx.window("Test", the_null_ptr, 0)
  val () = win.text("Hello World!")
  val () = win.destroy()
  val () = gui.ig_ctx.show_demo_window(the_null_ptr)
in
end
