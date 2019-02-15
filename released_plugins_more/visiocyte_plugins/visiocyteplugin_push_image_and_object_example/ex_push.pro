
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += x86_64
INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun 
HEADERS       = ex_push.h
SOURCES       = ex_push.cpp
SOURCES      += ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
TARGET        = $$qtLibraryTarget(ex_push)
DESTDIR       = ../../../../visiocyte_external/bin/plugins/Visiocyte_PluginInterface_Demos/3D_viewer_data_push_and_display

