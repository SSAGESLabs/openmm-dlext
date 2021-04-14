# Helper functions for FindOpenMM.cmake

function(find_openmm_with_python)
    find_package(Python QUIET COMPONENTS Interpreter)
    if(NOT Python_EXECUTABLE)
        return()
    endif()
    set(FIND_OpenMM_SCRIPT "
from __future__ import print_function;
import os
try:
    from simtk.openmm.version import openmm_library_path
    print(os.path.normpath(os.path.join(openmm_library_path, os.pardir)), end='')
except:
    print('', end='')"
    )
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "${FIND_OpenMM_SCRIPT}"
        OUTPUT_VARIABLE OpenMM_PATH
    )
    if(OpenMM_PATH)
        if(NOT OpenMM_ROOT)
            set(OpenMM_ROOT ${OpenMM_PATH} PARENT_SCOPE)
        endif()
        set(OpenMM_Python_EXECUTABLE ${Python_EXECUTABLE} PARENT_SCOPE)
    endif()
endfunction()

function(find_python_for_openmm)
    if(OpenMM_ROOT AND NOT Python_EXECUTABLE AND NOT OpenMM_Python_EXECUTABLE)
        set(Python_EXECUTABLE "${OpenMM_ROOT}/bin/python")
        find_openmm_with_python()
    endif()
    if(OpenMM_Python_EXECUTABLE)
        set(Python_EXECUTABLE ${OpenMM_Python_EXECUTABLE} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Couldn't find a matching python for OpenMM")
    endif()
endfunction()
