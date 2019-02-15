
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off

VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include
LIBS += -L$$VISIOCYTEPATH/visiocyte_main/jba/c++ -lvisiocytenewmat

mac{
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}

unix:!macx {
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS  += -std=c++11
}

# From VISIOCYTE Main
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_memory.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/stackutil.h
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_4dimage_create.cpp
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_utilities.h
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_utilities.cpp

# Main Plugin
HEADERS	+= rivulet.h
SOURCES	+= rivulet_utils.cpp
SOURCES	+= rivulet_tracer.cpp
HEADERS	+= Rivulet_plugin.h
SOURCES	+= Rivulet_plugin.cpp

# Utils
HEADERS += utils/rk4.h
SOURCES += utils/rk4.cpp
HEADERS += utils/graph.h
SOURCES += utils/graph.cpp
HEADERS += utils/marker_radius.h

# Fastmarching
HEADERS += fastmarching/fastmarching_dt.h
HEADERS += fastmarching/msfm.h
SOURCES += fastmarching/msfm.cpp
SOURCES += fastmarching/common.c
HEADERS += fastmarching/my_surf_objs.h
SOURCES += fastmarching/my_surf_objs.cpp

LIBS         += -lm -L$$VISIOCYTEPATH/visiocyte_main/common_lib/lib -lvisiocytetiff
LIBS         += -lpthread
LIBS         += -lvisiocytefftw3f -lvisiocytefftw3f_threads

TARGET	= $$qtLibraryTarget(Rivulet)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/Rivulet2/
