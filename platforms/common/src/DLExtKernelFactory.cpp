// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#include "cxx11utils.h"
#include "DLExtKernelFactory.h"

#include "openmm/OpenMMException.h"

#include "openmm/reference/ReferencePlatform.h"
#ifdef OPENMM_BUILD_CUDA_LIB
#include "openmm/cuda/CudaPlatform.h"
#endif


using namespace cxx11utils;
using namespace DLExt;


extern "C" DEFAULT_VISIBILITY void registerPlatforms() { }

extern "C" DEFAULT_VISIBILITY void registerKernelFactories()
{
    for (int i = 0; i < OpenMM::Platform::getNumPlatforms(); i++) {
        OpenMM::Platform& platform = OpenMM::Platform::getPlatform(i);
        registerKernelFactory(&platform);
    }
}

void registerKernelFactory(void* p)
{
    auto platform = dynamic_cast<OpenMM::Platform*>(p);
    if (platform)
        registerKernelFactory(platform);

    throw OpenMM::OpenMMException("Pointer to object is not an OpenMM::Platform*");
}

void registerKernelFactory(OpenMM::Platform* platform)
{
    if (isSupported(platform)) {
        KernelFactory* factory = new KernelFactory();
        platform->registerKernelFactory(ForceKernel::Name(), factory);
    }
}

bool isSupported(OpenMM::Platform* platform)
{
    // The first condition catches both Reference and CPU platforms.
    return dynamic_cast<OpenMM::ReferencePlatform*>(platform)
#ifdef OPENMM_BUILD_CUDA_LIB
        || dynamic_cast<OpenMM::CudaPlatform*>(platform)
#endif
    ;
}

OpenMM::KernelImpl* KernelFactory::createKernelImpl(
    std::string name, const OpenMM::Platform& platform, OpenMM::ContextImpl& context
) const {
    if (name == ForceKernel::Name())
        return new ForceKernel(name, platform);

    throw OpenMM::OpenMMException(
        (std::string("Tried to create illegal kernel with name '") + name + "'").c_str()
    );
}
