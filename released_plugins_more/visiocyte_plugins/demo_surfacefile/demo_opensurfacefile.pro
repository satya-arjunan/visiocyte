
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external 
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= demo_opensurfacefile_plugin.h
SOURCES	+= demo_opensurfacefile_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(demo_opensurfacefile)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/Visiocyte_PluginInterface_Demos/demo_opensurfacefile/
