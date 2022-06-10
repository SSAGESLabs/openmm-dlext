// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#include "DLExtKernelFactory.h"

#include "DLExtKernels.h"
#include "cxx11utils.h"
#include "openmm/OpenMMException.h"
#include "openmm/cpu/CpuPlatform.h"
#include "openmm/reference/ReferencePlatform.h"
#ifdef OPENMM_BUILD_CUDA_LIB
#include "openmm/cuda/CudaPlatform.h"
#endif

using namespace cxx11utils;
using namespace DLExt;

extern "C" DEFAULT_VISIBILITY void registerPlatforms() {}

extern "C" DEFAULT_VISIBILITY void registerKernelFactories()
{
    for (int i = 0; i < OpenMM::Platform::getNumPlatforms(); i++) {
        auto& platform = OpenMM::Platform::getPlatform(i);
        if (DLExt::isSupported(platform))
            platform.registerKernelFactory(ForceKernel::Name(), new KernelFactory());
    }
}

bool DLExt::isSupported(OpenMM::Platform& platform)
{
    const auto& id = typeid(platform);

    return (
        id == typeid(OpenMM::ReferencePlatform) || id == typeid(OpenMM::CpuPlatform)
#ifdef OPENMM_BUILD_CUDA_LIB
        || id == typeid(OpenMM::CudaPlatform)
#endif
    );
}

OpenMM::KernelImpl* KernelFactory::createKernelImpl(
    std::string name, const OpenMM::Platform& platform, OpenMM::ContextImpl& context) const
{
    if (name == ForceKernel::Name())
        return new ForceKernel(name, platform);

    throw OpenMM::OpenMMException(
        (std::string("Tried to create illegal kernel with name `") + name + "`").c_str());
}
