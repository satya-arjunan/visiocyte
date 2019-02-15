
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= cropImageTrace_plugin.h \
    crop_dialog.h
SOURCES	+= cropImageTrace_plugin.cpp \
    crop_dialog.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp \
           $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(cropImageTrace)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/image_geometry/cropImageTrace/
