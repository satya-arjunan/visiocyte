
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#QT+=opengl
#CONFIG      += x86_64

VISIOCYTEMAINPATH = ../../../visiocyte_main
QT_DIR = $$[QT_INSTALL_PREFIX]

INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun 
INCLUDEPATH  += $$VISIOCYTEMAINPATH/visiocyte
INCLUDEPATH  += $$VISIOCYTEMAINPATH/neuron_editing
INCLUDEPATH  += $$VISIOCYTEMAINPATH/common_lib/include

USE_Qt5 {
  INCLUDEPATH += $$QT_DIR/lib/QtConcurrent.framework/Versions/5/Headers  # for QtConcurrent, by PHC 2015May
  #SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets
  SHARED_FOLDER = ./painting/shared/ # for arthurwidgets
  include($$SHARED_FOLDER/shared.pri)
  INCLUDEPATH += $$SHARED_FOLDER
  LIBS += -L$$SHARED_FOLDER
} else {
  SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets
  include($$SHARED_FOLDER/shared.pri)
  INCLUDEPATH += $$SHARED_FOLDER
  LIBS += -L$$SHARED_FOLDER
}

HEADERS       = mostimage.h
HEADERS      += tools.h
HEADERS      += mostVesselTracer.h
HEADERS      += voxelcluster.h
HEADERS      += srb.h
HEADERS      += $$VISIOCYTEMAINPATH/neuron_editing/neuron_format_converter.h
HEADERS      += $$VISIOCYTEMAINPATH/neuron_editing/v_neuronswc.h

SOURCES       = mostimage.cpp
SOURCES      += tools.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += mostVesselTracer.cpp
SOURCES      += voxelcluster.cpp
SOURCES      += srb.c
SOURCES      += $$VISIOCYTEMAINPATH/neuron_editing/neuron_format_converter.cpp
SOURCES      += $$VISIOCYTEMAINPATH/neuron_editing/v_neuronswc.cpp



TARGET        = $$qtLibraryTarget(mostVesselTracer)

DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_tracing/MOST_tracing/



