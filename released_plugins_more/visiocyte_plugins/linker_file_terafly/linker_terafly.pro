
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/visiocyte_main/common_lib/include

HEADERS	+= linker_terafly_plugin.h
HEADERS	+= ../../../hackathon/zhi/IVSCC_sort_swc/openSWCDialog.h

SOURCES	+= linker_terafly_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= ../../../hackathon/zhi/IVSCC_sort_swc/openSWCDialog.cpp


TARGET	= $$qtLibraryTarget(linker_terafly)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/linker_file/linker_terafly/
