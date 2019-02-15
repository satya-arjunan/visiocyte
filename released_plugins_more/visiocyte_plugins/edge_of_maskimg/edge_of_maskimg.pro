
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH += main 

HEADERS       += edge_of_maskimg.h
SOURCES       = edge_of_maskimg.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(edge_of_maskimg)   #this should be the project name, i.e. the name of the .pro file
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_edge_detection/Edge_Extraction_from_Mask_Image    #better set a subdirectory here so that the plugin will be arranged nicely 
