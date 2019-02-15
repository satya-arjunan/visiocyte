
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH += main 

HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.h
HEADERS += gaussianfilterplugin.h
SOURCES  = gaussianfilterplugin.cpp

SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.cpp

TARGET        = $$qtLibraryTarget(gaussianfilter)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/Gaussian_Filter

