#pragma once

#include <vector>

#include "Mixer.hpp"
#include "Osc.hpp"

namespace masy
{
class ModuleManager
{
  public:
    int sampleRate;
    masy::Mixer mixer;
    std::vector<masy::Osc> oscillators;

    ModuleManager() {}

    ModuleManager(int countPerModule, int sampleRate) : sampleRate(sampleRate)
    {
        for (int i = 0; i < countPerModule; i++)
        {
            oscillators.push_back(masy::Osc(this->sampleRate));
        }
    }

    void evaluateModules()
    {
        for (auto& osc : oscillators)
        {
            osc.evaluateSample();
        }
        mixer.evaluate();
    }

    void cleanUp()
    {
        for (auto& osc : oscillators)
        {
            osc.resetAngle();
        }
    }
};
} // namespace masy
