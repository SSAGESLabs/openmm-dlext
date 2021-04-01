// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_H_
#define OPENMM_DLEXT_H_


#include "cxx11utils.h"
#include "dlpack.h"


namespace DLExt
{


static struct PositionsGetter{} kPositions;
static struct VelocitiesGetter{} kVelocities;
static struct ForcesGetter{} kForces;
static struct AtomIdsGetter{} kAtomIds;
static struct Dim2 { } kDim2;

struct DLDataBridge {
    std::vector<int64_t> shape;
    std::vector<int64_t> strides;
    DLManagedTensor tensor;
};


void DLDataBridgeDeleter(DLManagedTensor* tensor)
{
    if (tensor)
        delete static_cast<DLDataBridge*>(tensor->manager_ctx);
}

inline void* opaque(OpenMM::CudaArray& array)
{
    return (void*)(array.getDevicePointer());
}

template <typename T>
inline void* opaque(std::vector<T>& array)
{
    return static_cast<void*>(array.data());
}

template <typename T>
inline void* opaque(const std::vector<T>& array)
{
    return (void*)(array.data());
}

template <typename Property>
inline void* opaque(const ContextView& view, Property p)
{
    if (view.deviceType() == kDLGPU) {
        auto& pdata = view.platformData<CudaPlatformData>();
        return opaque(getProperty(pdata, p));
    }
    auto& pdata = view.platformData<ReferencePlatformData>();
    return opaque(getProperty(pdata, p));
}

inline void* opaque(const ContextView& view, AtomIdsGetter)
{
    if (view.deviceType() == kDLGPU) {
        auto& pdata = view.platformData<CudaPlatformData>();
        return opaque(getProperty(pdata, kAtomIds));
    }
    auto& pdata = view.platformData<ReferencePlatformData>();
    return opaque(view.atomIds());
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

constexpr int64_t paddedSize(const ContextView& view)
{
    auto n = view.particleNumber();

    if (view.deviceType() == kDLGPU)
        return 32 * static_cast<int64_t>(std::ceil(n / 32.0));

    return n;
}

template <typename PropertyGetter>
constexpr int64_t size(const ContextView& view, PropertyGetter)
{
    return paddedSize(view);
}

constexpr int64_t size(const ContextView& view, ForcesGetter) { return 3; }

constexpr int64_t size(const ContextView& view, PositionsGetter, Dim2)
{
    return view.deviceType() == kDLGPU ? 4 : 3;
}

constexpr int64_t size(const ContextView& view, VelocitiesGetter, Dim2)
{
    return view.deviceType() == kDLGPU ? 4 : 3;
}

constexpr int64_t size(const ContextView& view, ForcesGetter, Dim2)
{
    return paddedSize(view);
}

constexpr int64_t size(const ContextView& view, AtomIdsGetter, Dim2) { return 1; }

template <typename PropertyGetter>
DLManagedTensor* wrap(
    const ContextView& view, PropertyGetter property, uint64_t offset = 0
) {
    auto bridge = make_unique<DLDataBridge>();

    bridge->tensor.manager_ctx = bridge.get();
    bridge->tensor.deleter = DLDataBridgeDeleter;

    auto& dltensor = bridge->tensor.dl_tensor;
    dltensor.data = opaque(view, property);
    dltensor.ctx = deviceInfo(view);
    dltensor.dtype = dtype(view, property);

    auto& shape = bridge->shape;
    auto size2 = size(view, property, kDim2);
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
    dltensor.byte_offset = offset;

    return &(bridge.release()->tensor);
}


}  // DLExt


#endif  // OPENMM_DLEXT_H_
