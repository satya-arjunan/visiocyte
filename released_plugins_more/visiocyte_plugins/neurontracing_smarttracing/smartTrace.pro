
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
VISIOCYTETOOLPATH = ../../..
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH += $$VISIOCYTEMAINPATH/jba/newmat11
INCLUDEPATH += ./

TARGET	= $$qtLibraryTarget(smartTrace)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/smartTrace/

macx{
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib_mac64 -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
    CONFIG += x86_64
}

win32{
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibtiff
}

unix:!macx {
    #LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -ltiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}


#===============src=================================================
HEADERS += src/nt_selfcorrect_func.h
HEADERS	+= smartTrace_plugin.h

SOURCES	+= smartTrace_plugin.cpp
SOURCES += src/nt_selfcorrect_func.cpp


#===============visiocyte===============================================
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.h

SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp

#===============mRMR================================================
HEADERS += mrmr/gnu_getline.h \
    mrmr/nrutil.h \
    mrmr/mrmr.h

SOURCES += mrmr/sort2.cpp \
    mrmr/crank.cpp \
    mrmr/gnu_getline.c \
    mrmr/mrmr.cpp \
    mrmr/nrutil.cpp \
    mrmr/pbetai.cpp

#===============SVM=================================================
SOURCES += svm/svm.cpp

HEADERS += svm/svm.h

#===============Hang topology analysis==============================
HEADERS += hang/common_macro.h
HEADERS += hang/fastmarching_linker.h
HEADERS += hang/fastmarching_tree.h
HEADERS += hang/heap.h
HEADERS += hang/simple_c.h
HEADERS += hang/topology_analysis.h
HEADERS += hang/my_surf_objs.h

SOURCES += hang/simple_c.cpp
SOURCES += hang/my_surf_objs.cpp

#===============Radius Estimation===================================
INCLUDEPATH += $$VISIOCYTETOOLPATH/released_plugins/visiocyte_plugins/neurontracing_vn2/neuron_radius
HEADERS += $$VISIOCYTETOOLPATH/released_plugins/visiocyte_plugins/neurontracing_vn2/neuron_radius/marker_radius.h
