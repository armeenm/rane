#include "share/atspre_staload.hats"

staload "glfw.sats"
staload "symintr.sats"

macdef GL_COLOR_BUFFER_BIT = $extval(uint, "GL_COLOR_BUFFER_BIT")

abst@ype Gl = unit

fn glew_init (!GlfwWindow): Option(Gl)
fn gl_viewport (Gl, x: Nat, y: Nat, width: Nat, height: Nat): void
fn gl_clear (Gl, mask: uint): void
fn gl_clear_color (Gl, red: float, green: float, blue: float, alpha: float): void

overload .glew_init   with glew_init
overload .viewport    with gl_viewport
overload .clear       with gl_clear
overload .clear_color with gl_clear_color
