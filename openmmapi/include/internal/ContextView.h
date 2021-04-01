// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_CONTEXTVIEW_H_
#define OPENMM_DLEXT_CONTEXTVIEW_H_


#include "cxx11utils.h"
#include "dlpack.h"


namespace DLExt
{


using ReferencePlatformData = OpenMM::ReferencePlatform::PlatformData;
using CudaPlatformData = OpenMM::CudaPlatform::PlatformData;
using ReferenceArray = std::vector<OpenMM::Vec3>;
using CudaArray = OpenMM::CudaArray;


enum class IdsOrdering { Ordered, Forward, Reverse };


class DEFAULT_VISIBILITY ContextView {
public:
    ContextView(OpenMM::ContextImpl& context);
    DLDeviceType deviceType() const;
    template <typename T> T& platformData() const;
    int64_t particleNumber() const;
    const std::vector<double>& inverseMasses() const;
    const std::vector<int>& atomIds() const;
    IdsOrdering idsOrdering() const;
    uint8_t posPrecBits() const;
    uint8_t velPrecBits() const;
    uint8_t forcesTypeCode() const;
private:
    OpenMM::ContextImpl* ctx;
    void* pdata;
    DLDeviceType dtype;
    uint8_t pos_bits = 64;
    uint8_t vel_bits = 64;
    uint8_t forces_type = kDLFloat;
    std::vector<double> inv_masses;
    std::vector<int> atom_ids;
    IdsOrdering ids_ordering = IdsOrdering::Ordered;
};


DLDeviceType deviceType(OpenMM::Platform& platform);

template <typename T>
DLDeviceType deviceType();

template <>
constexpr DLDeviceType deviceType<ReferencePlatformData>() { return kDLCPU; }
template <>
constexpr DLDeviceType deviceType<CudaPlatformData>() { return kDLGPU; }

template <typename T>
T& ContextView::platformData() const
{
    if (DLExt::deviceType<T>() == dtype)
        return *reinterpret_cast<T*>(pdata);
    throw OpenMM::OpenMMException(
        "The requested type does not match the one of the stored data"
    );
}


}  // DLExt


#endif  // OPENMM_DLEXT_CONTEXTVIEW_H_
