#include "share/atspre_staload.hats"

staload "glfw.sats"
staload "glew.sats"
staload "gui.sats"

fn error_callback (int, string): void = "sta#"
fun loop (!Glfw, Gl, !Gui, GlfwWindow): void
