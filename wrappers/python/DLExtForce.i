%module dlpack_extension_swig

%import(module = "simtk.openmm") "OpenMMForce.i"
%include "std_string.i"

%{
#include "Convert.h"
#include "DLExtForce.h"
#include "OpenMM.h"
%}

namespace DLExt
{

class Force : public OpenMM::Force {
public:
    Force();
};

class Convert {
public:
    %extend
    {

    static PyObject* toCapsule(Force& force)
    {
        return PyCapsule_New(static_cast<void*>(&force), "DLExt::Force", nullptr);
    }
    static PyObject* toCapsule(OpenMM::Context& context)
    {
        return PyCapsule_New(static_cast<void*>(&context), "OpenMM::Context", nullptr);
    }
    static PyObject* toCapsule(OpenMM::System& system)
    {
        return PyCapsule_New(static_cast<void*>(&system), "OpenMM::System", nullptr);
    }

    }
};

}
