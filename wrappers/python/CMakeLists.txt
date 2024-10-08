check_python_and_openmm()

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(SWIG REQUIRED COMPONENTS python)

set_python_module_path()

# Policies for UseSWIG
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)
include(UseSWIG)
include(${PROJECT_MODULE_PATH}/Fetchpybind11.cmake)

install(DIRECTORY dlext/
    DESTINATION ${OpenMMDLExt_Python_PATH}/openmm/dlext
)
install(DIRECTORY openmm_dlext/
    DESTINATION ${OpenMMDLExt_Python_PATH}/openmm_dlext
)

# We build just a minimal SWIG wrapper for the DLExt::Force class
# to make it compatible with the SWIG wrapper of OpenMM::Force
# The name of the module in the SWIG headers should be "_${PySWIG_MODULE_NAME}"
set(PySWIG_MODULE_NAME "api_swig")

set_property(SOURCE DLExtForce.i PROPERTY CPLUSPLUS ON)
swig_add_library(${PySWIG_MODULE_NAME} TYPE MODULE LANGUAGE python SOURCES DLExtForce.i)

target_include_directories(${PySWIG_MODULE_NAME} SYSTEM PRIVATE ${Python_INCLUDE_DIRS})
target_compile_features(${PySWIG_MODULE_NAME} PRIVATE cxx_std_11)
target_link_libraries(${PySWIG_MODULE_NAME} PRIVATE ${PROJECT_NAME})

if(OpenMM_SIMTK_API)
    target_compile_definitions(${PySWIG_MODULE_NAME} PRIVATE SIMTK_API)
endif()

if(APPLE)
    target_link_options(${PySWIG_MODULE_NAME} PRIVATE "LINKER:-undefined,dynamic_lookup")
endif()

install(TARGETS ${PySWIG_MODULE_NAME}
    DESTINATION ${OpenMMDLExt_Python_PATH}/openmm/dlext
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PySWIG_MODULE_NAME}.py"
    DESTINATION ${OpenMMDLExt_Python_PATH}/openmm/dlext
)

# We then build a Python API for the rest of the main library with pybind11
set(pybind11_MODULE_NAME "_api")

pybind11_add_module(${pybind11_MODULE_NAME}_fallback MODULE "")

set_version(${pybind11_MODULE_NAME}_fallback)
target_sources(${pybind11_MODULE_NAME}_fallback PRIVATE PyDLExt.cpp)
target_compile_features(${pybind11_MODULE_NAME}_fallback PRIVATE cxx_std_11)
target_link_libraries(${pybind11_MODULE_NAME}_fallback PRIVATE ${PROJECT_NAME})

install(TARGETS ${pybind11_MODULE_NAME}_fallback
    DESTINATION ${OpenMMDLExt_Python_PATH}/openmm/dlext
)

# Also build a CUDA variant when possible
if(BUILD_CUDA_LIB)
    pybind11_add_module(${pybind11_MODULE_NAME} MODULE "")

    set_version(${pybind11_MODULE_NAME})
    target_sources(${pybind11_MODULE_NAME} PRIVATE PyDLExt.cpp)
    target_compile_features(${pybind11_MODULE_NAME} PRIVATE cxx_std_11)
    target_link_libraries(${pybind11_MODULE_NAME} PRIVATE ${PROJECT_NAME}CUDA)

    install(TARGETS ${pybind11_MODULE_NAME}
        DESTINATION ${OpenMMDLExt_Python_PATH}/openmm/dlext
    )
endif()
