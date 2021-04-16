add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC include)

install(DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/openmm-dlext/common"
    FILES_MATCHING PATTERN "*.h"
)
