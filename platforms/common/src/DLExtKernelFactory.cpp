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

using OpenMMException = OpenMM::OpenMMException;
using Platform = OpenMM::Platform;


extern "C" DEFAULT_VISIBILITY void registerPlatforms() { }

extern "C" DEFAULT_VISIBILITY void registerKernelFactories()
{
    for (int i = 0; i < Platform::getNumPlatforms(); i++) {
        auto& platform = Platform::getPlatform(i);
        registerKernelFactory(&platform);
    }
}

void registerKernelFactory(void* p)
{
    auto platform = dynamic_cast<Platform*>(static_cast<Platform*>(p));
    if (platform)
        registerKernelFactory(platform);

    throw OpenMMException("Pointer to object is not an OpenMM::Platform*");
}

void registerKernelFactory(Platform* platform)
{
    if (isSupported(platform)) {
        auto factory = new KernelFactory();
        platform->registerKernelFactory(ForceKernel::Name(), factory);
    }
}

bool isSupported(Platform* platform)
{
    // The first condition catches both Reference and CPU platforms.
    return dynamic_cast<OpenMM::ReferencePlatform*>(platform)
#ifdef OPENMM_BUILD_CUDA_LIB
        || dynamic_cast<OpenMM::CudaPlatform*>(platform)
#endif
    ;
}

OpenMM::KernelImpl* KernelFactory::createKernelImpl(
    std::string name, const Platform& platform, OpenMM::ContextImpl& context
) const {
    if (name == ForceKernel::Name())
        return new ForceKernel(name, platform);

    throw OpenMMException(
        (std::string("Tried to create illegal kernel with name '") + name + "'").c_str()
    );
}
