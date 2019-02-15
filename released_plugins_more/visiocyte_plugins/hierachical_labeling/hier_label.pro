
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun

HEADERS	+= hier_label_plugin.h
HEADERS	+= hier_label_func.h

SOURCES	= hier_label_plugin.cpp
SOURCES	+= hier_label_func.cpp
SOURCES	+= hierachical_labeling.cpp
SOURCES	+= eswc_core.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(hier_label)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_utilities/hierarchical_labeling_of_neuron
