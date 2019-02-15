
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64

VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun

HEADERS	= plugin_creator_plugin.h
HEADERS	+= plugin_creator_func.h
HEADERS	+= plugin_creator_gui.h
HEADERS	+= create_plugin.h
HEADERS += common_dialog.h
HEADERS += produce_simplest_plugin.h

SOURCES	= plugin_creator_plugin.cpp
SOURCES	+= plugin_creator_func.cpp
SOURCES += produce_demo1.cpp
SOURCES += produce_demo2.cpp
SOURCES += produce_simplest_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(plugin_creator)
DESTDIR = $$VISIOCYTEPATH/../bin/plugins/_Visiocyte_plugin_creator
