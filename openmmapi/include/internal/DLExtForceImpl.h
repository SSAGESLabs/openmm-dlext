// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_FORCEIMPL_H_
#define OPENMM_DLEXT_FORCEIMPL_H_

#include "DLExtForce.h"
#include "openmm/cpu/CpuPlatform.h"
#include "openmm/internal/ForceImpl.h"

#include <map>
#include <string>
#include <vector>

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
    void setCallback(Function<void, long long>& f);

private:
    const OpenMM::Force& owner;
    Function<void, long long> callback = [](long long) {};
};

//
//  TODO: For OpenMM > 7.6 write a simplified version using
//  `ContextImpl::getStepCount`
//
inline long long getStepCount(OpenMM::ContextImpl& context)
{
    long long step_count;
    const auto& id = typeid(context.getPlatform());

    if (id == typeid(OpenMM::ReferencePlatform) || id == typeid(OpenMM::CpuPlatform)) {
        auto data = reinterpret_cast<ReferencePlatformData*>(context.getPlatformData());
        step_count = static_cast<long long>(data->stepCount);
    }
#ifdef OPENMM_BUILD_CUDA_LIB
    if (id == typeid(OpenMM::CudaPlatform)) {
        auto data = reinterpret_cast<CudaPlatformData*>(context.getPlatformData());
        step_count = static_cast<long long>(data->stepCount);
    }
#endif

    return step_count;
}

}  // namespace DLExt

#endif  // OPENMM_DLEXT_FORCEIMPL_H_
