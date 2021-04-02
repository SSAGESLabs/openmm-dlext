cmake_minimum_required(VERSION 3.1..3.19)
#
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Set-up project
project(OpenMMDLExt LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

# We need to know where OpenMM is installed so we can access the headers and libraries.

# Specify the C++ version we are building for.
set(CMAKE_CXX_STANDARD 11)

# Set flags for linking on mac
if(APPLE)
    set(CMAKE_INSTALL_NAME_DIR "@rpath")
    set(EXTRA_COMPILE_FLAGS "-msse2 -stdlib=libc++")
endif()

# Select where to install

# Put all the tests and libraries in a single output directory.

# The source is organized into subdirectories, but we handle them all from
# this CMakeLists file rather than letting CMake visit them as SUBDIRS.
set(PLUGIN_SOURCE_SUBDIRS openmmapi)

# These are all the places to search for header files which are to be part of the API.
set(API_INCLUDE_DIRS "openmmapi/include" "openmmapi/include/internal")

# Locate header files.

# Create the library.

# Install headers

# Build the implementations for different platforms

add_subdirectory(platforms/common)

find_package(CUDA QUIET)

if(CUDA_FOUND)
    set(BUILD_CUDA_LIB ON CACHE BOOL "Build implementation for CUDA")
else()
    set(BUILD_CUDA_LIB OFF CACHE BOOL "Build implementation for CUDA")
endif()

if(BUILD_CUDA_LIB)
    add_subdirectory(platforms/cuda)
endif()

# Build the Python wrappers

find_package(Python)
find_package(SWIG)

if(Python_FOUND AND SWIG_FOUND)
    set(BUILD_PYTHON_WRAPPERS ON CACHE BOOL "Build wrappers for Python")
else()
    set(BUILD_PYTHON_WRAPPERS OFF CACHE BOOL "Build wrappers for Python")
endif()

if(BUILD_PYTHON_WRAPPERS)
    add_subdirectory(wrappers/python)
endif(BUILD_PYTHON_WRAPPERS)
