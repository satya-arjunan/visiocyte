
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEPATH/common_lib/include

HEADERS	+= inter_node_pruning_plugin.h
HEADERS      += ../sort_neuron_swc/openSWCDialog.h

SOURCES	+= inter_node_pruning_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= my_surf_objs.cpp
SOURCES      += ../sort_neuron_swc/openSWCDialog.cpp


TARGET	= $$qtLibraryTarget(inter_node_pruning)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_utilities/inter_node_pruning/
