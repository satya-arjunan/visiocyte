
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS	= sort_plugin.h
HEADERS	+= sort_func.h
HEADERS += sort_swc.h
HEADERS += openSWCDialog.h

SOURCES	= sort_plugin.cpp
SOURCES	+= sort_func.cpp
SOURCES += openSWCDialog.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(sort_neuron_swc)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/neuron_utilities/sort_neuron_swc/
