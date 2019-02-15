# a demo program written by Hanchuan Peng
# 2010-06-26
# updated by J. Zhou 2012-04-17

TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS += reset_xyz_resolution.h
SOURCES  = reset_xyz_resolution.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(example_reset_xyz_resolution)   #this should be the project name, i.e. the name of the .pro file
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_resolution/XYZ_Resolution #better set a subdirectory here so that the plugin will be arranged nicely 
