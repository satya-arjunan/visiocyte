
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off

#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/common_lib/include


HEADERS	+= nctuTW_plugin.h \
    Dijkstra.h \
    DistanceTransTwo.h \
    Geo.h \
    Graph_AdjacencyList3D.h \
    Image3D.h \
    IPT.h \
    LineSeg3D.h \
    main.h \
    Point3D.h \
    UnionFind.h \
    neurontracing.h \
    QDlgPara.h
SOURCES	+= nctuTW_plugin.cpp \
    Dijkstra.cpp \
    DistanceTransTwo.cpp \
    Geo.cpp \
    Graph_AdjacencyList3D.cpp \
    Image3D.cpp \
    IPT.cpp \
    LineSeg3D.cpp \
    Point3D.cpp \
    main.cpp \
    QDlgPara.cpp

SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(nctuTW)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/nctuTW/

FORMS += \
    QDlgPara.ui


