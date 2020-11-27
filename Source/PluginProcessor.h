/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspProcessor.h"

//==============================================================================
/**
*/
class CrumplingModelAudioProcessor  : public juce::AudioProcessor,
    juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    CrumplingModelAudioProcessor();
    ~CrumplingModelAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void init(); /*initialize the two impact models and register the interactions
                 called at the constructor of the PluginProcessor */

                 //==============================================================================
    void prepare(double sampleRate); /*set the processing parameters of the two
                                     impact models*/

                                     //==============================================================================
                                     /*update methods called when a parameter is changed in the GUI*/
                                     //==============================================================================
    void updateVolume();
    void updateInertialParameters();
    void updateModalParameters();
    void updateImpactParameters();

    //==============================================================================
    void reset() override;

    //==============================================================================
    void strike(); /*set the position and velocity parameter of the inertial point
                   mass */

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    void addImpactParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    void addInertialParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    void addModalParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    void addGainParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    bool isActive{ false };
    bool mustUpdateVolume{ false };
    bool mustUpdateInertialParameters{ false };
    bool mustUpdateModalParameters{ false };
    bool mustUpdateImpactParameters{ false };
    bool mustStrike{ false };

    static constexpr int numChannels = 2;

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    juce::LinearSmoothedValue<float> mVolume[2]{ 0.0 };

    std::unique_ptr<DspProcessor> model[numChannels];
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrumplingModelAudioProcessor)
};
