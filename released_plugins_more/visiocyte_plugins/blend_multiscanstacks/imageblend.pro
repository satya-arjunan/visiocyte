
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS      += imageblend.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/vcdiff.h

SOURCES      += imageblend.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/vcdiff.cpp

win32{
    LIBS	     += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibfftw3f-3
}

unix{
    LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f
    LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f_threads
    LIBS         += -lm -lpthread
}

TARGET        = $$qtLibraryTarget(blend_multiscanstacks)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/image_blending/blend_multiscanstacks

