
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS      += ifusion.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h

SOURCES      += ifusion.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp

win32{
    LIBS	     += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibfftw3f-3
    }

unix{
    LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f
    LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f_threads
    LIBS         += -lm -lpthread
}


TARGET        = $$qtLibraryTarget(Janelia_ifusion)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/FlyWorkstation_utilities/Janelia_ifusion

