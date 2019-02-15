TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
CONFIG  -= bundle_off
VISIOCYTEMAINPATH  =  ../../../visiocyte_main   
INCLUDEPATH +=  $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS	+= eswc_converter_plugin.h
HEADERS	+= eswc_converter_func.h

SOURCES	=  eswc_converter_plugin.cpp
SOURCES	+= eswc_converter_func.cpp
SOURCES += eswc_core.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(eswc_converter)
DESTDIR = $$VISIOCYTEMAINPATH/../bin/plugins//neuron_utilities/Enhanced_SWC_Format_Converter

