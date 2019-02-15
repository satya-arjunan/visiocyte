
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= ./
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include

HEADERS	+= Advantra_plugin.h \
    nf_dialog.h \
    toolbox.h \
    node.h \
    btracer.h \
    connected.h
SOURCES	+= Advantra_plugin.cpp \
    btracer.cpp \
    node.cpp \
    toolbox.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(Advantra)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/Advantra/
