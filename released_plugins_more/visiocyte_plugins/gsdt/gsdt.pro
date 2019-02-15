
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH += $$VISIOCYTEMAINPATH/jba/newmat11
macx{
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
#    CONFIG += x86_64
}

win32{
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibnewmat
}

unix:!macx {
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}

HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.h
INCLUDEPATH += main 

HEADERS += main/heap.h
HEADERS += main/fastmarching_dt.h

HEADERS	= gsdt_plugin.h
HEADERS	+= gsdt_func.h

SOURCES	= gsdt_plugin.cpp
SOURCES	+= gsdt_func.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.cpp

HEADERS += common_dialog.h

TARGET	= $$qtLibraryTarget(gsdt)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/Grayscale_Image_Distance_Transform

