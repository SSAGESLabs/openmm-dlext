find_package(Python 3 REQUIRED COMPONENTS Interpreter Development)
find_package(SWIG REQUIRED COMPONENTS python)

set(SWIG_WRAPPER "DLExtForceSWIG.cpp")

add_custom_command(
    OUTPUT ${SWIG_WRAPPER}
    COMMAND ${SWIG_EXECUTABLE}
        -python -c++ -o ${SWIG_WRAPPER}
        "-I${OpenMM_INCLUDE_DIR}"
        "-I${PROJECT_SOURCE_DIR}/openmmapi/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/DLExtForce.i"
)
add_custom_target(SWIGWrapper DEPENDS ${SWIG_WRAPPER})

Python_add_library("${PROJECT_NAME}SWIG" MODULE "")
add_dependencies("${PROJECT_NAME}SWIG" SWIGWrapper)

target_sources("${PROJECT_NAME}SWIG" PRIVATE ${SWIG_WRAPPER})
target_compile_features("${PROJECT_NAME}SWIG" PRIVATE cxx_std_11)
target_link_libraries("${PROJECT_NAME}SWIG" PRIVATE ${PROJECT_NAME})
