
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= shape_extractor_plugin.h \
    shape_dialog.h \
    shape_extr_template.h \
    extract_fun.h
SOURCES	+= shape_extractor_plugin.cpp \
    shape_dialog.cpp \
    extract_fun.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(shape_extractor)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/shape_analysis/shape_extractor/
