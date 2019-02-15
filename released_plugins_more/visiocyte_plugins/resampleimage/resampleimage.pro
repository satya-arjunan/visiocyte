
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH += $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEPATH/common_lib/include


HEADERS	+= resampleimage_plugin.h
HEADERS	+= ../../../released_plugins/visiocyte_plugins/cellseg_gvf/src/FL_upSample3D.h
HEADERS	+= ../../../released_plugins/visiocyte_plugins/cellseg_gvf/src/FL_downSample3D.h
HEADERS	+= ../../../released_plugins/visiocyte_plugins/istitch/y_imglib.h


SOURCES	+= resampleimage_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(resampleimage)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/image_geometry/image_resample/
