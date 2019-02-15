
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
CONFIG       += release x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS      += ireg.h
HEADERS      += y_NiftiImageIO.h
HEADERS      += y_imgreg.h
HEADERS      += y_img.h
HEADERS      += y_img.hxx

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
unix:HEADERS += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.h

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h

HEADERS      += niftilib/nifti1.h
HEADERS      += niftilib/nifti1_io.h
HEADERS      += niftilib/znzlib.h

SOURCES      += ireg.cpp
SOURCES      += y_NiftiImageIO.cpp

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
unix:SOURCES += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.cpp

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp

SOURCES      += niftilib/nifti1_io.c
SOURCES      += niftilib/znzlib.c

LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/src_packages/mylib_tiff -lmylib
unix:LIBS    += -lm -lpthread -lz
#LIBS	     += -lvisiocytefftw3f -lvisiocytefftw3f_threads

QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

TARGET        = $$qtLibraryTarget(ireg)
#DESTDIR       = ../../visiocyte/plugins/image_registration/ireg
DESTDIR       =$$VISIOCYTEMAINDIR/../bin/plugins/image_registration/ireg
