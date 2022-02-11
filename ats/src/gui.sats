#include "share/atspre_staload.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "glfw.sats"
staload "cimgui.sats"
staload "symintr.sats"

absvt@ype Gui = @(ImGuiContext, ImGuiImplGlfw, ImGuiImplOpenGL3)

fn gui_create (GlfwWindow): Option_vt(Gui)
fn gui_destroy (Gui): void

fn gui_render (!Gui): void
fn gui_update (!Gui): void

overload .destroy with gui_destroy
overload .render  with gui_render
overload .update  with gui_update
