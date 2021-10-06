// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#include "pybind11/functional.h"

#include "DLExtForce.h"
#include "DLExtKernelFactory.h"
#include "DLExtKernels.h"
#include "PyDLExt.h"


using namespace DLExt;
namespace py = pybind11;


namespace
{

void registerKernelFactories()
{
    std::vector<std::string> names = { ForceKernel::Name() };

    for (int i = 0; i < OpenMM::Platform::getNumPlatforms(); i++) {
        auto& platform = OpenMM::Platform::getPlatform(i);
        if (DLExt::isSupported(platform) && !platform.supportsKernels(names))
            platform.registerKernelFactory(ForceKernel::Name(), new KernelFactory());
    }
}

Force* toForcePtr(py::capsule& capsule)
{
    if (strcmp(capsule.name(), "DLExt::Force") == 0)
        return &cast<Force>(capsule);
    throw py::type_error("Cannot convert the object to a DLExt::Force");
}

OpenMM::Context& toContext(py::capsule& capsule)
{
    if (strcmp(capsule.name(), "OpenMM::Context") == 0)
        return cast<OpenMM::Context>(capsule);
    throw py::type_error("Cannot convert the object to a OpenMM::Context");
}

OpenMM::System& toSystem(py::capsule& capsule)
{
    if (strcmp(capsule.name(), "OpenMM::System") == 0)
        return cast<OpenMM::System>(capsule);
    throw py::type_error("Cannot convert the object to a OpenMM::System");
}

}


void export_ContextView(py::module& m)
{
    // ContextView can only be constructed from the Force::view factory
    py::class_<ContextView>(m, "ContextView")
        .def(py::init(
            [](Force& force, py::capsule& context) {
                return force.view(toContext(context));
            })
        )
        .def("device_type", &ContextView::deviceType)
        .def("particle_number", &ContextView::particleNumber)
        .def("ids_ordering", &ContextView::idsOrdering)
        .def("synchronize", &ContextView::synchronize)
    ;
}

void export_Force(py::module& m)
{
    // We only create instances of Force by wrapping existing instances created
    // by the SWIG bindings, which will be responsible for deallocating these
    py::class_<Force, std::unique_ptr<Force, py::nodelete>>(m, "Force")
        .def(py::init( [](py::capsule& force) { return toForcePtr(force); } ),
            py::return_value_policy::reference
        )
        .def("is_present_in",
            [](Force& self, py::capsule& system) {
                return self.isPresentIn(toSystem(system));
            }
        )
        .def("add_to",
            [](Force& self, py::capsule& context, py::capsule& system) {
                self.addTo(toContext(context), toSystem(system));
            }
        )
        .def("set_callback_in",
            [](Force& self, py::capsule& context, Function<void, long long>& callback) {
                self.setCallbackIn(toContext(context), callback);
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
    py::enum_<IdsOrdering>(m, "IdsOrdering")
        .value("Ordered", IdsOrdering::Ordered)
        .value("Forward", IdsOrdering::Forward)
        .value("Reverse", IdsOrdering::Reverse)
    ;

    // Classes
    export_ContextView(m);
    export_Force(m);

    // Methods
    m.def("register_plugin", &registerKernelFactories);
    m.def("atom_ids", encapsulate<&atomIds>);
    m.def("forces", encapsulate<&forces>);
    m.def("inverse_masses", encapsulate<&inverseMasses>);
    m.def("positions", encapsulate<&positions>);
    m.def("velocities", encapsulate<&velocities>);
}
