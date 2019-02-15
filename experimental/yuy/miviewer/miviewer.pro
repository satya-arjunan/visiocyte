
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
CONFIG       += x86_64

INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun
INCLUDEPATH  += ../../../visiocyte_main/common_lib/include

HEADERS       = miviewer.h \
		../../../visiocyte_main/basic_c_fun/stackutil.h \
		../../../visiocyte_main/basic_c_fun/mg_utilities.h \
		../../../visiocyte_main/basic_c_fun/mg_image_lib.h

SOURCES       += miviewer.cpp \
		../../../visiocyte_main/basic_c_fun/stackutil.cpp \
		../../../visiocyte_main/basic_c_fun/mg_utilities.cpp \
		../../../visiocyte_main/basic_c_fun/mg_image_lib.cpp

LIBS         += -lm -lpthread
LIBS	     += -L../../../visiocyte_main/common_lib/lib -lvisiocytetiff

TARGET        = $$qtLibraryTarget(miviewer)
DESTDIR       = ../../../visiocyte/plugins/miviewer

