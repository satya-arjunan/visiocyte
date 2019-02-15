
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = F:/newvisiocytecode/visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include

HEADERS	+= TipDetection_plugin.h
SOURCES	+= TipDetection_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(TipDetection)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/bigneuronhackathon/TipDetection/
