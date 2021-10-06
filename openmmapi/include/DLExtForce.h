// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_FORCE_H_
#define OPENMM_DLEXT_FORCE_H_


#include "cxx11utils.h"

#include "ContextView.h"

#include "openmm/Force.h"
#include "openmm/System.h"


namespace DLExt
{


using namespace cxx11utils;


//
//  This class is meant to provide DLPack wrappers around the particle data of an OpenMM
//  Simulation, but not to perform any direct computation. Instead, it provides a callback
//  interface to access and modify the simulation externally.
//
class DEFAULT_VISIBILITY Force : public OpenMM::Force {
public:
    Force() { }
    bool usesPeriodicBoundaryConditions() const;
    bool isPresentIn(const OpenMM::System& system);
    void addTo(OpenMM::Context& context, OpenMM::System& system);
    void setCallbackIn(OpenMM::Context& context, Function<void, long long>& callback);
    ContextView view(OpenMM::Context& context);
protected:
    OpenMM::ForceImpl* createImpl() const;
private:
    bool uses_periodic_bc = true;
};


}  // namespace DLExt


#endif  // OPENMM_DLEXT_FORCE_H_
