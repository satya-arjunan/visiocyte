
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
INCLUDEPATH  += ../../../../visiocyte_external/visiocyte_main/basic_c_fun
INCLUDEPATH  += ../../../../visiocyte_external/visiocyte_main/common_lib/include

HEADERS       = movieZCswitch.h
SOURCES       = movieZCswitch.cpp
SOURCES       +=  ../../../../visiocyte_external/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(movieZCswitch)
DESTDIR       = ../../../../visiocyte_external/bin/plugins/data_type/5D_Stack_Converter



