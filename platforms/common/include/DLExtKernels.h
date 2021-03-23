// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_KERNELS_H_
#define OPENMM_DLEXT_KERNELS_H_


#include <string>

#include "openmm/KernelImpl.h"
#include "openmm/Platform.h"
#include "openmm/System.h"


namespace DLExt
{


class ForceKernel : public OpenMM::KernelImpl {
public:
    static std::string Name() { return "DLExtForceKernel"; }

    ForceKernel(std::string name, const OpenMM::Platform& platform)
      : OpenMM::KernelImpl(name, platform)
    { }

    virtual void initialize(
        OpenMM::ContextImpl& context, Force& force, Callback update
    ) = 0;

    virtual double execute(
        OpenMM::ContextImpl& context, bool includeForces, bool includeEnergy
    ) = 0;
};


}  // namespace DLExt


#endif  // OPENMM_DLEXT_KERNELS_H_
