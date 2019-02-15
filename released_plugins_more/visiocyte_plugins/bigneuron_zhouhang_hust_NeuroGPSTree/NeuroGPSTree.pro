
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
mac{
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}

#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include
INCLUDEPATH	+= ./eigen3.2.2

win32 {
INCLUDEPATH += $$VISIOCYTEPATH/visiocyte_main/common_lib/include/eigen_3_3_4
LIBS += $$VISIOCYTEPATH/visiocyte_main/common_lib/src_packages/Pre-built.2/lib/pthreadVC2.lib
}

unix {
LIBS            += -lpthread  #-lgomp
}

#QMAKE_CXXFLAGS  += -std=c++11

unix:!macx {
    QMAKE_CXXFLAGS  += -std=c++11 -fopenmp
    LIBS            += -lgomp
}

HEADERS	+= NeuroGPSTree_plugin.h \
    Function/binaryfilter.h \
    Function/contourutil.h \
    Function/ineuronio.h \
    Function/ineuronprocessobject.h \
    Function/volumealgo.h \
    Function/Trace/bridgebreaker.h \
    Function/Trace/neurotreecluster.h \
    Function/Trace/tracefilter.h \
    Function/Trace/traceutil.h \
    ngtypes/basetypes.h \
    ngtypes/ineurondataobject.h \
    ngtypes/shape.h \
    ngtypes/soma.h \
    ngtypes/tree.h \
    ngtypes/volume.h \
    ngtypes/volumecreator.h \
    Function/Trace/WeakSWCFilter.h \
    Function/IO/treewriter.h
SOURCES	+= NeuroGPSTree_plugin.cpp \
    Function/binaryfilter.cpp \
    Function/contourutil.cpp \
    Function/volumealgo.cpp \
    Function/Trace/bridgebreaker.cpp \
    Function/Trace/neurotreecluster.cpp \
    Function/Trace/TraceCrossAnalysis.cpp \
    Function/Trace/tracefilter.cpp \
    Function/Trace/traceutil.cpp \
    ngtypes/shape.cpp \
    ngtypes/soma.cpp \
    ngtypes/tree.cpp \
    ngtypes/volume.cpp \
    Function/Trace/WeakSWCFilter.cpp \
    Function/IO/treewriter.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(NeuroGPSTree)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/HUST_NeuroGPSTree/
