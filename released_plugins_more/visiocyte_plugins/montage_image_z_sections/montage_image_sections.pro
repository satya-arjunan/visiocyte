
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS += montage_image_sections.h
SOURCES  = montage_image_sections.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(montage_image_sections)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_geometry/Montage_All_Z_Sections
