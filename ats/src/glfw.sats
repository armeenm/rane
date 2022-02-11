#include "share/atspre_staload.hats"

staload "libats/libc/SATS/stdlib.sats"
staload "symintr.sats"

macdef GLFW_CONTEXT_VERSION_MAJOR = $extval(int, "GLFW_CONTEXT_VERSION_MAJOR")
macdef GLFW_CONTEXT_VERSION_MINOR = $extval(int, "GLFW_CONTEXT_VERSION_MINOR")

typedef glfw_error_fun = (int, string) -> void

absvt@ype Glfw        = unit
abst@ype  GlfwMonitor = ptr
abst@ype  GlfwWindow  = ptr

fn glfw_set_error_callback (glfw_error_fun): glfw_error_fun

fn glfw_init (): Option_vt(Glfw)
fn glfw_terminate (Glfw): void

fn glfw_window_hint (!Glfw, hint: int, value: int): void
fn glfw_poll_events (!Glfw): void
fn glfw_create_window
  ( !Glfw
  , width:   Nat
  , height:  Nat
  , title:   string
  , monitor: Option(GlfwMonitor)
  , share:   Option(GlfwWindow)
  ): GlfwWindow

fn glfw_make_context_current (GlfwWindow): void
fn glfw_get_window_size (GlfwWindow): (Nat, Nat)
fn glfw_window_should_close (GlfwWindow): bool
fn test (): bool
fn glfw_swap_buffers (GlfwWindow): void

overload .destroy       with glfw_terminate

overload .window_hint   with glfw_window_hint
overload .poll_events   with glfw_poll_events
overload .create_window with glfw_create_window

overload .make_current  with glfw_make_context_current
overload .window_size   with glfw_get_window_size
overload .should_close  with glfw_window_should_close
overload .swap_buffers  with glfw_swap_buffers
