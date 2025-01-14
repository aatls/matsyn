#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

namespace masy
{
struct Mixer
{
    float lVolume = 1;
    float rVolume = 1;
    float mainVolume = 1;
    float pan = 0;
    float lIn = 0;
    float rIn = 0;
    float LRIn = 0;
    float lOutput = 0;
    float rOutput = 0;

    void evaluate()
    {
        const float clampedPan = std::clamp(pan, -1.0f, 1.0f);
        const float angle = 0.25f * M_PI * (clampedPan + 1.0); // Map pan to [0, π/2]
        const float lPan = cosf(angle);
        const float rPan = sinf(angle);

        constexpr const float safetyConstant = 0.25; // Is this necessary?

        lOutput = (lIn + LRIn) * lPan * mainVolume * lVolume * safetyConstant;
        rOutput = (rIn + LRIn) * rPan * mainVolume * rVolume * safetyConstant;
    }
};
} // namespace masy
