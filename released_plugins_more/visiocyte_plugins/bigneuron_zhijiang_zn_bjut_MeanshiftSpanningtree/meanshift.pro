#an example plugin project file
TEMPLATE        = lib
CONFIG  += qt plugin warn_off
#QT += core gui widgets
#CONFIG += x86_64

#set the Visiocyte main path
VISIOCYTEMAINPATH     =  ../../../../visiocyte_external/visiocyte_main

#include necessary paths
INCLUDEPATH     += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEMAINPATH/common_lib/include

win32{
    LIBS +=  -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibtiff
}

macx {
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib_mac64 -lvisiocytetiff
}



#include the headers used in the project
HEADERS += \
    meanshift_plugin.h \
    meanshift_func.h \
    node.h \
../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app2/heap.h\
$$VISIOCYTEMAINPATH/neuron_editing/v_neuronswc.h\
../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app2/my_surf_objs.h

   
#include the source files used in the project
SOURCES = \
    meanshift_plugin.cpp \
    meanshift_func.cpp \
    node.cpp
SOURCES +=
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.cpp
SOURCES += ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app2/my_surf_objs.cpp


#specify target name and directory
TARGET  = $$qtLibraryTarget(meanshift)
DESTDIR = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_tracing/BJUT_meanshift/

