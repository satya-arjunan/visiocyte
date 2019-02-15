
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH += ../
#INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/3drenderer/

HEADERS	+= file_combine_plugin.h \
    ../neuron_stitch_func.h
    #$$VISIOCYTEPATH/visiocyte_main/3drenderer/visiocyter_mainwindow.h
SOURCES	+= file_combine_plugin.cpp \
    ../neuron_stitch_func.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(file_combine)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_stitch/4_file_combine/
