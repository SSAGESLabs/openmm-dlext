// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_H_
#define OPENMM_DLEXT_H_


#include "cxx11utils.h"
#include "dlpack.h"


namespace DLExt
{


using ReferenceArray = std::vector<OpenMM::Vec3>;
using CudaArray = OpenMM::CudaArray;


static struct PositionsGetter     { } kPositions;
static struct VelocitiesGetter    { } kVelocities;
static struct ForcesGetter        { } kForces;
static struct AtomIdsGetter       { } kAtomIds;
static struct InverseMassesGetter { } kInverseMasses;

static struct SecondDim { } kSecondDim;

struct DLDataBridge {
    std::vector<int64_t> shape;
    std::vector<int64_t> strides;
    DLManagedTensor tensor;
};


void _DLDataBridgeDeleter(DLManagedTensor* tensor)
{
    if (tensor)
        delete static_cast<DLDataBridge*>(tensor->manager_ctx);
}

// Uniform interface for property extraction

inline ReferenceArray& getProperty(ReferencePlatformData& pdata, PositionsGetter)
{
    return *pdata.positions;
}

inline ReferenceArray& getProperty(ReferencePlatformData& pdata, VelocitiesGetter)
{
    return *pdata.velocities;
}

inline ReferenceArray& getProperty(ReferencePlatformData& pdata, ForcesGetter)
{
    return *pdata.forces;
}

inline CudaArray& getProperty(CudaPlatformData& pdata, PositionsGetter)
{
    return pdata.contexts[0]->getPosq();
}

inline CudaArray& getProperty(CudaPlatformData& pdata, VelocitiesGetter)
{
    return pdata.contexts[0]->getVelm();
}

inline CudaArray& getProperty(CudaPlatformData& pdata, ForcesGetter)
{
    return pdata.contexts[0]->getForce();
}

inline CudaArray& getProperty(CudaPlatformData& pdata, AtomIdsGetter)
{
    return pdata.contexts[0]->getAtomIndexArray();
}

// `opaque` returns a `void*` with the memory address of an array or stored property

inline void* opaque(CudaArray& array)
{
    return (void*)(array.getDevicePointer());
}

template <typename T>
inline void* opaque(const std::vector<T>& array)
{
    return (void*)(array.data());
}

template <typename PlatformData, typename Property>
inline void* opaque(const ContextView& view, Property p)
{
    auto& pdata = view.platformData<PlatformData>();
    return opaque(getProperty(pdata, p));
}

template <typename Property>
inline void* opaque(const ContextView& view, Property p)
{
    if (view.deviceType() == kDLGPU) {
        return opaque<CudaPlatformData>(view, p);
    }
    return opaque<ReferencePlatformData>(view, p);
}

inline void* opaque(const ContextView& view, AtomIdsGetter)
{
    if (view.deviceType() == kDLGPU) {
        return opaque<CudaPlatformData>(view, kAtomIds);
    }
    return opaque(view.atomIds());
}

inline void* opaque(const ContextView& view, InverseMassesGetter)
{
    if (view.deviceType() == kDLGPU) {
        // On the CudaPlatform inverse masses are stored along velocities
        return opaque<CudaPlatformData>(view, kVelocities);
    }
    return opaque(view.inverseMasses());
}

inline DLContext deviceInfo(const ContextView& view)
{
    if (view.deviceType() == kDLGPU) {
        auto& pdata = view.platformData<CudaPlatformData>();
        return DLContext { kDLGPU, pdata.contexts[0]->getDeviceIndex() };
    }
    return DLContext { kDLCPU, 0 };
}

constexpr DLDataType dtype(const ContextView& view, PositionsGetter)
{
    return DLDataType { kDLFloat, view.posPrecBits(), 1 };
}

constexpr DLDataType dtype(const ContextView& view, VelocitiesGetter)
{
    return DLDataType { kDLFloat, view.velPrecBits(), 1 };
}

constexpr DLDataType dtype(const ContextView& view, ForcesGetter)
{
    return DLDataType { view.forcesTypeCode(), 64, 1 };
}

constexpr DLDataType dtype(const ContextView& view, AtomIdsGetter)
{
    return DLDataType { kDLInt, 32, 1 };
}

constexpr DLDataType dtype(const ContextView& view, InverseMassesGetter)
{
    return dtype(view, kVelocities);
}

constexpr int64_t paddedSize(const ContextView& view)
{
    auto n = view.particleNumber();

    if (view.deviceType() == kDLGPU)
        return 32 * static_cast<int64_t>(std::ceil(n / 32.0));

    return n;
}

constexpr int64_t paddedSize(int64_t n)
{
    return (n % 32 == 0) ? n : 32 * (n / 32 + 1);
}

constexpr int64_t paddedSize(const ContextView& view, int64_t n)
{
    return view.deviceType() == kDLGPU ? paddedSize(n) : n;
}

template <typename PropertyGetter>
constexpr int64_t size(const ContextView& view, PropertyGetter)
{
    return paddedSize(view, view.particleNumber());
}

constexpr int64_t size(const ContextView& view, ForcesGetter)
{
    return view.deviceType() == kDLGPU ? 3 : view.particleNumber();
}

template <typename PropertyGetter>
constexpr int64_t size(const ContextView& view, PropertyGetter, SecondDim)
{
    return view.deviceType() == kDLGPU ? 4 : 3;
}

constexpr int64_t size(const ContextView& view, ForcesGetter, SecondDim)
{
    return view.deviceType() == kDLGPU ? paddedSize(view.particleNumber()) : 3;
}

constexpr int64_t size(const ContextView& view, AtomIdsGetter, SecondDim) { return 1; }

template <typename PropertyGetter>
constexpr uint64_t offset(const ContextView& view, PropertyGetter)
{
    return 0;
}

template <typename PropertyGetter>
DLManagedTensor* wrap(
    const ContextView& view, PropertyGetter property
) {
    auto bridge = make_unique<DLDataBridge>();

    bridge->tensor.manager_ctx = bridge.get();
    bridge->tensor.deleter = DLDataBridgeDeleter;

    auto& dltensor = bridge->tensor.dl_tensor;
    dltensor.data = opaque(view, property);
    dltensor.ctx = deviceInfo(view);
    dltensor.dtype = dtype(view, property);

    auto& shape = bridge->shape;
    auto size2 = size(view, property, kSecondDim);
    shape.push_back(size(view, property));
    if (size2 > 1)
        shape.push_back(size2);

    auto& strides = bridge->strides;
    strides.push_back(size2);
    if (size2 > 1)
        strides.push_back(1);

    dltensor.ndim = shape.size();
    dltensor.shape = reinterpret_cast<std::int64_t*>(shape.data());
    dltensor.strides = reinterpret_cast<std::int64_t*>(strides.data());
    dltensor.byte_offset = offset(view, property);

    return &(bridge.release()->tensor);
}

}  // DLExt


#endif  // OPENMM_DLEXT_H_
