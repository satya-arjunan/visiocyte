
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS	= marker2others_plugin.h

SOURCES	= marker2others_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(marker2others)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/marker_utilities/marker2others/
