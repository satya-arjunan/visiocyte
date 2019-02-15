file(GLOB Imaging_SRCS "*.cpp")
include_directories( ${VISIOCYTE_BINARY_DIR}/visiocytebase )
include_directories( ../basic_c_fun )
add_library( imaging ${Imaging_SRCS} )
add_dependencies(imaging ${TIFF_LIBRARY})

