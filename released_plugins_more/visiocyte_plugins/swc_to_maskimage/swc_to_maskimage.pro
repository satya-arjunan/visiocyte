TEMPLATE      = lib
CONFIG       += qt plugin warn_off

VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.h \
    filter_dialog.h
HEADERS      += swc_to_maskimage.h

SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp \
    filter_dialog.cpp
SOURCES      += swc_to_maskimage.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(swc_to_maskimage_sphere)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_utilities/swc_to_maskimage_sphere_unit
