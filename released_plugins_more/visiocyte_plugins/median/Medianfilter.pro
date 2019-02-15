
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS	+= Medianfilter_plugin.h
SOURCES	+= Medianfilter_plugin.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(medianfilter)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/median_filter/
