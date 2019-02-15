file(GLOB MTImaging_SRCS "*.cpp")
include_directories( ${VISIOCYTE_BINARY_DIR}/visiocytebase )
add_library( multithreadimageIO ${MTImaging_SRCS} )
add_dependencies(multithreadimageIO visiocytebase1)

