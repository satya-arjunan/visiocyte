
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
INCLUDEPATH	+= ../../../visiocyte_main/basic_c_fun

HEADERS	+= mouse_monitor.h
HEADERS	+= mouse_monitor_plugin.h
HEADERS	+= mouse_monitor_func.h
HEADERS	+= mouse_monitor_gui.h

SOURCES	= mouse_monitor_plugin.cpp
SOURCES	+= mouse_monitor_func.cpp
SOURCES	+= ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(mouse_monitor)
DESTDIR = ../../../../visiocyte_external/bin/plugins/Visiocyte_PluginInterface_Demos/mouse_event_monitor