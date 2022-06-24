function(fetch_dlpack ver)
    CPMAddPackage(NAME dlpack
        VERSION         ${ver}
        GIT_REPOSITORY  https://github.com/dmlc/dlpack.git
        GIT_TAG         v${ver}
        GIT_SHALLOW     TRUE
        DOWNLOAD_ONLY   TRUE
    )
    set(BUILD_MOCK OFF CACHE BOOL "Do not build DLPack mock target" FORCE)
    add_subdirectory(${dlpack_SOURCE_DIR} "${PROJECT_BINARY_DIR}/extern/dlpack")
endfunction()

find_package(dlpack 0.5 QUIET)

if(dlpack_FOUND)
    message(STATUS "Found dlpack: ${dlpack_DIR} (version ${dlpack_VERSION})")
else()
    fetch_dlpack(0.7)
endif()
