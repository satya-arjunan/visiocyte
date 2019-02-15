
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH += main 

HEADERS       += channel_split_plugin.h

SOURCES       += channel_split_plugin.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_4dimage_create.cpp

TARGET        = $$qtLibraryTarget(channelsplit)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/color_channel/split_extract_channels

