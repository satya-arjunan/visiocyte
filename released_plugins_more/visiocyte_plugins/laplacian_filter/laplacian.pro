
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS       += laplacianfilterplugin.h

SOURCES       = laplacianfilterplugin.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(laplacianfilter)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/Laplacian_Filter

