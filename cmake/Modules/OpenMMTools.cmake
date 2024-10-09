# Helper functions for FindOpenMM.cmake

function(find_openmm_with_python)
    find_package(Python QUIET COMPONENTS Interpreter)
    if(NOT Python_EXECUTABLE)
        return()
    endif()
    set(FIND_OpenMM_SCRIPT "
from __future__ import print_function;
from importlib import import_module
import os
openmm = None
for m in ('openmm', 'simtk.openmm'):
    try:
        openmm = import_module(m)
        break
    except:
        continue
if openmm is not None:
    libpath = openmm.version.openmm_library_path
    print(os.path.normpath(os.path.join(libpath, os.pardir)))
    print(openmm.__version__, end='')
else:
    print()"
    )
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "${FIND_OpenMM_SCRIPT}"
        OUTPUT_VARIABLE OpenMM_PATH_AND_VER
    )
    string(REGEX MATCH "(.*)\n(.*)" _ "${OpenMM_PATH_AND_VER}")
    set(OpenMM_PATH ${CMAKE_MATCH_1})
    set(OpenMM_VERSION ${CMAKE_MATCH_2})
    if(OpenMM_PATH)
        if(NOT OpenMM_ROOT)
            set(OpenMM_ROOT ${OpenMM_PATH} PARENT_SCOPE)
        endif()
        set(OpenMM_Python_EXECUTABLE ${Python_EXECUTABLE} PARENT_SCOPE)
    endif()
    if(OpenMM_VERSION)
        set(OpenMM_VERSION ${OpenMM_VERSION} PARENT_SCOPE)
        if(${OpenMM_VERSION} VERSION_LESS "7.6")
            set(OpenMM_SIMTK_API TRUE PARENT_SCOPE)
        endif()
    endif()
endfunction()

function(check_python_and_openmm)
    if(OpenMM_ROOT AND NOT OpenMM_Python_EXECUTABLE)
        set(Python_EXECUTABLE_GUESS "${OpenMM_ROOT}/bin/python")
        if(NOT Python_EXECUTABLE AND EXISTS ${Python_EXECUTABLE_GUESS})
            set(Python_EXECUTABLE ${Python_EXECUTABLE_GUESS})
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
    set(OpenMMDLExt_Python_PATH "${OpenMM_Python_PATH}" PARENT_SCOPE)
endfunction()

# This requires setuptools-scm to be installed as we will use it to setup the module version
function(set_version target)
    find_package(Python QUIET COMPONENTS Interpreter)
    set(GET_VERSION_SCRIPT "
from setuptools_scm import get_version
print(get_version(), end='')"
    )
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c ${GET_VERSION_SCRIPT}
        WORKING_DIRECTORY ${${PROJECT_NAME}_SOURCE_DIR}
        ERROR_VARIABLE error
        OUTPUT_VARIABLE GIT_VERSION
        RESULT_VARIABLE exit_code
    )
    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR
            "The build process depends on setuptools-scm, make sure it is installed. "
            "Got the following error:\n${error}"
        )
    endif()
    target_compile_definitions(${target} PUBLIC PYPROJECT_VERSION=${GIT_VERSION})
endfunction()
