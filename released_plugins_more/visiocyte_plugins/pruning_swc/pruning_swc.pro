
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEPATH/common_lib/include


HEADERS	+= pruning_swc_plugin.h
SOURCES	+= pruning_swc_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= my_surf_objs.cpp


TARGET	= $$qtLibraryTarget(pruning_swc)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_utilities/pruning_swc_simple/
