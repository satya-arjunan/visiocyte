
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
PLUGINPATH = ./src/
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH     += $$PLUGINPATH
INCLUDEPATH     += $$PLUGINPATH/..

HEADERS	+= GVFplugin_plugin.h
HEADERS += $$PLUGINPATH/FL_gvfCellSeg.h

SOURCES	+= GVFplugin_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$PLUGINPATH/FL_gvfCellSeg.cpp

FORMS += src/FL_watershedSegPara.ui

TARGET	= $$qtLibraryTarget(gvf_cellseg)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/image_segmentation/Cell_Segmentation_GVF/
