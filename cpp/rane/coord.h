#pragma once

template <typename T> struct Coord {
  T x, y;

  constexpr Coord(T x_in, T y_in) : x{x_in}, y{y_in} {}
};
