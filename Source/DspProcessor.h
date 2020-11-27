/*
  ==============================================================================

    DspProcessor.h
    Created: 27 Nov 2020 4:00:33pm
    Author:  Giova

    The processing is done by retriving the displacement of the modal resonator
    in the first pickup point. The processing is a sample-by-sample processing.
    The only processing information needed by the library is the sample rate.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SDTCommon.h"
#include "SDTSolids.h"
#include "Impact.h"
#include "Modal.h"
#include "Inertial.h"

class DspProcessor
{
public:
    DspProcessor(char* key0, char* key1);
    ~DspProcessor();

    void setSampleRate(double sampleRate);
    double process();
    void reset();

    std::unique_ptr<Inertial> inertialResonator;
    std::unique_ptr<Modal> modalResonator;
    std::unique_ptr<Impact> impactModel;

private:
    int nModes{ 3 };
    int nPickups{ 2 };

    double tmpOuts[2 * SDT_MAX_PICKUPS]; /*array where the velocity and displacent information
                                         of the modal resonator in the two pickup points are
                                         stored*/

    double f{ 0.0 };
    

};
