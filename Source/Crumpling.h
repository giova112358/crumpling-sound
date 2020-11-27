/*
  ==============================================================================

    Crumpling.h
    Created: 27 Nov 2020 12:01:02pm
    Author:  Giova

  ==============================================================================
*/

#pragma once
#include "SDTCommon.h"
#include "SDTControl.h"

class Crumpling
{
public:
    Crumpling();
    ~Crumpling();

    void setCrumplingParameters(double crushingEnergy, double granularity, double fragmentation);

    SDTCrumpling* getSDTObj();

private:
    SDTCrumpling* crumpling;
};