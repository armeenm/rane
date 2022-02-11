#define ATS_DYNLOADFLAG 0

#include "share/atspre_staload.hats"
#include "cimgui.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "cimgui.sats"

assume ImGuiContext     = ptr
assume ImGuiImplGlfw    = unit
assume ImGuiImplOpenGL3 = unit
assume ImGuiWindow      = unit

(*** ImGui Proper ***)

implement ig_create_context shared_font_atlas_opt = let
  val shared_font_atlas = case+ shared_font_atlas_opt of
    | Some(x) => x
    | None() => the_null_ptr

  val p = $extfcall(ImGuiContext, "igCreateContext", shared_font_atlas)
in
  if p = the_null_ptr then None_vt() else Some_vt(p)
end

implement ig_destroy_context ctx = $extfcall(void, "igDestroyContext", ctx)

implement ig_get_io ctx = $extfcall(ImGuiIO, "igGetIO")

implement ig_style_colors_dark (ctx, dst_opt) = let
  val dst = case+ dst_opt of
    | Some(x) => x
    | None() => the_null_ptr
in
  $extfcall(void, "igStyleColorsDark", dst)
end

implement ig_render ctx =
  $extfcall(void, "igRender")

implement ig_get_draw_data ctx =
  $extfcall(ImDrawData, "igGetDrawData")

implement ig_new_frame ctx =
  $extfcall(void, "igNewFrame")

implement ig_begin (ctx, name, p_open, flags) = let
  val _ = $extfcall(bool, "igBegin", name, p_open, flags)
in
  unit()
end

implement ig_text (win, str) =
  $extfcall(void, "igText", str)

implement ig_end _ =
  $extfcall(void, "igEnd")

implement ig_show_demo_window (ctx, p_open) =
  $extfcall(void, "igShowDemoWindow", p_open)

(*** GLFW Impl ***)

implement igimpl_glfw_init_for_opengl (ctx, win, install_callbacks) = let
  val success = $extfcall(bool, "ImGui_ImplGlfw_InitForOpenGL", win, install_callbacks)
in
  if success then Some_vt(unit()) else None_vt()
end

implement igimpl_glfw_shutdown _ = $extfcall(void, "ImGui_ImplGlfw_Shutdown")

implement igimpl_glfw_new_frame igimpl_glfw = $extfcall(void, "ImGui_ImplGlfw_NewFrame")

(*** OpenGL3 Impl ***)

implement igimpl_opengl3_init (impl, glsl_version) = let
  val success = $extfcall(bool, "ImGui_ImplOpenGL3_Init", glsl_version)
in
  if success then Some_vt(unit()) else None_vt()
end

implement igimpl_opengl3_shutdown _ = $extfcall(void, "ImGui_ImplOpenGL3_Shutdown")

implement igimpl_opengl3_render_draw_data (impl, draw_data) =
  $extfcall(void, "ImGui_ImplOpenGL3_RenderDrawData", draw_data)

implement igimpl_opengl3_new_frame impl =
  $extfcall(void, "ImGui_ImplOpenGL3_NewFrame")
