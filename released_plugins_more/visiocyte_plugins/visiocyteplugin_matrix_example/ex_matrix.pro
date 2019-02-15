
TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += x86_64
INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun
HEADERS       = ex_matrix.h
SOURCES       = ex_matrix.cpp
SOURCES      += ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
TARGET        = $$qtLibraryTarget(ex_matrix)
DESTDIR       = ../../../../visiocyte_external/bin/plugins/Visiocyte_PluginInterface_Demos/call_each_other

