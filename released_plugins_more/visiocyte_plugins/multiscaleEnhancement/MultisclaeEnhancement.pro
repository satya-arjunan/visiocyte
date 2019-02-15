

TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH += $$VISIOCYTEMAINPATH/jba/newmat11

macx{
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib_mac64 -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
#    CONFIG += x86_64
}

win32 {
    contains(QMAKE_HOST.arch, x86_64) {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibtiff
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibnewmat
    } else {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibtiff
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibnewmat
    }
}

unix:!macx {
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -ltiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
}


INCLUDEPATH += main

HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.h
HEADERS	+= MultisclaeEnhancement_plugin.h
HEADERS += ../neurontracing_vn2/vn_imgpreprocess.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/vcdiff.h



SOURCES	+= MultisclaeEnhancement_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES	+=  ../../../hackathon/zhi/APP2_large_scale/readrawfile_func.cpp
SOURCES += ../neurontracing_vn2/vn_imgpreprocess.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/vcdiff.cpp


SOURCES += ../neurontracing_vn2/app2/my_surf_objs.cpp


TARGET	= $$qtLibraryTarget(multiscaleEnhancement)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/image_analysis/muitiscaleEnhancement
