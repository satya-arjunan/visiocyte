
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
#INCLUDEPATH	+= $$VISIOCYTEMAINPATH/3drenderer

HEADERS	+= saveToanoPlugin_plugin.h
SOURCES	+= saveToanoPlugin_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp


TARGET	= $$qtLibraryTarget(save3dviewer2linker)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/linker_file/Save_3Dviewer_Content_to_Linker_File/
