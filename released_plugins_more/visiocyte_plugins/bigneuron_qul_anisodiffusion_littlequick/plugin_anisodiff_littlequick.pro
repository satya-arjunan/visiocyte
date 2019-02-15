# 3D anisotropic diffusion plugin for neuron reconstruction
# by Lei Qu
# 2015-03-16

TEMPLATE	= lib
CONFIG		+= qt plugin warn_off
#CONFIG		+= x86_64

SRCFOLDER = ../bigneuron_qul_anisodiffusion/src/
VISIOCYTEMAINPATH   = $$SRCFOLDER/../../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun

unix {
	LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -lvisiocytetiff
}

win32 {
	LIBS 	+= -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibtiff 
}

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.h
HEADERS	     += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.h
HEADERS      += $$SRCFOLDER/q_AnisoDiff3D.h
HEADERS      += $$SRCFOLDER/q_derivatives3D.h
HEADERS      += $$SRCFOLDER/EigenDecomposition3.h
HEADERS      += $$SRCFOLDER/q_EigenVectors3D.h
HEADERS      += $$SRCFOLDER/q_imgaussian3D.h
HEADERS      += $$SRCFOLDER/plugin_anisodiff_littlequick.h

SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINPATH//basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES      += $$SRCFOLDER/q_AnisoDiff3D.cpp
SOURCES      += $$SRCFOLDER/q_derivatives3D.cpp
SOURCES      += $$SRCFOLDER/EigenDecomposition3.cpp
SOURCES      += $$SRCFOLDER/q_EigenVectors3D.cpp
SOURCES      += $$SRCFOLDER/q_imgaussian3D.cpp
SOURCES      += $$SRCFOLDER/plugin_anisodiff_littlequick.cpp


TARGET	= $$qtLibraryTarget(anisodiff_littlequick)
#DESTDIR	= ../../visiocyte/plugins/anisodiff/anisodiff_littlequick/
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/anisotropic_filter/anisodiff_littlequick/
