
TEMPLATE = lib
CONFIG	 += qt plugin warn_off


VISIOCYTEPATH   = ../../../..//visiocyte_external
VISIOCYTEMAINPATH = ../../../..//visiocyte_external/visiocyte_main
INCLUDEPATH += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH += $$VISIOCYTEPATH/visiocyte_main/jba/newmat11


macx{
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}

win32 {
    contains(QMAKE_HOST.arch, x86_64) {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibnewmat
    }
    else {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibnewmat
    }
}

unix:!macx{
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}


HEADERS	+= NeuronPopulator_plugin.h
HEADERS	+= populate_neurons.h
HEADERS	+= ../neuron_image_profiling/openSWCDialog.h
HEADERS	+= ../affine_transform_swc/apply_transform_func.h
HEADERS	+= ../affine_transform_swc/io_affine_transform.h
HEADERS	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.h

SOURCES	+= NeuronPopulator_plugin.cpp
SOURCES	+= populate_neurons.cpp
SOURCES	+= ../neuron_image_profiling/openSWCDialog.cpp
SOURCES	+= ../affine_transform_swc/apply_transform_func.cpp
SOURCES	+= ../affine_transform_swc/io_affine_transform.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp



TARGET	= $$qtLibraryTarget(NeuronPopulator)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/AllenApps/neuron_populator/
