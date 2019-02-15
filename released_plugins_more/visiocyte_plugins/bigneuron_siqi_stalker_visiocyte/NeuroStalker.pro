
TEMPLATE = lib
CONFIG	+= qt plugin warn_off

macx{
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    ITKLIBPATH = lib/ITKlibs_MAC
}
else{
    ITKLIBPATH = lib/ITKlibs_Linux
    SOURCES = lib/ITK_include/itkLightProcessObject.cxx
}

VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include
INCLUDEPATH += lib/ITK_include
INCLUDEPATH += lib/Filters

LIBS += -L$$ITKLIBPATH -litksys-4.5 -lITKCommon-4.5 -lITKStatistics-4.5 -lITKIOImageBase-4.5 -litkdouble-conversion-4.5
LIBS += -L$$ITKLIBPATH -lvnl_algo -lvnl -lv3p_netlib
LIBS += -L$$VISIOCYTEPATH/visiocyte_main/jba/c++ -lvisiocytenewmat

HEADERS	+= NeuroStalker_plugin.h\
           PressureSampler.h\
           DandelionTracer.h
HEADERS += utils/vn_imgpreprocess.h\
           utils/matmath.h

HEADERS += test/unittest.h

HEADERS += lib/ImageOperation.h\
           lib/PointOperation.h\
           lib/SnakeOperation.h

HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/stackutil.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_utilities.h

SOURCES	+= NeuroStalker_plugin.cpp\
           PressureSampler.cpp\
           DandelionTracer.cpp\
           utils/vn_imgpreprocess.cpp\
           utils/matmath.cpp\
           lib/ImageOperation.cpp\
           lib/PointOperation.cpp\
           lib/SnakeOperation.cpp\
           test/unittest.cpp\
           lib/ITK_include/vcl_deprecated.cxx

SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_4dimage_create.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_utilities.cpp

LIBS         += -lm -L$$VISIOCYTEPATH/visiocyte_main/common_lib/lib -lvisiocytetiff
LIBS         += -lpthread
LIBS	     += -lvisiocytefftw3f -lvisiocytefftw3f_threads

TARGET	= $$qtLibraryTarget(NeuroStalker)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/NeuroStalker/
