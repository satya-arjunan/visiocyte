
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS      +=	clonalselect_gui.h
HEADERS      +=	clonalselect_core.h
HEADERS      += clonalselect.h
HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp

SOURCES      += clonalselect_gui.cpp
SOURCES      += clonalselect_core.cpp
SOURCES      += clonalselect.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp

LIBS         += -lm -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff

TARGET        = $$qtLibraryTarget(Janelia_clonalSelect)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/FlyWorkstation_utilities/Janelia_clonalSelect

