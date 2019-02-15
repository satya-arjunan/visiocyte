
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS	+= neuron_toolbox_plugin.h
HEADERS	+= neuron_toolbox_func.h
HEADERS	+= toolbox_gui.h

SOURCES	= neuron_toolbox_plugin.cpp
SOURCES	+= neuron_toolbox_func.cpp
SOURCES	+= toolbox_gui.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(neuron_toolbox)
DESTDIR        = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_toolbox/

