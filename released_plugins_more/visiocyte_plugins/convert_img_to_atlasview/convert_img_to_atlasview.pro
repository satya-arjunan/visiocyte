
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS       = convert_img_to_atlasview.h

SOURCES       = convert_img_to_atlasview.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(convert_img_to_atlasview)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/data_type/Convert_Image_to_AtlasViewMode

