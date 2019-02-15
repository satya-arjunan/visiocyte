
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH += $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS	+= ZMovieMaker_plugin.h
SOURCES	+= ZMovieMaker_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(ZMovieMaker)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/movies_and_snapshots/ZMovieMaker/
