
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte $$VISIOCYTEPATH/basic_c_fun $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH     += functions


HEADERS	+= PSF_zhi_plugin.h
SOURCES	+= PSF_zhi_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp


SOURCES	+= functions/PreProcessDataImage.cpp
SOURCES	+= functions/Create3DLookUpTable.cpp
SOURCES	+= functions/Parse_Input.cpp
SOURCES += functions/FunctionsForMainCode.cpp
SOURCES += functions/EigenDec_3D.cpp
SOURCES += functions/Gamma.cpp
SOURCES += functions/MatrixMultiplication.cpp
SOURCES += functions/KernelDensityEstimation.cpp
SOURCES += functions/ML_Divide.cpp
SOURCES += functions/EigenDec_2D.cpp
SOURCES += functions/extractTree.cpp

TARGET	= $$qtLibraryTarget(PSF_zhi)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/PSF_tracing/
