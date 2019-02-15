
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/common_lib/include

HEADERS	+= histogram_plugin.h
HEADERS	+= histogram_func.h
HEADERS	+= histogram_gui.h

SOURCES	= histogram_plugin.cpp
SOURCES	+= histogram_func.cpp
SOURCES	+= histogram_gui.cpp

SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(histogram)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/image_analysis/histogram/
