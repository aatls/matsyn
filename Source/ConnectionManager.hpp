#pragma once

#include <vector>

namespace masy
{
struct Connection
{
    float* source;
    float* destination;
    float gain;

    Connection(float* source, float* destination, float gain) :
        source(source), destination(destination), gain(gain) {}
};

class ConnectionManager
{
  public:
    std::vector<float> channelsCurr;
    std::vector<float> channelsPrev;
    int numChannels;
    std::vector<Connection> connections;
    float one = 1;

    ConnectionManager() {}

    ConnectionManager(int numChannels) : numChannels(numChannels), 
        channelsCurr(std::vector<float> (numChannels, 0)),
        channelsPrev(std::vector<float> (numChannels, 0)) {}

    void evaluateConnections()
    {
        for (int i = 0; i < numChannels; i++) {
            channelsPrev[i] = channelsCurr[i];
        }

        // zero out the connections before filling them
        for (auto& connection : connections)
        {
            *connection.destination = 0.0;
        }

        for (auto& connection : connections)
        {
            *connection.destination += connection.gain * (*connection.source);
        }
    }

    void connectModuleToChannel(float* source, int channel, float gain)
    {
        setOrEditConnection(source, &channelsCurr[channel], gain);
    }

    // @param channel If channel is -1, source signal is 1.0, else source is the given channel
    void connectChannelToModule(int channel, float* destination, float gain)
    {
        float* source = channel == -1 ? &one : &channelsPrev[channel];
        setOrEditConnection(source, destination, gain);
    }

    void setOrEditConnection(float* source, float* destination, float gain)
    {
        for (auto connection : connections)
        {
            if (connection.source == source &&
                connection.destination == destination)
            {
                connection.gain = gain;
                return;
            }
        }
        connections.push_back(Connection(source, destination, gain));
    }

    // void removeConnection(float* moduleParameter, int channel)
    // {
    //     TODO
    // }
};
} // namespace masy