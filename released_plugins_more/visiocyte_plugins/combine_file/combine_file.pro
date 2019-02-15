
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun

HEADERS	+= combine_file_plugin.h
HEADERS      += ../sort_neuron_swc/openSWCDialog.h

SOURCES	+= combine_file_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app2/my_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES      += ../sort_neuron_swc/openSWCDialog.cpp



TARGET	= $$qtLibraryTarget(combine_file)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/misc/combine_file/
