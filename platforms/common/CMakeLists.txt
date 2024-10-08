add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC include)

if(BUILD_CUDA_LIB)
    target_include_directories(${PROJECT_NAME}CUDA PUBLIC include)
endif()

install(DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/openmm-dlext/common"
    FILES_MATCHING PATTERN "*.h"
)
