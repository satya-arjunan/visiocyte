
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= neuron_connector_plugin.h \
    neuron_connector_func.h
SOURCES	+= neuron_connector_plugin.cpp \
    neuron_connector_func.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(neuron_connector)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/neuron_connector/
