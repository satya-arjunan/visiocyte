
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../..
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= multi_channel_swc_plugin.h \
    multi_channel_swc_dialog.h \
    multi_channel_swc_template.h \
    multi_channel_swc_func.h

SOURCES	+= multi_channel_swc_plugin.cpp \
    multi_channel_swc_dialog.cpp \
    multi_channel_swc_func.cpp

SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(multi_channel_swc)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/multi_channel_swc/
