#include "rane/rane.h"

#include <spdlog/spdlog.h>

int main() {
  try {
    spdlog::set_level(spdlog::level::debug);

    auto const rane = Rane{};

    while (!rane.done())
      rane.loop();

  } catch (std::exception const& e) {
    spdlog::error(e.what());
  }
}
