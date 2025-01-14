#pragma once

#include <cmath>
#include <vector>

namespace masy
{
class Osc {
  public:
    int sampleRate;
    float angle = 0.0;
    float delta = 1;
    float output = 0.0;
    float volume = 1;

    Osc(int sampleRate) : sampleRate(sampleRate)
    {
        setFrequency(440);
    }

    void setFrequency(float frequency)
    {
        delta = 2.0 * M_PI * frequency / sampleRate;
    }

    void resetAngle()
    {
        angle = fmod(angle, 2.0 * M_PI);
    }

    void evaluateSample()
    {
        output = volume * sinf(angle);
        angle += delta;
    }
};
} // namespace masy
