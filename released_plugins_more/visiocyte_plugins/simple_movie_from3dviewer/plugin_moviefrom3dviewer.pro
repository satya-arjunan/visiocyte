#created by Lei Qu
# Last change: by Hanchuan Peng, 2010-11-23

TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += x86_64

INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun 

HEADERS       = plugin_moviefrom3dviewer.h
SOURCES       = plugin_moviefrom3dviewer.cpp
SOURCES      += ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
TARGET        = $$qtLibraryTarget(moviefrom3dviewer)

DESTDIR       = ../../../../visiocyte_external/bin/plugins/movies_and_snapshots/Simple_Movie_Maker

