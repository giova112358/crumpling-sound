/*
  ==============================================================================

    Impact.h
    Created: 27 Nov 2020 12:00:35pm
    Author:  Giova

    Basic solid impact interaction algorithm. The interaction is
    represented by the structure resonator-interactor-resonator
    object models expose two pickup points, for the resonating object,
    and one pickup point for the exciter. Each pickup point gives information
    about displacement and velocity, and can as well be used to apply
    an external force to the resonator.

  ==============================================================================
*/

#pragma once
#include "SDTCommon.h"
#include "SDTSolids.h"

class Impact
{
public:
    Impact();
    ~Impact();

    /*
     ============================================================================
       Link the two resonators with the interactor through their ID
       ============================================================================
   */
    void registerInteraction(char* key0, char* key1);

    /*
     ============================================================================
     Simulates a non-linear impact, by computing the contact force from the total
     compression, namely the relative displacement between the two contact points.
     Set the parameter of the impact interaction.
     =============================================================================
    */
    void setImpactParameters(float stiffness, double dissipation, double shape,
        long contact0, long contact1);

private:
    SDTInteractor* impact; /*impact inetractor object*/

};


