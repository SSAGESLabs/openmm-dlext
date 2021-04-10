if(NOT pybind11_ROOT)
    if(DEFINED ENV{CONDA_PREFIX})
        set(pybind11_ROOT ${CONDA_PREFIX})
    elseif(DEFINED ENV{CONDA_EXE})
        get_filename_component(CONDA_BIN_DIR $ENV{CONDA_EXE} DIRECTORY)
        get_filename_component(pybind11_ROOT ${CONDA_BIN_DIR} DIRECTORY)
    endif()
endif()

find_path(pybind11_INCLUDE_DIR
    NAMES "pybind11/pybind11.h"
    HINTS "${pybind11_ROOT}/include" "$ENV{pybind11_ROOT}/include"
)

function(fetch_pybind11 ver)
    CPMFindPackage(NAME pybind11
        VERSION         ${ver}
        GIT_REPOSITORY  https://github.com/pybind/pybind11.git
        GIT_TAG         v${ver}
        GIT_SHALLOW     TRUE
        DOWNLOAD_ONLY   TRUE
    )
    set(pybind11_INCLUDE_DIR "${pybind11_SOURCE_DIR}/include" PARENT_SCOPE)
endfunction()

if(NOT pybind11_INCLUDE_DIR)
    set(pybind11_FALLBACK_VERSION 2.6.2)
    fetch_pybind11(${pybind11_FALLBACK_VERSION})
endif()
