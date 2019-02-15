
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/neuron_editing
INCLUDEPATH += ../

HEADERS	+= file_transform_plugin.h \
    ../neuron_stitch_func.h \
    $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.h
SOURCES	+= file_transform_plugin.cpp \
    ../image_transform_and_combine_by_affine_mat_func.cpp \
    $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_xforms.cpp \
    ../neuron_stitch_func.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(file_transform)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_stitch/3_file_transform/
