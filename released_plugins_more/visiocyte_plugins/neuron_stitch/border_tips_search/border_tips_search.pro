
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH += ../
#INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/3drenderer/

HEADERS	+= border_tips_plugin.h
HEADERS	+= ../neuron_stitch_func.h
HEADERS	+= ../neuron_tipspicker_dialog.h
    #$$VISIOCYTEPATH/visiocyte_main/3drenderer/visiocyter_mainwindow.h
SOURCES	+= border_tips_plugin.cpp
SOURCES	+= ../neuron_stitch_func.cpp
SOURCES	+= ../neuron_tipspicker_dialog.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(search_border_tips)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_stitch/2_search_border_tips/
