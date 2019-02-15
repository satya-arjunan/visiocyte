
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun

HEADERS	+= sync_swc_terafly_plugin.h

SOURCES	+= sync_swc_terafly_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(sync_swc_terafly)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/Sync_Views/sync_swc_terafly/
