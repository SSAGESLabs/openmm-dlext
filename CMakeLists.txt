cmake_minimum_required(VERSION 3.13..3.20)
#
#if(${CMAKE_VERSION} VERSION_LESS 3.12)
#  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
#endif()

# Set-up project
project(OpenMMDLExt LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(OpenMM REQUIRED)

# Create the library
add_library(${PROJECT_NAME} SHARED "")

add_subdirectory(openmmapi)
add_subdirectory(platforms)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMM::OpenMM)

# Install

# Build the Python wrappers
#if(BUILD_PYTHON_WRAPPERS)
#  find_package(Python 3)
#  find_package(SWIG)
#  add_subdirectory(wrappers/python)
#endif()
