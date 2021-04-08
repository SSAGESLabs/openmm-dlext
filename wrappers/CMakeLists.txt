option(BUILD_PYTHON_WRAPPERS "Build Python wrappers" ON)

if(BUILD_PYTHON_WRAPPERS)
    add_subdirectory(python)
endif()
