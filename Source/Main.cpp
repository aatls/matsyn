/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "App.hpp"
#include "ConnectionManager.hpp"
#include "ModuleManager.hpp"

//==============================================================================
int main (int argc, char* argv[])
{
    juce::ScopedJuceInitialiser_GUI libraryInitialiser;
    juce::ConsoleApplication app;

    syna::ConnectionManager connections(10);
    syna::ModuleManager modules(10, 44100);

    // This starts audio callback
    syna::App<false> syna(connections, modules);

    // Wait forever
    std::promise<void>().get_future().wait();

    return 0;
}
