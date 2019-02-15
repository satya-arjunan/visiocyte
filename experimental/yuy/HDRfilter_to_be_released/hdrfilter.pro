
TEMPLATE      = lib
CONFIG       += qt plugin warn_off

QT += opengl

VISIOCYTEMAINDIR = ../../../visiocyte_main

# cross-OS-platform, cross-Qt-version
QT_DIR = $$dirname(QMAKE_QMAKE)/..  # cross-Qt-version

SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets

INCLUDEPATH  += $$SHARED_FOLDER	
INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include
INCLUDEPATH  += $$QT_DIR
INCLUDEPATH  += $$VISIOCYTEMAINDIR/visiocyte

macx {
    # Mac possible location of arthurwidgets.h with official Qt 4.7 installer
    QTINST_SHARED_FOLDER = /Developer/Examples/Qt/Demos/shared
    include($$QTINST_SHARED_FOLDER/shared.pri)
    INCLUDEPATH += $$QTINST_SHARED_FOLDER
    LIBS += -L$$QTINST_SHARED_FOLDER
}

HEADERS      += hdrfilter.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/NaZStackWidget.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h

HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/BrightnessCalibrator.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/NaViewer.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/Na2DViewer.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/MouseClickManager.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/geometry/CameraModel.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_annotator/geometry/Rotation3D.h

SOURCES      += hdrfilter.cpp
SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/NaZStackWidget.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp

SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/BrightnessCalibrator.cpp
SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/NaViewer.cpp
SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/Na2DViewer.cpp
SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/gui/MouseClickManager.cpp
SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/geometry/CameraModel.cpp
SOURCES      += $$VISIOCYTEMAINDIR/neuron_annotator/geometry/Rotation3D.cpp

LIBS         += -lm -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
LIBS         += -L$$SHARED_FOLDER 

TARGET        = $$qtLibraryTarget(hdrfilter)
DESTDIR       = ../../visiocyte/plugins/image_filters/HDR_Filter

