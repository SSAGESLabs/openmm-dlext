target_sources(${PROJECT_NAME}
    PRIVATE ContextView.cpp DLExtForce.cpp DLExtForceImpl.cpp
)

if(BUILD_CUDA_LIB)
    target_sources(${PROJECT_NAME}CUDA
        PRIVATE ContextView.cpp DLExtForce.cpp DLExtForceImpl.cpp
    )
endif()
