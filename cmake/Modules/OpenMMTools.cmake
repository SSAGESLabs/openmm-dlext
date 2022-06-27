# Helper functions for FindOpenMM.cmake

function(find_openmm_with_python)
    find_package(Python QUIET COMPONENTS Interpreter)
    if(NOT Python_EXECUTABLE)
        return()
    endif()
    set(FIND_OpenMM_SCRIPT "
from __future__ import print_function;
import os
import openmm
libpath = openmm.version.openmm_library_path
print(os.path.normpath(os.path.join(libpath, os.pardir)), end='')"
    )
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "${FIND_OpenMM_SCRIPT}"
        OUTPUT_VARIABLE OpenMM_PATH
    )
    message(STATUS "Path for OpenMM: ${OpenMM_PATH}")
    if(OpenMM_PATH)
        if(NOT OpenMM_ROOT)
            set(OpenMM_ROOT ${OpenMM_PATH} PARENT_SCOPE)
        endif()
        set(OpenMM_Python_EXECUTABLE ${Python_EXECUTABLE} PARENT_SCOPE)
    endif()
endfunction()

function(check_python_and_openmm)
    if(OpenMM_ROOT AND NOT OpenMM_Python_EXECUTABLE)
        if(NOT Python_EXECUTABLE)
            set(Python_EXECUTABLE "${OpenMM_ROOT}/bin/python")
        endif()
        find_openmm_with_python()
    endif()
    if(OpenMM_Python_EXECUTABLE)
        set(Python_EXECUTABLE ${OpenMM_Python_EXECUTABLE} PARENT_SCOPE)
        return()
    endif()
    message(FATAL_ERROR "Couldn't find a matching python installation for OpenMM")
endfunction()

function(set_python_module_path)
    set(FIND_OpenMM_SCRIPT "
from __future__ import print_function;
from importlib import import_module
import os
try:
    openmm = import_module('openmm')
except:
    openmm = import_module('simtk')
print(os.path.normpath(os.path.join(openmm.__file__, os.pardir, os.pardir)), end='')"
    )
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "${FIND_OpenMM_SCRIPT}"
        OUTPUT_VARIABLE OpenMM_Python_PATH
    )
    set(OpenMMDLExt_Python_PATH "${OpenMM_Python_PATH}/openmm_dlext" PARENT_SCOPE)
endfunction()
