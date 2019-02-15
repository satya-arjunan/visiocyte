
TEMPLATE = lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../..
VISIOCYTEMAINPATH = $$VISIOCYTEPATH/visiocyte_main
INCLUDEPATH += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH += $$VISIOCYTEMAINPATH/jba/newmat11
INCLUDEPATH += $$VISIOCYTEMAINPATH/jba/c++

HEADERS	+= jba_plugin.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/convert_type2uint8.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/jba_match_landmarks.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/displacefield_comput.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/remove_nonaffine_points.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/histeq.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/seg_fly_brain.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/jba_affine_xform.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/wkernel.h
HEADERS += $$VISIOCYTEMAINPATH/jba/c++/jba_mainfunc.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.h
unix:HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/imageio_mylib.h

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/jba/c++/histeq.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/jba/c++/convert_type2uint8.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/jba/c++/wkernel.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
unix:SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/imageio_mylib.cpp
SOURCES	+= basic_volproc3d.cpp
SOURCES	+= jba_mainfunc.cpp
SOURCES	+= jba_affine_xform.cpp
SOURCES	+= jba_match_landmarks.cpp
SOURCES	+= remove_nonaffine_points.cpp
SOURCES	+= jba_plugin.cpp

win32 {
    contains(QMAKE_HOST.arch, x86_64) {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibnewmat
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibtiff
    } else {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibnewmat
    }
}

unix {
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/src_packages/mylib_tiff -lmylib
}

TARGET	= $$qtLibraryTarget(brainaligner)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/image_registration/brainaligner/

