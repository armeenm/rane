#include "share/atspre_staload.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "glfw.sats"
staload "symintr.sats"
staload "general.sats"

absvt@ype ImGuiContext     = ptr
absvt@ype ImGuiImplGlfw    = unit
absvt@ype ImGuiImplOpenGL3 = unit
absvt@ype ImGuiWindow      = unit
abst@ype  ImFontAtlas      = ptr
abst@ype  ImGuiIO          = ptr
abst@ype  ImGuiStyle       = ptr
abst@ype  ImDrawData       = ptr

typedef ImGuiWindowFlags = int

fn ig_create_context (shared_font_atlas: Option(ImFontAtlas)): Option_vt(ImGuiContext)
fn ig_destroy_context (ImGuiContext): void

fn ig_get_io (!ImGuiContext): ImGuiIO
fn ig_style_colors_dark (!ImGuiContext, Option(ImGuiStyle)): void
fn ig_render (!ImGuiContext): void
fn ig_get_draw_data (!ImGuiContext): ImDrawData
fn ig_new_frame (!ImGuiContext): void
fn ig_begin
  (!ImGuiContext, name: string, p_open: ptr, ImGuiWindowFlags):
  ImGuiWindow
fn ig_text (!ImGuiWindow, string): void
fn ig_button (!ImGuiWindow): void
fn ig_end (ImGuiWindow): void
fn ig_show_demo_window (!ImGuiContext, p_open: ptr): void

fn igimpl_glfw_init_for_opengl (!ImGuiContext, GlfwWindow, install_callbacks: bool):
  Option_vt(ImGuiImplGlfw)
fn igimpl_glfw_shutdown (ImGuiImplGlfw): void

fn igimpl_glfw_new_frame (!ImGuiImplGlfw): void

fn igimpl_opengl3_init (!ImGuiImplGlfw, glsl_version: string): Option_vt(ImGuiImplOpenGL3)
fn igimpl_opengl3_shutdown (ImGuiImplOpenGL3): void

fn igimpl_opengl3_render_draw_data (!ImGuiImplOpenGL3, ImDrawData): void
fn igimpl_opengl3_new_frame (!ImGuiImplOpenGL3): void

overload .destroy           with ig_destroy_context
overload .destroy           with ig_end
overload .destroy           with igimpl_glfw_shutdown
overload .destroy           with igimpl_opengl3_shutdown

overload .io                with ig_get_io
overload .style_colors_dark with ig_style_colors_dark
overload .render            with ig_render
overload .draw_data         with ig_get_draw_data
overload .new_frame         with ig_new_frame
overload .window            with ig_begin
overload .show_demo_window  with ig_show_demo_window

overload .text              with ig_text
overload .button            with ig_button

overload .init_glfw_opengl  with igimpl_glfw_init_for_opengl
overload .new_frame         with igimpl_glfw_new_frame

overload .init_opengl3      with igimpl_opengl3_init
overload .render_draw_data  with igimpl_opengl3_render_draw_data
overload .new_frame         with igimpl_opengl3_new_frame
