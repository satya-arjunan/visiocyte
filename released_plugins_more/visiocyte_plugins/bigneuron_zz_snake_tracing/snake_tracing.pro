
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off

mac{
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    ITKLIBPATH = ITKlibs_MAC
}
else{
    ITKLIBPATH = ITKlibs_Linux
    SOURCES = ITK_include/itkLightProcessObject.cxx
}

VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte $$VISIOCYTEPATH/basic_c_fun $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH     += ITK_include
INCLUDEPATH     +=  TracingCore/Filters/

LIBS += -L$$ITKLIBPATH -litksys-4.5 -lITKCommon-4.5 -lITKStatistics-4.5 -lITKIOImageBase-4.5 -litkdouble-conversion-4.5
LIBS += -L$$ITKLIBPATH -lvnl_algo -lvnl -lv3p_netlib
LIBS += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat

HEADERS	+= snake_tracing_plugin.h
SOURCES	+= snake_tracing_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp

SOURCES += TracingCore/ImageOperation.cpp\
           TracingCore/PointOperation.cpp\
           TracingCore/SnakeOperation.cpp\
           OpenSnakeTracer.cpp\
           ITK_include/vcl_deprecated.cxx

TARGET	= $$qtLibraryTarget(snake_tracing)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/Visiocyte-FarSight_snake_tracing/
