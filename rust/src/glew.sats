#include "share/atspre_staload.hats"

staload "glfw.sats"

fn glew_init {l: addr} (!glfw_window @ l | unit): void
