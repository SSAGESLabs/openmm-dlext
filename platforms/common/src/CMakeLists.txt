target_sources(${PROJECT_NAME} PRIVATE DLExtKernelFactory.cpp)

if(BUILD_CUDA_LIB)
    target_sources(${PROJECT_NAME}CUDA PRIVATE DLExtKernelFactory.cpp)
endif()
