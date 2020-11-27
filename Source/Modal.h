/*
  ==============================================================================

    Modal.h
    Created: 27 Nov 2020 12:00:25pm
    Author:  Giova

    Physical model of a set of parallel mass–spring–damper mechanical oscillators,
    with each oscillator representing a resonant frequency of the object.
    Set the parameters of the modal resonator.
  ==============================================================================
*/

#pragma once
#include "SDTCommon.h"
#include "SDTSolids.h"

class Modal
{
public:
    Modal(char* key, int nModes, int nPickups);
    ~Modal();

    void setModalParameters(double freqs[SDT_MAX_MODES], double decays[SDT_MAX_MODES],
        double gains[SDT_MAX_PICKUPS][SDT_MAX_MODES], double fragmentSize,
        long activeModes);

    SDTResonator* getSDTObj();

private:
    SDTResonator* modal;  /*modal resonator object*/
    int nModes;
    int nPickups;
};
