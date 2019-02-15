
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS	+= consensus_skeleton_plugin.h
HEADERS	+= consensus_skeleton_func.h

SOURCES	= consensus_skeleton_plugin.cpp
SOURCES	+= consensus_skeleton_func.cpp
SOURCES	+= consensus_skeleton.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(consensus_skeleton)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/neuron_utilities/consensus_skeleton_via_clustering/
