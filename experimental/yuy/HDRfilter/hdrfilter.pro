
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
CONFIG       += x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun

HEADERS      += hdrfilter.h 
HEADERS      += hdrfilter_gui.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h

SOURCES      += hdrfilter.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp

LIBS         += -lm -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff

TARGET        = $$qtLibraryTarget(hdrfilter)
DESTDIR       = ../../visiocyte/plugins/image_filters/HDR_Filter

