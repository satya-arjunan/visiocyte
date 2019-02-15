
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS += minMaxfilterplugin.h
SOURCES  = minMaxfilterplugin.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(minMaxfilter)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/min_Max_Filter

