#an example plugin project file
TEMPLATE        = lib
CONFIG  += qt plugin warn_off
#QT += core gui widgets
#CONFIG += x86_64

#set the Visiocyte main path
VISIOCYTEMAINPATH     =  ../../../../visiocyte_external/

#include necessary paths
INCLUDEPATH     += $$VISIOCYTEMAINPATH/visiocyte_main/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEMAINPATH/visiocyte_main/common_lib/include
#LIBS +=  -L$$VISIOCYTEMAINPATH/visiocyte_main/common_lib/winlib -llibtiff

#include the headers used in the project
HEADERS += \
    myplugin_func.h \
    myplugin_plugin.h \
    global.h \
    ../../APP2_ported/heap.h \
    tree.h \
    graph.h \
    node.h
HEADERS +=

#include the source files used in the project
SOURCES = \
    myplugin_func.cpp \
    myplugin_plugin.cpp
SOURCES +=
SOURCES += $$VISIOCYTEMAINPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEMAINPATH/visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEMAINPATH/visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEMAINPATH/visiocyte_main/basic_c_fun/basic_memory.cpp


#specify target name and directory
TARGET  = $$qtLibraryTarget(myplugin)
DESTDIR = ../Visiocyte/bin/plugins/myplugin/
