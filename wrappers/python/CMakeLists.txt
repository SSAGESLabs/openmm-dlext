option(BUILD_PYTHON_WRAPPERS "Build Python wrappers" ON)

if(BUILD_PYTHON_WRAPPERS)
    find_package(Python 3 REQUIRED)
    find_package(SWIG REQUIRED)
endif()
