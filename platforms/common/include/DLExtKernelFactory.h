// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_KERNEL_FACTORY_H_
#define OPENMM_DLEXT_KERNEL_FACTORY_H_


#include "openmm/KernelFactory.h"


namespace DLExt
{


//
//  This class creates kernels for the DLExt plugin.
//
class KernelFactory : public OpenMM::KernelFactory {
public:
    OpenMM::KernelImpl* createKernelImpl(
        std::string name, const OpenMM::Platform& platform, OpenMM::ContextImpl& context
    ) const;
};

void registerKernelFactory(void* platform);
void registerKernelFactory(OpenMM::Platform* platform);
bool isSupported(OpenMM::Platform* platform);


}  // namespace DLExt


#endif  // OPENMM_DLEXT_KERNEL_FACTORY_H_
