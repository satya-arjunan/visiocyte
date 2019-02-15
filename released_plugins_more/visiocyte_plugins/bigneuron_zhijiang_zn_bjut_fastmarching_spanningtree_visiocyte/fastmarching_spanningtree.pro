
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../../visiocyte_external/visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun 
INCLUDEPATH  += $$VISIOCYTEMAINPATH/common_lib/include

#include the headers used in the project
HEADERS	+= fastmarching_spanningtree_plugin.h
HEADERS += \
    myplugin_func.h \
    global.h \
    ../../../bigneuron_ported/APP2_ported/heap.h \
    tree.h \
    graph.h \
    node.h

SOURCES	+= fastmarching_spanningtree_plugin.cpp
SOURCES	+= myplugin_func.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp


TARGET	= $$qtLibraryTarget(fastmarching_spanningtree)
DESTDIR = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_tracing/BJUT_fastmarching_spanningtree
