TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH += main 

HEADERS += datatypecnvrt.h 

SOURCES  = datatypecnvrt.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(datatypeconvert)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/data_type/Convert_8_16_32_bits_data
