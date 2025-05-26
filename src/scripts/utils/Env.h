#ifndef ENV
#define ENV

#include <cstdint>

namespace Env {
    inline constexpr uint16_t windowWidth = 640u;
    inline constexpr uint16_t windowHeight = 480u;

    inline constexpr uint16_t gameViewportPositionX = 32u;
    inline constexpr uint16_t gameViewportPositionY = 16u;

    inline constexpr uint16_t gameViewportWidth = 384u;
    inline constexpr uint16_t gameViewportHeight = 448u;
}

#endif // ENV