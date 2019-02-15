
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/visiocyte_main/common_lib/include
INCLUDEPATH	+= ./

HEADERS	+= ../neurontracing_vn2/neuron_radius/neuron_radius_plugin.h
HEADERS	+= ../neurontracing_vn2/neuron_radius/my_surf_objs.h
HEADERS	+= ../neurontracing_vn2/neuron_radius/marker_radius.h 
SOURCES	+= ../neurontracing_vn2/neuron_radius/neuron_radius_plugin.cpp
SOURCES += ../neurontracing_vn2/neuron_radius/my_surf_objs.cpp

SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(neuron_radius)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/neuron_radius/
