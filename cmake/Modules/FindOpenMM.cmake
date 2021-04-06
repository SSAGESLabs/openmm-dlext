#  FindOpenMM
#  ----------
#
#  OpenMM_INCLUDE_DIR   --  Path to the OpenMM headers
#  OpenMM_LIBRARY       --  Path to the main OpenMM library
#  OpenMM_CUDA_LIBRARY  --  Path to the OpenMMCUDA library (if available)
#  OpenMM_FOUND
#

find_path(OpenMM_INCLUDE_DIR
  NAMES OpenMMException.h
  PATH_SUFFIXES openmm
)
find_library(OpenMM_LIBRARY NAMES OpenMM)
find_library(OpenMM_CUDA_LIBRARY NAMES OpenMMCUDA)

mark_as_advanced(OpenMM_INCLUDE_DIR OpenMM_LIBRARY OpenMM_CUDA_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenMM
  REQUIRED_VARS OpenMM_INCLUDE_DIR OpenMM_LIBRARY
)

if(OpenMM_FOUND AND NOT TARGET OpenMM::OpenMM)
  add_library(OpenMM::OpenMM UNKNOWN IMPORTED)
  set_target_properties(OpenMM::OpenMM PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${OpenMM_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OpenMM_INCLUDE_DIR}"
  )
  target_include_directories(OpenMM::OpenMM INTERFACE
    "${OpenMM_INCLUDE_DIR}/common"
    "${OpenMM_INCLUDE_DIR}/cpu"
    "${OpenMM_INCLUDE_DIR}/cuda"
    "${OpenMM_INCLUDE_DIR}/opencl"
    "${OpenMM_INCLUDE_DIR}/reference"
    "${OpenMM_INCLUDE_DIR}/internal"
  )
endif()
