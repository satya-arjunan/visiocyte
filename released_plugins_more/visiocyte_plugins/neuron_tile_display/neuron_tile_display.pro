
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= neuron_tile_display_plugin.h \
    neuron_tile_display_dialog.h
SOURCES	+= neuron_tile_display_plugin.cpp \
    neuron_tile_display_dialog.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.h
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
HEADERS += $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.h
SOURCES += $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.cpp

TARGET	= $$qtLibraryTarget(neuron_tile_display)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/tile_display_multiple_neurons/
