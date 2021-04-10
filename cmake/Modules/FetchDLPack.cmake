function(fetch_dlpack ver)
    CPMFindPackage(NAME DLPack
        VERSION         ${ver}
        GIT_REPOSITORY  https://github.com/dmlc/dlpack.git
        GIT_TAG         v${ver}
        GIT_SHALLOW     TRUE
        DOWNLOAD_ONLY   TRUE
    )
    set(DLPack_SOURCE_DIR "${DLPack_SOURCE_DIR}" PARENT_SCOPE)
endfunction()

if(NOT DLPack_SOURCE_DIR)
    set(DLPack_FALLBACK_VERSION 0.4)
    fetch_dlpack(${DLPack_FALLBACK_VERSION})
endif()

set(BUILD_MOCK OFF CACHE BOOL "Do not build DLPack mock target" FORCE)
add_subdirectory(${DLPack_SOURCE_DIR} "${PROJECT_BINARY_DIR}/extern/dlpack")
