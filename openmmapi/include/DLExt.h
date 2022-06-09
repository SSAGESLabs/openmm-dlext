// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_DLEXT_H_
#define OPENMM_DLEXT_H_

#include <vector>

#include "cxx11utils.h"
#include "dlpack/dlpack.h"

#include "ContextView.h"

#include "openmm/Vec3.h"
#ifdef OPENMM_BUILD_CUDA_LIB
#include "openmm/cuda/CudaArray.h"
#include "openmm/cuda/CudaContext.h"
#endif

namespace DLExt {

using ReferenceArray = std::vector<OpenMM::Vec3>;
#ifdef OPENMM_BUILD_CUDA_LIB
using CudaArray = OpenMM::CudaArray;
#endif

static struct PositionsGetter {
} kPositions;
static struct VelocitiesGetter {
} kVelocities;
static struct ForcesGetter {
} kForces;
static struct AtomIdsGetter {
} kAtomIds;
static struct InverseMassesGetter {
} kInverseMasses;

static struct SecondDim {
} kSecondDim;

struct DLDataBridge {
  std::vector<int64_t> shape;
  std::vector<int64_t> strides;
  DLManagedTensor tensor;
};

void _DLDataBridgeDeleter(DLManagedTensor *tensor) {
  if (tensor)
    delete static_cast<DLDataBridge *>(tensor->manager_ctx);
}

// Uniform interface for property extraction
// `opaque` returns a `void*` with the memory address of an array or stored
// property

template <typename T> inline void *opaque(const std::vector<T> *array) {
  return (void *)(array->data());
}

inline void *opaque(ReferencePlatformData &pdata, PositionsGetter) {
  return opaque(reinterpret_cast<ReferenceArray *>(pdata.positions));
}

inline void *opaque(ReferencePlatformData &pdata, VelocitiesGetter) {
  return opaque(reinterpret_cast<ReferenceArray *>(pdata.velocities));
}

inline void *opaque(ReferencePlatformData &pdata, ForcesGetter) {
  return opaque(reinterpret_cast<ReferenceArray *>(pdata.forces));
}

#ifdef OPENMM_BUILD_CUDA_LIB

inline void *opaque(CudaArray &array) {
  return (void *)(array.getDevicePointer());
}

inline void *opaque(CudaPlatformData &pdata, PositionsGetter) {
  return opaque(pdata.contexts[0]->getPosq());
}

inline void *opaque(CudaPlatformData &pdata, VelocitiesGetter) {
  return opaque(pdata.contexts[0]->getVelm());
}

inline void *opaque(CudaPlatformData &pdata, ForcesGetter) {
  return opaque(pdata.contexts[0]->getForce());
}

inline void *opaque(CudaPlatformData &pdata, AtomIdsGetter) {
  return opaque(pdata.contexts[0]->getAtomIndexArray());
}

#endif // OPENMM_BUILD_CUDA_LIB

template <typename PlatformData, typename Property>
inline void *opaque(const ContextView &view, Property p) {
  auto &pdata = view.platformData<PlatformData>();
  return opaque(pdata, p);
}

template <typename Property>
inline void *opaque(const ContextView &view, Property p) {
#ifdef OPENMM_BUILD_CUDA_LIB
  if (view.deviceType() == kDLGPU)
    return opaque<CudaPlatformData>(view, p);
#endif
  return opaque<ReferencePlatformData>(view, p);
}

inline void *opaque(const ContextView &view, AtomIdsGetter) {
#ifdef OPENMM_BUILD_CUDA_LIB
  if (view.deviceType() == kDLGPU)
    return opaque<CudaPlatformData>(view, kAtomIds);
#endif
  return opaque(&view.atomIds());
}

inline void *opaque(const ContextView &view, InverseMassesGetter) {
#ifdef OPENMM_BUILD_CUDA_LIB
  // On the CudaPlatform inverse masses are stored along velocities
  if (view.deviceType() == kDLGPU)
    return opaque<CudaPlatformData>(view, kVelocities);
#endif
  return opaque(&view.inverseMasses());
}

inline DLContext deviceInfo(const ContextView &view) {
#ifdef OPENMM_BUILD_CUDA_LIB
  if (view.deviceType() == kDLGPU) {
    auto &pdata = view.platformData<CudaPlatformData>();
    return DLContext{kDLGPU, pdata.contexts[0]->getDeviceIndex()};
  }
#endif
  return DLContext{kDLCPU, 0};
}

inline DLDataType dType(const ContextView &view, PositionsGetter) {
  return DLDataType{kDLFloat, view.posPrecBits(), 1};
}

inline DLDataType dType(const ContextView &view, VelocitiesGetter) {
  return DLDataType{kDLFloat, view.velPrecBits(), 1};
}

inline DLDataType dType(const ContextView &view, ForcesGetter) {
  return DLDataType{view.forcesTypeCode(), 64, 1};
}

constexpr DLDataType dType(const ContextView &view, AtomIdsGetter) {
  return DLDataType{kDLInt, 32, 1};
}

inline DLDataType dType(const ContextView &view, InverseMassesGetter) {
  return dType(view, kVelocities);
}

constexpr int64_t paddedSize(int64_t n) {
  return (n % 32 == 0) ? n : 32 * (n / 32 + 1);
}

inline int64_t paddedSize(const ContextView &view, int64_t n) {
  return view.deviceType() == kDLGPU ? paddedSize(n) : n;
}

template <typename PropertyGetter>
inline int64_t size(const ContextView &view, PropertyGetter) {
  return paddedSize(view, view.particleNumber());
}

inline int64_t size(const ContextView &view, ForcesGetter) {
  return view.deviceType() == kDLGPU ? 3 : view.particleNumber();
}

template <typename PropertyGetter>
inline int64_t size(const ContextView &view, PropertyGetter, SecondDim) {
  return view.deviceType() == kDLGPU ? 4 : 3;
}

inline int64_t size(const ContextView &view, ForcesGetter, SecondDim) {
  return view.deviceType() == kDLGPU ? paddedSize(view.particleNumber()) : 3;
}

constexpr int64_t size(const ContextView &view, AtomIdsGetter, SecondDim) {
  return 1;
}

inline int64_t size(const ContextView &view, InverseMassesGetter, SecondDim) {
  return view.deviceType() == kDLGPU ? 4 : 1;
}

template <typename PropertyGetter>
constexpr uint64_t offset(const ContextView &view, PropertyGetter) {
  return 0;
}

template <typename PropertyGetter>
DLManagedTensor *wrap(const ContextView &view, PropertyGetter property) {
  auto bridge = make_unique<DLDataBridge>();

  bridge->tensor.manager_ctx = bridge.get();
  bridge->tensor.deleter = _DLDataBridgeDeleter;

  auto &dltensor = bridge->tensor.dl_tensor;
  dltensor.data = opaque(view, property);
  dltensor.device = deviceInfo(view);
  dltensor.dtype = dType(view, property);

  auto &shape = bridge->shape;
  auto size2 = size(view, property, kSecondDim);
  shape.push_back(size(view, property));
  if (size2 > 1)
    shape.push_back(size2);

  auto &strides = bridge->strides;
  strides.push_back(size2);
  if (size2 > 1)
    strides.push_back(1);

  dltensor.ndim = shape.size();
  dltensor.shape = reinterpret_cast<std::int64_t *>(shape.data());
  dltensor.strides = reinterpret_cast<std::int64_t *>(strides.data());
  dltensor.byte_offset = offset(view, property);

  return &(bridge.release()->tensor);
}

inline DLManagedTensor *positions(const ContextView &view) {
  return wrap(view, kPositions);
}

inline DLManagedTensor *velocities(const ContextView &view) {
  return wrap(view, kVelocities);
}

inline DLManagedTensor *forces(const ContextView &view) {
  return wrap(view, kForces);
}

inline DLManagedTensor *atomIds(const ContextView &view) {
  return wrap(view, kAtomIds);
}

inline DLManagedTensor *inverseMasses(const ContextView &view) {
  return wrap(view, kInverseMasses);
}

} // namespace DLExt

#endif // OPENMM_DLEXT_H_
