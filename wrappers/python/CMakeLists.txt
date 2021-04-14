find_python_for_openmm()

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(SWIG REQUIRED COMPONENTS python)

include(${PROJECT_MODULE_PATH}/Fetchpybind11.cmake)

# We build just a minimal SWIG wrapper for the DLExt::Force class
# to make it compatible with the SWIG wrapper of OpenMM::Force
set(SWIG_WRAPPER "DLExtForceSWIG.cpp")

add_custom_command(
    OUTPUT ${SWIG_WRAPPER}
    COMMAND ${SWIG_EXECUTABLE}
        -python -c++ -o ${SWIG_WRAPPER}
        "-I${OpenMM_INCLUDE_DIR}"
        "-I${PROJECT_SOURCE_DIR}/openmmapi/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/DLExtForce.i"
)
add_custom_target(PySWIGWrapper DEPENDS ${SWIG_WRAPPER})

# The name of the module should match the one declared in the SWIG headers
set(PySWIG_MODULE_NAME "_dlpack_extension_swig")

Python_add_library(${PySWIG_MODULE_NAME} MODULE "")
add_dependencies(${PySWIG_MODULE_NAME} PySWIGWrapper)

target_sources(${PySWIG_MODULE_NAME} PRIVATE ${SWIG_WRAPPER})
target_compile_features(${PySWIG_MODULE_NAME} PRIVATE cxx_std_11)
target_link_libraries(${PySWIG_MODULE_NAME} PRIVATE ${PROJECT_NAME})

# We then build a Python API for the rest of the main library with pybind11
# The name of the module should match the one declared within the source code
set(pybind11_MODULE_NAME "dlpack_extension")

pybind11_add_module(${pybind11_MODULE_NAME} MODULE "")

target_sources(${pybind11_MODULE_NAME} PRIVATE PyDLExt.cpp)
target_compile_features(${pybind11_MODULE_NAME} PRIVATE cxx_std_11)
target_link_libraries(${pybind11_MODULE_NAME} PRIVATE ${PROJECT_NAME})
