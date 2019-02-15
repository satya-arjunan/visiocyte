
TEMPLATE      = lib
CONFIG       += plugin warning_off
#CONFIG	     += X86_64 
INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun
HEADERS       = newwindowplugin.h
SOURCES       = newwindowplugin.cpp
TARGET        = $$qtLibraryTarget(newwindow)
DESTDIR       = ../../../../visiocyte_external/bin/plugins/Visiocyte_PluginInterface_Demos/Multi_Image_Interface #win32 qmake couldn't handle space in path

