
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= neuron_extractor_plugin.h
SOURCES	+= neuron_extractor_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(neuron_fragment_extractor)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/neuron_fragment_extractor/
