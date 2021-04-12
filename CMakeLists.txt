cmake_minimum_required(VERSION 3.13..3.20)
#
#if(${CMAKE_VERSION} VERSION_LESS 3.12)
#  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
#endif()

# Set-up project
project(OpenMMDLExt LANGUAGES CXX)

set(PROJECT_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_MODULE_PATH})

find_package(OpenMM REQUIRED)

include("${PROJECT_MODULE_PATH}/FetchCPM.cmake")
include("${PROJECT_MODULE_PATH}/FetchDLPack.cmake")

# Create the library
add_library(${PROJECT_NAME} SHARED "")

add_subdirectory(openmmapi)
add_subdirectory(platforms)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMM::OpenMM dlpack::dlpack)

# Build wrappers
add_subdirectory(wrappers)

# Install
include(GNUInstallDirs)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    get_filename_component(OpenMM_ROOT_DIR ${OpenMM_INCLUDE_DIR} DIRECTORY)
    set(CMAKE_INSTALL_PREFIX ${OpenMM_ROOT_DIR} CACHE PATH "" FORCE)
endif()

install(TARGETS ${PROJECT_NAME}
    #EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
)
#install(EXPORT "${PROJECT_NAME}Targets"
#    NAMESPACE "${PROJECT_NAME}::"
#    DESTINATION "lib/cmake/${PROJECT_NAME}"
#)
