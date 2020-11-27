/*
  ==============================================================================

    Inertial.cpp
    Created: 27 Nov 2020 12:00:13pm
    Author:  Giova

  ==============================================================================
*/

#include "Inertial.h"

Inertial::Inertial(char* key)
{
    inertial = SDTResonator_new(1, 1);

    if (SDT_registerResonator(inertial, key)) {
        SDTResonator_free(inertial);
        //print error("sdt.inertial: Error registering the resonator. Probably a duplicate id?")
    }
}

Inertial::~Inertial()
{
}

void Inertial::setInertialParameters(double mass, double fragmentSize)
{
    SDTResonator_setFrequency(inertial, 0, 0.0);
    SDTResonator_setDecay(inertial, 0, 0.0);
    SDTResonator_setWeight(inertial, 0, mass);
    SDTResonator_setGain(inertial, 0, 0, 1.0);
    SDTResonator_setFragmentSize(inertial, fragmentSize);
    SDTResonator_setActiveModes(inertial, 1);
}

void Inertial::setStrike(double position, double velocity)
{
    SDTResonator_setPosition(inertial, 0, position);
    SDTResonator_setVelocity(inertial, 0, velocity);
}

SDTResonator* Inertial::getSDTObj()
{
    return this->inertial;
}
