
TEMPLATE      = lib
CONFIG       += plugin warning_off 
INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun
HEADERS       = extrafiltersplugin.h
SOURCES       = extrafiltersplugin.cpp
TARGET        = $$qtLibraryTarget(extrafilters)
DESTDIR       = ../../../../visiocyte_external/bin/plugins/Visiocyte_PluginInterface_Demos/Single_Image_Interface #win32 qmake couldn't handle space in path

