%module dlpack_extension_swig

%import(module = "simtk.openmm") "OpenMMForce.i"
%include "swig/typemaps.i"
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
