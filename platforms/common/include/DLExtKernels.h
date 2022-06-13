// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_KERNELS_H_
#define OPENMM_DLEXT_KERNELS_H_

#include "openmm/KernelImpl.h"

namespace DLExt
{

class ForceKernel : public OpenMM::KernelImpl {
public:
    static std::string Name() { return "DLExtForceKernel"; }

    ForceKernel(std::string name, const OpenMM::Platform& platform)
        : OpenMM::KernelImpl(name, platform)
    { }
};

}  // namespace DLExt

#endif  // OPENMM_DLEXT_KERNELS_H_
