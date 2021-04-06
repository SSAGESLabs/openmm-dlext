// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_FORCEIMPL_H_
#define OPENMM_DLEXT_FORCEIMPL_H_


#include <map>
#include <string>
#include <vector>

#include "DLExtForce.h"

#include "openmm/internal/ForceImpl.h"


namespace DLExt
{


//
//  Internal implementation of DLExt::Force.
//
class DEFAULT_VISIBILITY ForceImpl : public OpenMM::ForceImpl {
public:
    ForceImpl(const Force& owner);
    void initialize(OpenMM::ContextImpl& context);
    const OpenMM::Force& getOwner() const;
    void updateContextState(OpenMM::ContextImpl& context, bool& forcesInvalid);
    double calcForcesAndEnergy(
        OpenMM::ContextImpl& context, bool includeForces, bool includeEnergy, int groups
    );
    std::map<std::string, double> getDefaultParameters();
    std::vector<std::string> getKernelNames();
    void setCallback(Function<void>& f);
private:
    const OpenMM::Force& owner;
    Function<void> callback = []() { };
};


}  // namespace DLExt


#endif  // OPENMM_DLEXT_FORCEIMPL_H_
