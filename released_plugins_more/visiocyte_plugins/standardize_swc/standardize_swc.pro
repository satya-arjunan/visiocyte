
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun

HEADERS	+= standardize_swc_plugin.h
HEADERS	+= $$VISIOCYTEPATH/../../visiocyte_tools/released_plugins/visiocyte_plugins/resample_swc/resampling.h
HEADERS	+= $$VISIOCYTEPATH/../../visiocyte_tools/released_plugins/visiocyte_plugins/sort_neuron_swc/sort_swc.h



SOURCES	+= standardize_swc_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp


TARGET	= $$qtLibraryTarget(standardize__swc)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_utilities/standardize_swc/
