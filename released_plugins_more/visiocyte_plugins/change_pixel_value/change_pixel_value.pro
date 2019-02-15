
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH += main 

HEADERS += updatepxlvalplugin.h
SOURCES  = updatepxlvalplugin.cpp

SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(updatepxlval)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/pixel_intensity/Change_single_pixel_value

