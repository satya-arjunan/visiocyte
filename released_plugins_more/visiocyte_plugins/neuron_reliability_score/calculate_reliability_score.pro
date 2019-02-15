
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/neuron_editing

INCLUDEPATH += $$VISIOCYTEMAINPATH/jba/newmat11

macx{
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib_mac64 -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
    CONFIG += x86_64
}

win32{
}

unix:!macx {
    #LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -ltiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}

HEADERS	+= calculate_reliability_score_plugin.h
SOURCES	+= calculate_reliability_score_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/neuron_editing/neuron_format_converter.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/neuron_editing/v_neuronswc.cpp

HEADERS += src/common_macro.h
HEADERS += src/fastmarching_linker.h
HEADERS += src/fastmarching_tree.h
HEADERS += src/heap.h
HEADERS += src/simple_c.h
HEADERS += src/topology_analysis.h
HEADERS += src/my_surf_objs.h

SOURCES += src/simple_c.cpp
SOURCES += src/my_surf_objs.cpp

TARGET	= $$qtLibraryTarget(calculate_reliability_score)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/calculate_reliability_score/
