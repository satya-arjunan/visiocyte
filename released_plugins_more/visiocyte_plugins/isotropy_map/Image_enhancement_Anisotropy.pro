
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
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

HEADERS	+= Image_enhancement_Anisotropy_plugin.h
SOURCES	+= Image_enhancement_Anisotropy_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(image_anisotropy_map)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/image_analysis/image_anisotropy_map/
