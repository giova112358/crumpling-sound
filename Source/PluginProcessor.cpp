/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrumplingModelAudioProcessor::CrumplingModelAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    apvts.addParameterListener("VOL", this);
    apvts.addParameterListener("VEL", this);
    apvts.addParameterListener("MASS", this);
    apvts.addParameterListener("FOR", this);
    apvts.addParameterListener("FREQ0", this);
    apvts.addParameterListener("FREQ1", this);
    apvts.addParameterListener("FREQ2", this);
    apvts.addParameterListener("DEC0", this);
    apvts.addParameterListener("DEC1", this);
    apvts.addParameterListener("DEC2", this);
    apvts.addParameterListener("GAINPICK10", this);
    apvts.addParameterListener("GAINPICK11", this);
    apvts.addParameterListener("GAINPICK12", this);
    apvts.addParameterListener("STIFF", this);
    apvts.addParameterListener("DISS", this);
    apvts.addParameterListener("SH", this);
    apvts.addParameterListener("BANG", this);

    init();
}

CrumplingModelAudioProcessor::~CrumplingModelAudioProcessor()
{
}

//==============================================================================
const juce::String CrumplingModelAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CrumplingModelAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CrumplingModelAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CrumplingModelAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CrumplingModelAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CrumplingModelAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CrumplingModelAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CrumplingModelAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CrumplingModelAudioProcessor::getProgramName (int index)
{
    return {};
}

void CrumplingModelAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CrumplingModelAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    prepare(sampleRate);
    updateVolume();
    updateInertialParameters();
    updateModalParameters();
    updateImpactParameters();
    reset();
    isActive = true;
}

void CrumplingModelAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CrumplingModelAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CrumplingModelAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!isActive)
        return;
    if (mustUpdateVolume)
        updateVolume();
    if (mustUpdateInertialParameters)
        updateInertialParameters();
    if (mustUpdateModalParameters)
        updateModalParameters();
    if (mustUpdateImpactParameters)
        updateImpactParameters();
    if (mustStrike)
        strike();

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            channelData[sample] = 1000 * model[channel]->process();
            //channelData[sample] = juce::jlimit(-1.0f, 1.0f, channelData[sample]);
        }

        mVolume[channel].applyGain(channelData, numSamples);
    }
}

//==============================================================================
bool CrumplingModelAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CrumplingModelAudioProcessor::createEditor()
{
    /*return new CrumplingModelAudioProcessorEditor (*this);*/
    return new juce::GenericAudioProcessorEditor(this);
}

//==============================================================================
void CrumplingModelAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CrumplingModelAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CrumplingModelAudioProcessor();
}

void CrumplingModelAudioProcessor::init()
{
    char ine1[10] = { "hammercr0" };
    char mod1[10] = { "objectcr0" };
    char ine2[10] = { "hammercr1" };
    char mod2[10] = { "objectcr1" };

    model[0] = std::make_unique<DspProcessor>(ine1, mod1);
    model[1] = std::make_unique<DspProcessor>(ine2, mod2);
}

void CrumplingModelAudioProcessor::prepare(double sampleRate)
{
    for (int channel = 0; channel < numChannels; ++channel) {
        model[channel]->setSampleRate(sampleRate);
    }
}

//===============================================================================
void CrumplingModelAudioProcessor::reset()
{
    for (int channel = 0; channel < numChannels; ++channel) {
        mVolume[channel].reset(getSampleRate(), 0.050);
    }
}
//===============================================================================
void CrumplingModelAudioProcessor::addImpactParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    std::function<juce::String(float, int)> valueToTextFunction =
        [](float x, int l) {return juce::String(x, 4);  };
    std::function<float(const juce::String&)> textToValueFunction = 
        [](const juce::String& str) {return str.getFloatValue(); };

    auto stiffness = std::make_unique<juce::AudioParameterFloat>("STIFF", "Stiffness", 
        juce::NormalisableRange<float>(1000.0, 20000000000.0), 37396, "k", 
        juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto shape = std::make_unique<juce::AudioParameterFloat>("SH", "Shape",
        juce::NormalisableRange<float>(1.0, 4.0), 1.8055,
        "alpha", juce::AudioProcessorParameter::genericParameter, valueToTextFunction, textToValueFunction);

    auto dissipation = std::make_unique<juce::AudioParameterFloat>("DISS", "Dissipation",
        juce::NormalisableRange<float>(0.00001, 1.0), 0.5092, "mu", 
        juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("sdt.impact", "IMPACT PARAMETERS", "|",
        std::move(stiffness), std::move(shape), std::move(dissipation));

    layout.add(std::move(group));
}

void CrumplingModelAudioProcessor::addInertialParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    std::function<juce::String(float, int)> valueToTextFunction =
        [](float x, int l) {return juce::String(x, 4);  };
    std::function<float(const juce::String&)> textToValueFunction =
        [](const juce::String& str) {return str.getFloatValue(); };

    auto velocity = std::make_unique<juce::AudioParameterFloat>("VEL", "Velocity",
        juce::NormalisableRange<float>(0.001, 40.0), 3.741,
        "m/s", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto mass = std::make_unique<juce::AudioParameterFloat>("MASS", "Mass", 
        juce::NormalisableRange<float>(0.001, 0.26), 0.001047,
        "Kg", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto force = std::make_unique<juce::AudioParameterFloat>("FOR", "Force", 
        juce::NormalisableRange<float>(0.0, 0.2), 0.0,
        "N", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("sdt.inertial", "HAMMER CONTROLS", "|",
        std::move(velocity), std::move(mass), std::move(force));

    layout.add(std::move(group));
}

void CrumplingModelAudioProcessor::addModalParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    std::function<juce::String(float, int)> valueToTextFunction = [](float x, int l) {return juce::String(x, 4);  };
    std::function<float(const juce::String&)> textToValueFunction = [](const juce::String& str) {return str.getFloatValue(); };

    auto freq0 = std::make_unique<juce::AudioParameterFloat>("FREQ0", "Frequency0",
        juce::NormalisableRange<float>(20.0, 5000), 500,
        "Hz", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto freq1 = std::make_unique<juce::AudioParameterFloat>("FREQ1", "Frequency1", 
        juce::NormalisableRange<float>(20.0, 5000), 509,
        "Hz", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto freq2 = std::make_unique<juce::AudioParameterFloat>("FREQ2", "Frequency2",
        juce::NormalisableRange<float>(20.0, 5000), 795,
        "Hz", juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto dec0 = std::make_unique<juce::AudioParameterFloat>("DEC0", "Decay0", 
        juce::NormalisableRange<float>(0.0, 1.0), 0.8,
        "", juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto dec1 = std::make_unique<juce::AudioParameterFloat>("DEC1", "Decay1",
        juce::NormalisableRange<float>(0.0, 1.0), 0.6,
        "", juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto dec2 = std::make_unique<juce::AudioParameterFloat>("DEC2", "Decay2", 
        juce::NormalisableRange<float>(0.0, 1.0), 1.0,
        "", juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto gain10 = std::make_unique<juce::AudioParameterFloat>("GAINPICK10", "Gain Pickup1 Mode0",
        juce::NormalisableRange<float>(10, 100), 60,
        "", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto gain11 = std::make_unique<juce::AudioParameterFloat>("GAINPICK11", "Gain Pickup1 Mode1", 
        juce::NormalisableRange<float>(10, 100), 40,
        "", juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto gain12 = std::make_unique<juce::AudioParameterFloat>("GAINPICK12", "Gain Pickup1 Mode2",
        juce::NormalisableRange<float>(10, 100), 30,
        "", juce::AudioProcessorParameter::genericParameter, 
        valueToTextFunction, textToValueFunction);

    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("sdt.modal", "RESONATOR CONTROLS", "|",
        std::move(freq0), std::move(freq1), std::move(freq2),
        std::move(dec0), std::move(dec1), std::move(dec2), std::move(gain10),
        std::move(gain11), std::move(gain12));

    layout.add(std::move(group));
}

void CrumplingModelAudioProcessor::addGainParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    std::function<juce::String(float, int)> valueToTextFunction = [](float x, int l) {return juce::String(x, 4);  };
    std::function<float(const juce::String&)> textToValueFunction = [](const juce::String& str) {return str.getFloatValue(); };

    auto bang = std::make_unique<juce::AudioParameterBool>("BANG", "bang", false);

    auto volume = std::make_unique<juce::AudioParameterFloat>("VOL", "Volume",
        juce::NormalisableRange< float >(-40.0f, 10.0f), 0.0f,
        "dB", juce::AudioProcessorParameter::genericParameter,
        valueToTextFunction, textToValueFunction);

    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("sdt.gain", "VOLUME CONTROLS", "|",
        std::move(volume), std::move(bang));

    layout.add(std::move(group));

}

juce::AudioProcessorValueTreeState::ParameterLayout CrumplingModelAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    CrumplingModelAudioProcessor::addImpactParameters(layout);
    CrumplingModelAudioProcessor::addModalParameters(layout);
    CrumplingModelAudioProcessor::addInertialParameters(layout);
    CrumplingModelAudioProcessor::addGainParameters(layout);
    return layout;
}

//===============================================================================
void CrumplingModelAudioProcessor::updateVolume()
{
    mustUpdateVolume = false;

    auto volume = apvts.getRawParameterValue("VOL");

    for (int channel = 0; channel < numChannels; ++channel) {
        mVolume[channel].setTargetValue(juce::Decibels::decibelsToGain(volume->load()));
    }
}

void CrumplingModelAudioProcessor::updateInertialParameters()
{
    mustUpdateInertialParameters = false;
    auto m = apvts.getRawParameterValue("MASS");
    //auto vel = apvts.getRawParameterValue("VEL");
    auto f = apvts.getRawParameterValue("FOR");

    double mass = m->load();
    //double velocity = vel->load();
    double force = f->load();

    for (int channel = 0; channel < numChannels; ++channel) {
        model[channel]->inertialResonator->setInertialParameters(mass, 1.0);
        /*model[channel]->setExtenalForce(-1 * force);*/
        /*model[channel]->setStrike(0.0, -1 * velocity);*/
    }
    //strike();
}

void CrumplingModelAudioProcessor::updateModalParameters()
{
    mustUpdateModalParameters = false;
    auto freq0 = apvts.getRawParameterValue("FREQ0");
    auto freq1 = apvts.getRawParameterValue("FREQ1");
    auto freq2 = apvts.getRawParameterValue("FREQ2");
    auto dec0 = apvts.getRawParameterValue("DEC0");
    auto dec1 = apvts.getRawParameterValue("DEC1");
    auto dec2 = apvts.getRawParameterValue("DEC2");
    auto gain10 = apvts.getRawParameterValue("GAINPICK10");
    auto gain11 = apvts.getRawParameterValue("GAINPICK11");
    auto gain12 = apvts.getRawParameterValue("GAINPICK12");

    double freqs[SDT_MAX_MODES]{ freq0->load(), freq1->load(), freq2->load() };
    double decays[SDT_MAX_MODES]{ dec0->load(), dec1->load(), dec2->load() };
    double gains[SDT_MAX_PICKUPS][SDT_MAX_MODES]{ {gain10->load(), gain11->load(), gain12->load()}, {0.0, 0.0, 0.0} };

    for (int channel = 0; channel < numChannels; ++channel) {
        model[channel]->modalResonator->setModalParameters(freqs, decays, gains, 1.0, 3);
    }
    //strike();
}

void CrumplingModelAudioProcessor::updateImpactParameters()
{
    mustUpdateImpactParameters = false;
    auto stiff = apvts.getRawParameterValue("STIFF");
    auto sh = apvts.getRawParameterValue("SH");
    auto diss = apvts.getRawParameterValue("DISS");

    float stiffness = stiff->load();
    double shape = sh->load();
    double dissipation = diss->load();

    for (int channel = 0; channel < numChannels; ++channel) {
        model[channel]->impactModel->setImpactParameters(stiffness, shape, dissipation, 0, 0);
    }
    //strike();
}

//===============================================================================
void CrumplingModelAudioProcessor::strike()
{
    mustStrike = false;
    auto vel = apvts.getRawParameterValue("VEL");
    double velocity = vel->load();

    for (int channel = 0; channel < numChannels; ++channel) {
        model[channel]->inertialResonator->setStrike(0.0, /*-9.741634*/-1*velocity);
    }

}


void CrumplingModelAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "VOL")
        mustUpdateVolume = true;
    if (parameterID == "VEL")
        mustUpdateInertialParameters = true;
    if (parameterID == "FOR")
        mustUpdateInertialParameters = true;
    if (parameterID == "MASS")
        mustUpdateInertialParameters = true;
    if (parameterID == "FREQ0")
        mustUpdateModalParameters = true;
    if (parameterID == "FREQ1")
        mustUpdateModalParameters = true;
    if (parameterID == "FREQ2")
        mustUpdateModalParameters = true;
    if (parameterID == "DEC0")
        mustUpdateModalParameters = true;
    if (parameterID == "DEC1")
        mustUpdateModalParameters = true;
    if (parameterID == "DEC2")
        mustUpdateModalParameters = true;
    if (parameterID == "GAINPICK10")
        mustUpdateModalParameters = true;
    if (parameterID == "GAINPICK11")
        mustUpdateModalParameters = true;
    if (parameterID == "GAINPICK12")
        mustUpdateModalParameters = true;
    if (parameterID == "STIFF")
        mustUpdateImpactParameters = true;
    if (parameterID == "DISS")
        mustUpdateImpactParameters = true;
    if (parameterID == "SH")
        mustUpdateImpactParameters = true;
    if (parameterID == "BANG")
        mustStrike = true;
}
