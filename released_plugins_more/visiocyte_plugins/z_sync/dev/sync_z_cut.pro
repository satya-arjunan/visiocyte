
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
#VISIOCYTEPATH = D:\VISIOCYTE_STUFF\visiocyte_external
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= sync_z_cut_plugin.h
SOURCES	+= sync_z_cut_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(sync_z_cut)
DESTDIR = $$VISIOCYTEPATH/bin/plugins/sync_z_cut
#DESTDIR	= D:\VISIOCYTE_STUFF\visiocyte_win7_32bit_v2.707\plugins\sync_z_cut
