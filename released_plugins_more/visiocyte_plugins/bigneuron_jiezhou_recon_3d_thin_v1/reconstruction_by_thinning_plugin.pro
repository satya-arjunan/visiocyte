
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off

mac{
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    ITKLIBPATH = ../bigneuron_zz_snake_tracing/ITKlibs_MAC
}
else{
    ITKLIBPATH = ../bigneuron_zz_snake_tracing/ITKlibs_Linux
    SOURCES = ../bigneuron_zz_snake_tracing/ITK_include/itkLightProcessObject.cxx
}

VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte $$VISIOCYTEPATH/basic_c_fun $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH     += ../bigneuron_zz_snake_tracing/ITK_include


LIBS += -L$$ITKLIBPATH -litksys-4.5 -lITKCommon-4.5 -lITKStatistics-4.5 -lITKIOImageBase-4.5 -litkdouble-conversion-4.5
LIBS += -L$$ITKLIBPATH -lvnl_algo -lvnl -lv3p_netlib
LIBS += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat

HEADERS	+= Point.h
HEADERS += pixPoint.h
HEADERS += reconstruction_by_thinning_plugin_plugin.h
HEADERS += itkBinaryThinningImageFilter3D.h
HEADERS += Edge.h
HEADERS += Vertex.h
HEADERS += Preprocessing.h
SOURCES	+= Point.cxx
SOURCES += pixPoint.cxx
SOURCES += Vertex.cxx
SOURCES += Edge.cxx
SOURCES += reconstruction_by_thinning_plugin_plugin.cpp
SOURCES += itkBinaryThinningImageFilter3D.txx
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp

SOURCES += ../bigneuron_zz_snake_tracing/ITK_include/vcl_deprecated.cxx

TARGET	= $$qtLibraryTarget(SimpleAxisAnalyzer)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/SimpleAxisAnalyzer
