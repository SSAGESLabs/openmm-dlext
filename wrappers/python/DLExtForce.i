%module dlpack_extension_swig

%import(module = "simtk.openmm") "OpenMMForce.i"
%include "std_string.i"

%{
#include "DLExtForce.h"
#include "OpenMM.h"
%}

namespace DLExt
{

class Force : public OpenMM::Force {
public:
    Force();
};

}

%inline
%{

PyObject* _to_capsule(DLExt::Force& force)
{
    return PyCapsule_New(static_cast<void*>(&force), "DLExt::Force", nullptr);
}

PyObject* _to_capsule(OpenMM::Context& context)
{
    return PyCapsule_New(static_cast<void*>(&context), "OpenMM::Context", nullptr);
}

PyObject* _to_capsule(OpenMM::System& system)
{
    return PyCapsule_New(static_cast<void*>(&system), "OpenMM::System", nullptr);
}

%}
