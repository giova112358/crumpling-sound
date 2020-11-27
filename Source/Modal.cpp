/*
  ==============================================================================

    Modal.cpp
    Created: 27 Nov 2020 12:00:25pm
    Author:  Giova

  ==============================================================================
*/

#include "Modal.h"

Modal::Modal(char* key, int nModes, int nPickups)
{
    this->nModes = nModes; this->nPickups = nPickups;
    modal = SDTResonator_new(nModes, nPickups);

    if (SDT_registerResonator(modal, key)) {
        SDTResonator_free(modal);
        //print error("sdt.inertial: Error registering the resonator. Probably a duplicate id?")
    }
}

Modal::~Modal()
{
}

void Modal::setModalParameters(double freqs[SDT_MAX_MODES],
    double decays[SDT_MAX_MODES], double gains[SDT_MAX_PICKUPS][SDT_MAX_MODES],
    double fragmentSize, long activeModes)
{
    for (int mode = 0; mode < nModes; mode++) {
        SDTResonator_setFrequency(modal, mode, freqs[mode]);
        SDTResonator_setDecay(modal, mode, decays[mode]);
        SDTResonator_setWeight(modal, mode, 1.0);
        for (int pickup = 0; pickup < nPickups; pickup++) {
            SDTResonator_setGain(modal, pickup, mode, gains[pickup][mode]);
        }
    }
    SDTResonator_setFragmentSize(modal, fragmentSize);
    SDTResonator_setActiveModes(modal, activeModes);
}

SDTResonator* Modal::getSDTObj()
{
    return this->getSDTObj;
}
