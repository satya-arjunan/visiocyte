
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS       = istitch.h 
HEADERS      +=	istitch_gui.h
HEADERS      +=	y_imglib.h

SOURCES       = istitch.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp

win32{
    LIBS	     += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibfftw3f-3
#    LIBS	     += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibfftw3l-3
#    LIBS	     += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibfftw3-3
    }
unix{
     LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f
     LIBS	     += -lvisiocytefftw3f -lvisiocytefftw3f_threads
     LIBS         += -lpthread
}

TARGET        = $$qtLibraryTarget(Janelia_imageStitch)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/FlyWorkstation_utilities/Janelia_istitch

RESOURCE     += istitch.qrc

