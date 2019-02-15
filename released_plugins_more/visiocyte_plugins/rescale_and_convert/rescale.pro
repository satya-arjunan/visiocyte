
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS       += rescale_and_convert.h
SOURCES       += rescale_and_convert.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_4dimage_create.cpp

TARGET        = $$qtLibraryTarget(rescale)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/data_type/intensity_rescale

