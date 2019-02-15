
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH += main 

HEADERS += ada_threshold.h
SOURCES  = ada_threshold.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(ada_threshold)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_thresholding/Simple_Adaptive_Thresholding
