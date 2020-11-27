/*
  ==============================================================================

    Crumpling.cpp
    Created: 27 Nov 2020 12:01:02pm
    Author:  Giova

  ==============================================================================
*/

#include "Crumpling.h"

Crumpling::Crumpling()
{
    crumpling = SDTCrumpling_new();
}

Crumpling::~Crumpling()
{
}

void Crumpling::setCrumplingParameters(double crushingEnergy, double granularity, double fragmentation)
{
    SDTCrumpling_setCrushingEnergy(crumpling, crushingEnergy);
    SDTCrumpling_setGranularity(crumpling, granularity);
    SDTCrumpling_setFragmentation(crumpling, fragmentation);

}

SDTCrumpling* Crumpling::getSDTObj()
{
    return this->crumpling;
}

