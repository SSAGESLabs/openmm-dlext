// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#include "DLExtForce.h"
#include "PyDLExt.h"


using namespace DLExt;
namespace py = pybind11;


void export_ContextView(py::module& m)
{
    // ContextView can only be constructed from the Force::view factory
    py::class_<ContextView>(m, "ContextView")
        .def(py::init(
            [](Force& force, py::capsule& context) {
                return force.view(cast<OpenMM::Context>(context));
            }
        ))
        .def("device_type", &ContextView::deviceType)
        .def("particle_number", &ContextView::particleNumber)
        .def("ids_ordering", &ContextView::idsOrdering)
    ;
}

void export_Force(py::module& m)
{
    py::class_<Force>(m, "Force")
        .def(py::init<>())
        .def("add_to",
            [](Force& self, py::capsule& context, py::capsule& system) {
                self.addTo(cast<OpenMM::Context>(context), cast<OpenMM::System>(system));
            }
        )
        .def("set_callback_in",
            [](Force& self, py::capsule& context, Function<void>& callback) {
                self.setCallbackIn(cast<OpenMM::Context>(context), callback);
            }
        )
        .def("view",
            [](Force& self, py::capsule& context) {
                return self.view(cast<OpenMM::Context>(context));
            }
        )
    ;
}


PYBIND11_MODULE(dlpack_extension, m)
{
    // Enums
    py::enum_<DLDeviceType>(m, "DeviceType")
        .value("CPU", kDLCPU)
        .value("GPU", kDLGPU)
    ;
    py::enum_<DLDataTypeCode>(m, "DLType")
        .value("Int", kDLInt)
        .value("UInt", kDLUInt)
        .value("Float", kDLFloat)
    ;
    py::enum_<IdsOrdering>(m, "IdsOrdering")
        .value("Ordered", IdsOrdering::Ordered)
        .value("UInt", IdsOrdering::Forward)
        .value("Float", IdsOrdering::Reverse)
    ;

    // Classes
    export_ContextView(m);
    export_Force(m);

    // Methods
    m.def("positions", encapsulate<&positions>);
    m.def("velocities", encapsulate<&velocities>);
    m.def("forces", encapsulate<&forces>);
    m.def("atom_ids", encapsulate<&atomIds>);
    m.def("inverse_masses", encapsulate<&inverseMasses>);
}
