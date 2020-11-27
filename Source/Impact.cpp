/*
  ==============================================================================

    Impact.cpp
    Created: 27 Nov 2020 12:00:35pm
    Author:  Giova

  ==============================================================================
*/

#include "Impact.h"

Impact::Impact()
{
    impact = SDTImpact_new();
}

Impact::~Impact()
{
}

void Impact::registerInteraction(char* key0, char* key1)
{
    SDT_registerInteractor(impact, key0, key1);
}

void Impact::setImpactParameters(float stiffness, double dissipation,
    double shape, long contact0, long contact1)
{
    SDTImpact_setStiffness(impact, stiffness);
    SDTImpact_setDissipation(impact, dissipation);
    SDTImpact_setShape(impact, shape);
    SDTInteractor_setFirstPoint(impact, contact0);
    SDTInteractor_setSecondPoint(impact, contact1);
}