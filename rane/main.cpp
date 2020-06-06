#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "rane/glfw.h"
#include "rane/rane.h"
#include "rane/vulkan.h"

#include <fmt/core.h>
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
