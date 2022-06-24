function(fetch_pybind11 ver)
    CPMAddPackage(NAME pybind11
        VERSION         ${ver}
        GIT_REPOSITORY  https://github.com/pybind/pybind11.git
        GIT_TAG         v${ver}
        GIT_SHALLOW     TRUE
        DOWNLOAD_ONLY   TRUE
    )
    add_subdirectory(${pybind11_SOURCE_DIR} "${PROJECT_BINARY_DIR}/extern/pybind11")
endfunction()

find_package(pybind11 2.6.2 QUIET)

if(pybind11_FOUND)
    message(STATUS "Found pybind11: ${pybind11_DIR} (version ${pybind11_VERSION})")
else()
    fetch_pybind11(2.9.2)
endif()
