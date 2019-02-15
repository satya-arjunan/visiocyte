
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= neuron_color_display_plugin.h
SOURCES	+= neuron_color_display_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(neuron_color_display)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/neuron_color_display/
