
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEPATH/common_lib/include

HEADERS	+= mipZSlices_plugin.h
SOURCES	+= mipZSlices_plugin.cpp

HEADERS += $$VISIOCYTEPATH/basic_c_fun/basic_memory.h

SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_4dimage_create.cpp

SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_memory.cpp

TARGET	= $$qtLibraryTarget(mipZSlices)
DESTDIR       =  $$VISIOCYTEPATH/../bin/plugins/image_projection/maximum_intensity_projection_Z_Slices/
