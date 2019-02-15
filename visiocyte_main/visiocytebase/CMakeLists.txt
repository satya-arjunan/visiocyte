set(VISIOCYTEBASE_SRCS
  ../visiocyte/visiocyte_version_info.cpp
  ../visiocyte/surfaceobj_geometry_dialog.cpp
  )

set(VISIOCYTEBASE_UI_SRCS
  ../visiocyte/visiocyte_global_preference.ui
  ../visiocyte/template_matching_cellseg.ui
  ../visiocyte/landmark_property.ui
  ../visiocyte/surface_obj_annotation.ui
  ../visiocyte/surfaceobj_geometry_dialog.ui
  ../visiocyte/dialog_update_visiocyte.ui
  ../visiocyte/dialog_update_list.ui
  ../visiocyte/dialog_update_options.ui
  ../visiocyte/dialog_update_downloading.ui
  ../visiocyte/dialog_update_checking.ui
  ../visiocyte/visiocyte_global_preference.ui
  ../visiocyte/dialog_curve_trace.ui
  )

set(VISIOCYTEBASE_MOC_SRCS
  ../visiocyte/surfaceobj_geometry_dialog.h
  ../visiocyte/surfaceobj_annotation_dialog.h
  ../visiocyte/template_matching_cellseg_dialog.h
  ../visiocyte/visiocyte_version_info.h
  )

if(NOT Qt5Core_FOUND)
  QT4_WRAP_UI( QT_UI_SRCS ${VISIOCYTEBASE_UI_SRCS} )
  QT4_WRAP_CPP(QT_MOC_SRCS ${VISIOCYTEBASE_MOC_SRCS})
else()
  QT5_WRAP_UI( QT_UI_SRCS ${VISIOCYTEBASE_UI_SRCS} )
  QT5_WRAP_CPP(QT_MOC_SRCS ${VISIOCYTEBASE_MOC_SRCS})
endif()

include_directories(BEFORE ../basic_c_fun )
include_directories(BEFORE ${VISIOCYTE_BINARY_DIR}/visiocyte)
include_directories(BEFORE ${VISIOCYTE_BINARY_DIR}/visiocytebase)
include_directories(${TIFF_INCLUDE_DIR})

add_library(visiocytebase1 ${QT_UI_SRCS} ${QT_MOC_SRCS} ${VISIOCYTEBASE_SRCS})

target_link_libraries( visiocytebase1 neuron_editing )
target_link_libraries( visiocytebase1 ${QT_QTXML_LIBRARY} )

if(VISIOCYTE_USE_OWN_TIFF)
    add_dependencies(visiocytebase1 CustomTiffLibrary)
endif(VISIOCYTE_USE_OWN_TIFF)

# Dependency of interface on some overly included ui headers (cellseg)
add_dependencies(VISIOCYTEInterface visiocytebase1)

