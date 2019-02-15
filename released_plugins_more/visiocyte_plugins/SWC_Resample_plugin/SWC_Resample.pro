
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= SWC_Resample_plugin.h
HEADERS += ../../../released_plugins/visiocyte_plugins/resample_swc/resampling.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.h

SOURCES	+= SWC_Resample_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.cpp

TARGET	= $$qtLibraryTarget(SWC_Resample)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/swc_gui_resample/
