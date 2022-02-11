#define ATS_DYNLOADFLAG 0

#include "share/atspre_staload.hats"
#include "glew.hats"

staload "glew.sats"

assume Gl = unit

implement glew_init win = let
  val err = $extfcall(bool, "glewInit")
in
  if err then None() else Some(unit())
end

implement gl_viewport (_, x, y, width, height) =
  $extfcall(void, "glViewport", x, y, width, height)

implement gl_clear (_, mask) =
  $extfcall(void, "glClear", mask)

implement gl_clear_color (_, red, green, blue, alpha) =
  $extfcall(void, "glClearColor", red, green, blue, alpha)
