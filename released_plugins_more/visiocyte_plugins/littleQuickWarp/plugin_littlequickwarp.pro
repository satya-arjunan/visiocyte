# littlequickwarp
# by Lei Qu
# 2012-07-16

TEMPLATE	 = lib
CONFIG		+= qt plugin warn_off
#CONFIG		+= x86_64

VISIOCYTEMAINPATH   = ../../../visiocyte_main
JBADIR        = ../jba

INCLUDEPATH  += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINPATH/jba/newmat11
INCLUDEPATH  += $$JBADIR/
INCLUDEPATH  += $$VISIOCYTEMAINPATH/jba/c++

macx{
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib_mac64 -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
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
    LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -lvisiocytetiff
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}

FORMS         = q_paradialog_littlequickwarp.ui

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINPATH//basic_c_fun/basic_surf_objs.h
HEADERS	     += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINPATH/jba/c++/jba_mainfunc.h
HEADERS      += $$VISIOCYTEMAINPATH/jba/c++/jba_match_landmarks.h
HEADERS      += $$JBADIR/remove_nonaffine_points.cpp
HEADERS      += q_paradialog_littlequickwarp.h
HEADERS      += q_interpolate.h
HEADERS      += q_bspline.h
HEADERS      += plugin_littlequickwarp.h

SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINPATH//basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES      += $$JBADIR/jba_mainfunc.cpp
SOURCES      += $$JBADIR/jba_match_landmarks.cpp
SOURCES      += $$JBADIR/remove_nonaffine_points.cpp
SOURCES      += q_paradialog_littlequickwarp.cpp
SOURCES      += q_interpolate.cpp
SOURCES      += q_bspline.cpp
SOURCES      += q_imgwarp_tps_quicksmallmemory.cpp
SOURCES      += plugin_littlequickwarp.cpp


TARGET	= $$qtLibraryTarget(littlequickwarp)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/image_registration/littleQuickWarp/
