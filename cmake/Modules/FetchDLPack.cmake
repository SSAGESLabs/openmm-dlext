#if(NOT DLPack_ROOT)
#    if(DEFINED ENV{CONDA_PREFIX})
#        set(DLPack_ROOT ${CONDA_PREFIX})
#    elseif(DEFINED ENV{CONDA_EXE})
#        get_filename_component(CONDA_BIN_DIR $ENV{CONDA_EXE} DIRECTORY)
#        get_filename_component(DLPack_ROOT ${CONDA_BIN_DIR} DIRECTORY)
#    endif()
#endif()

#find_path(DLPack_INCLUDE_DIR
#    NAMES "dlpack/dlpack.h"
#    HINTS "${DLPack_ROOT}/include" "$ENV{DLPack_ROOT}/include"
#)

function(fetch_dlpack ver)
    CPMFindPackage(NAME DLPack
        VERSION         ${ver}
        GIT_REPOSITORY  https://github.com/dmlc/dlpack.git
        GIT_TAG         v${ver}
        GIT_SHALLOW     TRUE
        DOWNLOAD_ONLY   TRUE
        OPTIONS         "BUILD_MOCK OFF"
    )
    set(DLPack_INCLUDE_DIR "${DLPack_SOURCE_DIR}/include" PARENT_SCOPE)
endfunction()

if(NOT DLPack_INCLUDE_DIR)
    set(DLPack_FALLBACK_VERSION 0.4)
    fetch_dlpack(${DLPack_FALLBACK_VERSION})
endif()
