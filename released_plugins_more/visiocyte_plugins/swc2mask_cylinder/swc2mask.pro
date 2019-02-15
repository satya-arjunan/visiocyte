TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64

VISIOCYTE_DIR = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTE_DIR/basic_c_fun
INCLUDEPATH     += $$VISIOCYTE_DIR/common_lib/include

HEADERS	+= swc2mask_plugin.h
HEADERS += my_surf_objs.h
HEADERS += swc_convert.h
HEADERS += src/swc2mask.h
HEADERS += $$VISIOCYTE_DIR/basic_c_fun/customary_structs/visiocyte_neurontoolbox_para.h
HEADERS += $$VISIOCYTE_DIR/basic_c_fun/basic_surf_objs.h

SOURCES	+= swc2mask_plugin.cpp
SOURCES += my_surf_objs.cpp
SOURCES += swc_convert.cpp
SOURCES += $$VISIOCYTE_DIR/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTE_DIR/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(swc2mask)
DESTDIR	= $$VISIOCYTE_DIR/../bin/plugins/neuron_utilities/swc_to_maskimage_cylinder_unit/
