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

void Crumpling::setCrushingEnergy(double crushingEnergy)
{
    SDTCrumpling_setCrushingEnergy(crumpling, crushingEnergy);
}

void Crumpling::setGranularity(double granularity)
{
    SDTCrumpling_setGranularity(crumpling, granularity);
}

void Crumpling::setFragmentation(double fragmentation)
{
    SDTCrumpling_setFragmentation(crumpling, fragmentation);
}

SDTCrumpling* Crumpling::getSDTObj()
{
    return this->crumpling;
}

