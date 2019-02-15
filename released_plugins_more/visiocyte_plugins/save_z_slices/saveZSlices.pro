
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH += $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEPATH/common_lib/include

HEADERS	+= saveZSlices_plugin.h
SOURCES	+= saveZSlices_plugin.cpp

SOURCES += $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_4dimage_create.cpp

TARGET	= $$qtLibraryTarget(saveZSlices)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/data_IO/save_Z_Slices/

