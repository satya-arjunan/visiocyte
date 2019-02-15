
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/common_lib/include


HEADERS	+= mipImageSeries_plugin.h
SOURCES	+= mipImageSeries_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp


TARGET	= $$qtLibraryTarget(mipImageSeries)
DESTDIR =  $$VISIOCYTEPATH/../bin/plugins/image_projection/maximum_intensity_projection_Image_Series/
