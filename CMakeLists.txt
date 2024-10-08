cmake_minimum_required(VERSION 3.16..3.24)

# Set-up project
project(OpenMMDLExt LANGUAGES CXX)

set(PROJECT_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_MODULE_PATH})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(OpenMM REQUIRED)

include(GNUInstallDirs)
include("${PROJECT_MODULE_PATH}/FetchCPM.cmake")
include("${PROJECT_MODULE_PATH}/FetchDLPack.cmake")

# Create the library
add_library(${PROJECT_NAME} SHARED "")
# Also create a CUDA variant when possible
if(BUILD_CUDA_LIB)
    add_library(${PROJECT_NAME}CUDA SHARED "")
endif()

add_subdirectory(openmmapi)
add_subdirectory(platforms)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMM::OpenMM dlpack::dlpack)

if(BUILD_CUDA_LIB)
    target_compile_definitions(${PROJECT_NAME}CUDA PUBLIC OPENMM_BUILD_CUDA_LIB)
    target_link_libraries(${PROJECT_NAME}CUDA PUBLIC
        ${PROJECT_NAME} ${OpenMM_CUDA_LIBRARY}
    )
endif()

# Install
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${OpenMM_INSTALL_PREFIX} CACHE PATH "" FORCE)
endif()

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${OpenMM_INSTALL_LIBDIR}
    #EXPORT "${PROJECT_NAME}Targets"
)
#install(EXPORT "${PROJECT_NAME}Targets"
#    NAMESPACE "${PROJECT_NAME}::"
#    DESTINATION "lib/cmake/${PROJECT_NAME}"
#)

install(DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/openmm-dlext"
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_CUDA_LIB)
    install(TARGETS ${PROJECT_NAME}CUDA
        DESTINATION ${OpenMM_INSTALL_LIBDIR}
    )
endif()

# Build wrappers
add_subdirectory(wrappers)
