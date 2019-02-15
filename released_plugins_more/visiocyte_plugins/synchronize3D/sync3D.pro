
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

win32{
}


INCLUDEPATH += main 


HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.h

HEADERS	+= sync3D_plugin.h
SOURCES	+= sync3D_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.cpp

TARGET	= $$qtLibraryTarget(sync3D)
DESTDIR = $$VISIOCYTEMAINPATH/../bin/plugins/Sync_Views/sync3D/

