
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS      += y_lsminfo.h
HEADERS      += imageblend.h
HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h

SOURCES      += imageblend.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += basic_4dimage_ori.cpp

LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/src_packages/mylib_tiff -lmylib
LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f
LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytefftw3f_threads
LIBS         += -lm -lpthread

TARGET        = $$qtLibraryTarget(Janelia_blend_multiscanstacks)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/FlyWorkstation_utilities/Janelia_blend_multiscanstacks

