
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH += ../../../released_plugins/visiocyte_plugins/sort_neuron_swc

HEADERS	+= subtree_labelling_plugin.h \
    subtree_dialog.h \
#    ../../../released_plugins/visiocyte_plugins/sort_neuron_swc/sort_swc.h
#    ../../../released_plugins/visiocyte_plugins/sort_neuron_swc/openSWCDialog.h

SOURCES	+= subtree_labelling_plugin.cpp \
    subtree_dialog.cpp \
#    ../../../released_plugins/visiocyte_plugins/sort_neuron_swc/openSWCDialog.cpp \
#    ../../../released_plugins/visiocyte_plugins/sort_neuron_swc/sort_swc.cpp

SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(subtree_labelling)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/subtree_labelling/
