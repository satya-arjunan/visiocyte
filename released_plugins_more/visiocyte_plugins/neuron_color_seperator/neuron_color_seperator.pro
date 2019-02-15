
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= neuronPicker_plugin.h \
    neuronpicker_func.h \
    neuronpicker_templates.h \
    neuron_seperator_explorer.h
SOURCES	+= neuronPicker_plugin.cpp \
    neuronpicker_func.cpp \
    neuron_seperator_explorer.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(neuron_color_seperator)
DESTDIR = $$VISIOCYTEPATH/bin/plugins/neuron_utilities/neuron_color_seperator/
