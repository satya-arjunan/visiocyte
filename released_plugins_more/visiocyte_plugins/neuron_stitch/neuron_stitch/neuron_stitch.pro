
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/3drenderer/
INCLUDEPATH += ../

HEADERS	+= neuron_stitch_plugin.h \
    ../neuron_stitch_func.h \
    ../ui_neuron_geometry_dial.h \
    ../neuron_geometry_dialog.h \
    ../marker_match_dialog.h \
    ../neuron_match_clique.h \
    ../performance_timer.h \
    neuron_match_gmhash.h
    #$$VISIOCYTEPATH/visiocyte_main/3drenderer/visiocyter_mainwindow.h
SOURCES	+= neuron_stitch_plugin.cpp \
    ../neuron_geometry_dialog.cpp \
    $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.cpp \
    ../neuron_stitch_func.cpp \
    ../marker_match_dialog.cpp \
    ../neuron_match_clique.cpp \
    ../performance_timer.cpp \
    neuron_match_gmhash.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(neuron_stitch)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_stitch/1_stitchers/
