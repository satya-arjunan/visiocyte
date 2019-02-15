find_package(OpenGL REQUIRED)

set(3DRenderer_SRCS
  GLee_r.c
  barFigureDialog.cpp
  # glsl_r.cpp
  gradients.cpp
  # visiocyte_hoverpoints.cpp
  ItemEditor.cpp
  marchingcubes.cpp
  renderer.cpp
  renderer_gl2.cpp
  renderer_hit.cpp
  renderer_labelfield.cpp
  renderer_obj.cpp
  renderer_tex.cpp
  test_main.cpp
  visiocyter_colormapDialog.cpp
  # visiocyter_control_signal.cpp
  visiocyter_glwidget.cpp
  visiocyter_mainwindow.cpp
  visiocyter_surfaceDialog.cpp
  nstroke.cpp
  nstroke_tracing.cpp
  ../neuron_toolbox/visiocyte_neurontoolbox.cpp
  )

# QT4_ADD_RESOURCES(VISIOCYTE_RENDERER_RCC_SRCS "3drenderer.qrc")

set(3DRENDERER_MOC_SRCS
  gradients.h
  ItemEditor.h
  visiocyter_colormapDialog.h
  visiocyter_glwidget.h
  visiocyter_mainwindow.h
  visiocyter_surfaceDialog.h
  )

if(NOT Qt5Core_FOUND)
  QT4_WRAP_CPP(QT_MOC_SRCS ${3DRENDERER_MOC_SRCS})
else()
  QT5_WRAP_CPP(QT_MOC_SRCS ${3DRENDERER_MOC_SRCS})
endif()


include_directories( ../basic_c_fun )
include_directories( ../terafly/src )
include_directories( ${VISIOCYTE_BINARY_DIR}/visiocytebase )
include_directories( ${CMAKE_BINARY_DIR}/cellseg )

add_library( 3drenderer
    ${QT_MOC_SRCS}
    # ${VISIOCYTE_RENDERER_RCC_SRCS} # no, this needs to be linked directly to visiocyte executable
    ${3DRenderer_SRCS})

target_link_libraries( 3drenderer ${QT_QTOPENGL_LIBRARY} ${OPENGL_LIBRARIES} )
target_link_libraries( 3drenderer imaging VISIOCYTEInterface)

add_dependencies(3drenderer visiocytebase)
add_dependencies(3drenderer VISIOCYTEInterface)

# Avoid link error on Mac Snow Leopard
if(APPLE)
  target_link_libraries( 3drenderer "-framework CoreServices" )
endif()
