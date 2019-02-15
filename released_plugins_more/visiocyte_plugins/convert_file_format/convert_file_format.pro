
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= convert_file_format_plugin.h
SOURCES	+= convert_file_format_plugin.cpp

SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(convert_file_format)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/data_IO/convert_file_format/

