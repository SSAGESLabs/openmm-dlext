// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef OPENMM_PYDLEXT_H_
#define OPENMM_PYDLEXT_H_

#include "ContextView.h"
#include "DLExt.h"
#include "pybind11/pybind11.h"

namespace DLExt
{

using PropertyGetter = DLManagedTensor* (*)(const ContextView&);

const char* const kDLTensorCapsuleName = "dltensor";

template <PropertyGetter property>
inline pybind11::capsule encapsulate(const ContextView& view)
{
    auto dl_managed_tensor = property(view);
    return pybind11::capsule(
        dl_managed_tensor, kDLTensorCapsuleName,
        [](PyObject* obj) { // PyCapsule_Destructor
            auto dlmt =
                static_cast<DLManagedTensor*>(PyCapsule_GetPointer(obj, kDLTensorCapsuleName));
            if (dlmt && dlmt->deleter) {
                dlmt->deleter(dlmt);
            } else {
                PyErr_Clear();
            }
        });
}

template <typename T>
inline T& cast(pybind11::capsule& capsule)
{
    T* ptr = capsule;
    return *ptr;
}

} // namespace DLExt

#endif // OPENMM_PYDLEXT_H_
