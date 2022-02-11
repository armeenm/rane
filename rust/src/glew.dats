#define ATS_DYNLOADFLAG 0

#include "share/atspre_staload.hats"
#include "main.hats"

staload "glew.sats"

implement glew_init (pf | _) = $extfcall(void, "glewInit")
