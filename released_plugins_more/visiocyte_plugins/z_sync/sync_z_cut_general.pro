
TEMPLATE = lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun

HEADERS	+= sync_z_cut_plugin.h
SOURCES	+= sync_z_cut_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(sync_z_cut)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/Sync_Views/sync_z_cut
