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

    void setCrushingEnergy(double crushingEnergy);
    void setGranularity(double granularity);
    void setFragmentation(double fragmentation);

    SDTCrumpling* getSDTObj();

private:
    SDTCrumpling* crumpling;
};