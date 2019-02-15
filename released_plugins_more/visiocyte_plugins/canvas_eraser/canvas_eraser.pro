# a demo program written by Hanchuan Peng
# 2009-08-14
# updated by Jianlong Zhou 2012-04-18


TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH += main 

HEADERS += canvas_eraser.h

SOURCES  = canvas_eraser.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(canvas_eraser)   #this should be the project name, i.e. the name of the .pro file
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/pixel_intensity/Canvas_Eraser #better set a subdirectory here so that the plugin will be arranged nicely 
