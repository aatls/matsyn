#pragma once

#include <vector>
#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"

#include "ConnectionManager.hpp"
#include "ModuleManager.hpp"

#define d(msg) if (debug) std::cerr << msg << "\n" // for debugging

namespace masy
{
template<bool debug>
class App : public juce::AudioIODeviceCallback
{
  public:
    App(ConnectionManager& connectionManager, ModuleManager& moduleManager) :
        connectionManager(connectionManager),
        moduleManager(moduleManager)
    {
        juce::String error = deviceManager.initialiseWithDefaultDevices(0, 2);
        if (error.isNotEmpty())
        {
            std::cerr << "Audio device error: " << error << std::endl;
        }

        deviceManager.addAudioCallback(this);
    }

    ~App() override
    {
        deviceManager.removeAudioCallback(this);
        deviceManager.closeAudioDevice();
    }

    void fillBuffer()
    {
        for (int i = 0; i < bufferSize; i++)
        {
            moduleManager.evaluateModules();
            connectionManager.evaluateConnections();

            buffer[0][i] = moduleManager.mixer.lOutput;
            buffer[1][i] = moduleManager.mixer.rOutput;
        }

        moduleManager.cleanUp();
    }

    void audioDeviceIOCallbackWithContext(const float *const *inputChannelData,
                                          int numInputChannels,
                                          float *const *outputChannelData,
                                          int numOutputChannels,
                                          int numSamples,
                                          const juce::AudioIODeviceCallbackContext &context) override
    {
        juce::ignoreUnused(inputChannelData, numInputChannels, context);

        fillBuffer();

        for (auto channel = 0; channel < numOutputChannels; channel++)
        {
            auto* out = outputChannelData[channel];
            for (auto sample = 0; sample < numSamples; sample++)
            {
                out[sample] = buffer[channel][sample];
            }
        }
    }

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override
    {
        bufferSize = device->getCurrentBufferSizeSamples();
        sampleRate = device->getCurrentSampleRate();

        buffer.push_back(std::vector<float>(bufferSize, 0)); // Left channel
        buffer.push_back(std::vector<float>(bufferSize, 0)); // Right channel




        // ---------------------------- TEST PATCH TEST --------------------------------
        // Implement static test patches here

        
    }

    void audioDeviceStopped() override {}

  private:
    juce::AudioDeviceManager deviceManager;
    std::vector<std::vector<float>> buffer;
    int bufferSize;
    int sampleRate;

    masy::ConnectionManager connectionManager;
    masy::ModuleManager moduleManager;

};
} // namespace masy