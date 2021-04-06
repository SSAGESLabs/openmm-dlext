cmake_minimum_required(VERSION 3.12..3.20)
#
#if(${CMAKE_VERSION} VERSION_LESS 3.12)
#  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
#endif()

# Set-up project
project(OpenMMDLExt LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(OpenMM REQUIRED)

if(OpenMM_CUDA_LIBRARY)
  set(OPENMM_BUILD_CUDA_LIB ON CACHE BOOL "Build with CUDA support")
else()
  set(OPENMM_BUILD_CUDA_LIB OFF CACHE BOOL "Build with CUDA support")
endif()

# Create the library
add_library(${PROJECT_NAME} SHARED)

set(OPENMMAPI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/openmmapi")
set(PLATFORMS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/platforms")

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${OPENMMAPI_DIR}/include"
    "${PLATFORMS_DIR}/common/include"
)

target_sources(${PROJECT_NAME}
  PUBLIC
    "${OPENMMAPI_DIR}/src/ContextView.cpp"
    "${OPENMMAPI_DIR}/src/DLExtForce.cpp"
  PRIVATE
    "${OPENMMAPI_DIR}/src/DLExtForceImpl.cpp"
    "${PLATFORMS_DIR}/common/src/DLExtKernelFactory.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMM::OpenMM)

# Install

# Build the Python wrappers

#if(BUILD_PYTHON_WRAPPERS)
#  find_package(Python 3)
#  find_package(SWIG)
#  add_subdirectory(wrappers/python)
#endif()
