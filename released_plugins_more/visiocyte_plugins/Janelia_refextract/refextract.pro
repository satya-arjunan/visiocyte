
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS     = refextract.h

SOURCES     = refextract.cpp
SOURCES     +=$$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp


LIBS         += -lpthread
#LIBS	     += -lvisiocytefftw3f -lvisiocytefftw3f_threads

TARGET        = $$qtLibraryTarget(Janelia_refExtract)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/FlyWorkstation_utilities/Janelia_refExtract


