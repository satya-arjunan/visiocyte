
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/common_lib/include

HEADERS	+= cropped3DImageSeries_plugin.h
HEADERS	+= ../../../hackathon/zhi/APP2_large_scale/readRawfile_func.h
HEADERS	+= ../../../released_plugins/visiocyte_plugins/istitch/y_imglib.h


SOURCES	+= cropped3DImageSeries_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_4dimage_create.cpp


SOURCES	+=  ../../../hackathon/zhi/APP2_large_scale/readrawfile_func.cpp
SOURCES      +=	$$VISIOCYTEPATH/basic_c_fun/mg_utilities.cpp
SOURCES      +=	$$VISIOCYTEPATH/basic_c_fun/mg_image_lib.cpp

win32{
LIBS         += -L$$VISIOCYTEPATH/common_lib/winlib64 -llibtiff
}

unix{
LIBS         += -lpthread
LIBS	     += -lvisiocytefftw3f -lvisiocytefftw3f_threads
LIBS         += -lm -L$$VISIOCYTEPATH/common_lib/lib -lvisiocytetiff
}

TARGET	= $$qtLibraryTarget(cropped3DImageSeries)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/image_geometry/crop3d_image_series
