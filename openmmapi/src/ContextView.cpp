// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#include "ContextView.h"

#include "openmm/OpenMMException.h"
#include "openmm/cpu/CpuPlatform.h"
#ifdef OPENMM_BUILD_CUDA_LIB
#include "openmm/cuda/CudaContext.h"
#endif


using namespace DLExt;


DLDeviceType deviceType(OpenMM::Platform& platform)
{
    const auto& id = typeid(platform);

    if (id == typeid(OpenMM::ReferencePlatform) || id == typeid(OpenMM::CpuPlatform))
        return kDLCPU;
#ifdef OPENMM_BUILD_CUDA_LIB
    if (id == typeid(OpenMM::CudaPlatform))
        return kDLGPU;
#endif

    throw OpenMM::OpenMMException("Unsupported platform");
}

ContextView::ContextView(OpenMM::ContextImpl& context)
    : ctx { &context }
{
    dtype = DLExt::deviceType(ctx->getPlatform());  // disambiguate from member function
    pdata = ctx->getPlatformData();

    auto& system = ctx->getSystem();
    auto n = system.getNumParticles();
    inv_masses.resize(n);
    atom_ids.resize(n);
    for (int i = 0; i < n; ++i) {
        inv_masses[i] = 1 / system.getParticleMass(i);
        atom_ids[i] = i;
    }

#ifdef OPENMM_BUILD_CUDA_LIB
    if (dtype == kDLGPU) {
        ids_ordering = IdsOrdering::Forward;
        auto data = reinterpret_cast<CudaPlatformData*>(pdata);
        if (!data->contexts[0]->getUseDoublePrecision()) {
            pos_bits = 32;
            if (!data->contexts[0]->getUseMixedPrecision())
                vel_bits = 32;
        }
        forces_type = kDLInt;
    }
#endif
}

DLDeviceType ContextView::deviceType() const { return dtype; }

int64_t ContextView::particleNumber() const { return ctx->getSystem().getNumParticles(); }

const std::vector<double>& ContextView::inverseMasses() const { return inv_masses; }

const std::vector<int>& ContextView::atomIds() const { return atom_ids; }

IdsOrdering ContextView::idsOrdering() const { return ids_ordering; }

uint8_t ContextView::posPrecBits() const { return pos_bits; }

uint8_t ContextView::velPrecBits() const { return vel_bits; }

uint8_t ContextView::forcesTypeCode() const { return forces_type; }
