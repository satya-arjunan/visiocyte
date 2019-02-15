
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include

HEADERS	+= NeuronChaser_plugin.h \
    btracer.h \
    nf_dialog.h \
    toolbox.h \
    model.h \
    node.h \
    connected.h
SOURCES	+= NeuronChaser_plugin.cpp \
    btracer.cpp \
    toolbox.cpp \
    model.cpp \
    node.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(NeuronChaser)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/NeuronChaser/
