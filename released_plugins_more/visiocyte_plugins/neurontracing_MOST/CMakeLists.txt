include_directories( "${VISIOCYTEMAINDIR}/visiocyte" )
include_directories( "${VISIOCYTEMAINDIR}/neuron_editing" )

include_directories( "${VISIOCYTE_BUILD_DIR}/visiocyte_main/visiocyte" ) # for the ui_*.h file 

find_path(QT4_DEMOS_INCLUDE_DIR
   arthurwidgets.h
   PATHS
         ${QT_LIBRARY_DIR}/..
         /Developer/Examples/Qt
   /usr/lib64/qt4
   /usr/lib/qt4
   /usr/local/Trolltech/Qt-${QTVERSION}
   "C:/Qt/${QTVERSION}"
   PATH_SUFFIXES
   demos/shared
   Demos/shared
    )
include_directories(${QT4_DEMOS_INCLUDE_DIR})

set(PLUGIN_DIRECTORY_NAME mostVesselTracer/ )
set(PLUGIN_SOURCES mostVesselTracer.cpp mostimage.cpp voxelcluster.cpp srb.c tools.cpp
	"${VISIOCYTEMAINDIR}/neuron_editing/v_neuronswc.cpp"   "${VISIOCYTEMAINDIR}/neuron_editing/neuron_format_converter.cpp")
set(PLUGIN_HEADERS mostVesselTracer.h mostimage.h voxelcluster.h srb.h tools.h)

configure_visiocyte_plugin(mostVesselTracer)
