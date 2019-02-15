
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEPATH/common_lib/include


HEADERS	+= neurontracing_mip_plugin.h
SOURCES	+= neurontracing_mip_plugin.cpp

SOURCES += my_surf_objs.cpp

SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/stackutil.cpp
SOURCES      +=	$$VISIOCYTEPATH/basic_c_fun/mg_utilities.cpp
SOURCES      +=	$$VISIOCYTEPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += ../../../hackathon/zhi/APP2_large_scale/readrawfile_func.cpp

win32{
    LIBS         += -L$$VISIOCYTEPATH/common_lib/winlib64 -llibtiff
    LIBS	 += -L$$VISIOCYTEPATH/common_lib/winlib64 -llibfftw3f-3
}

unix{
    LIBS         += -lm -L$$VISIOCYTEPATH/common_lib/lib -lvisiocytetiff
    LIBS         += -lpthread
    LIBS	     += -lvisiocytefftw3f -lvisiocytefftw3f_threads
}

TARGET	= $$qtLibraryTarget(neurontracing_mip)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/TReMap/
