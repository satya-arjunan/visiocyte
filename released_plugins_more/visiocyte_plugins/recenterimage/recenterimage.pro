# revised by Hanchuan Peng
# 2010-05-20. Note that there is a problem about non-VC compliers (e.g. gcc) may complain the redundant compiling of vcdiff.h/.cpp

TEMPLATE      = lib
CONFIG       += plugin warn_off

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

HEADERS      += recenterimageplugin.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/vcdiff.h
#HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
#HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
#HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
#HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h

SOURCES      += recenterimageplugin.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/vcdiff.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
#SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
#SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.cpp # THIS IS INCLUDED IN THE cpp FILE.
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
#SOURCES      += ../blend_multiscanstacks/basic_4dimage_ori.cpp

#LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib  -lvisiocytetiff
#LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/src_packages/mylib_tiff -lmylib

TARGET        = $$qtLibraryTarget(recenterimage)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/image_geometry/ReCenter_Image

