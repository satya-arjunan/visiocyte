
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS	+= resample_swc_plugin.h
HEADERS	+= resample_swc_func.h
HEADERS      += ../sort_neuron_swc/openSWCDialog.h


SOURCES	= resample_swc_plugin.cpp
SOURCES	+= resample_swc_func.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES      += ../sort_neuron_swc/openSWCDialog.cpp


TARGET	= $$qtLibraryTarget(resample_swc)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/neuron_utilities/resample_swc/
