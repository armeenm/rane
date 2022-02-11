#include "share/atspre_staload.hats"

staload "libats/libc/SATS/stdlib.sats"

vtypedef tptr (a:t@ype, l: addr) = (a @ l | ptr l)
