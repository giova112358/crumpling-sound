/*
  ==============================================================================

    Inertial.h
    Created: 27 Nov 2020 12:00:13pm
    Author:  Giova

    The inertial resonator simulates an inertial point mass used as an exciter for
    the modal resonator.
    Set the parameter of the inertial resonator, set the position and velocity of
    the inertial point mass.
  ==============================================================================
*/

#pragma once
#include "SDTCommon.h"
#include "SDTSolids.h"


class Inertial
{
public:
    Inertial(char* key);
    ~Inertial();

    void setInertialParameters(double mass, double fragmentSize);
    void setStrike(double position, double velocity);

    SDTResonator* getSDTObj();

private:
    SDTResonator* inertial; /*inertial resonator object*/
};