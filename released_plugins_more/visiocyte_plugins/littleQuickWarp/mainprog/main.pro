# littlequickwarp
# by Lei Qu
# 2012-07-08

TEMPLATE      = app
CONFIG       += qt 
macx: CONFIG -= app_bundle
CONFIG       += x86_64
#QT          -= gui # Only the core module is used

VISIOCYTEMAINDIR    = ../../../visiocyte_main
JBADIR        = ../../../../brainaligner

INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include
INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/jba/newmat11
INCLUDEPATH  += $$JBADIR/jba/c++/

LIBS 	     += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
LIBS         += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINDIR//basic_c_fun/basic_surf_objs.h
HEADERS	     += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$JBADIR/jba/c++/jba_mainfunc.h
HEADERS      += $$JBADIR/jba/c++/jba_match_landmarks.h
HEADERS      += $$JBADIR/jba/c++/remove_nonaffine_points.cpp
HEADERS      += q_interpolate.h

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINDIR//basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$JBADIR/jba/c++/jba_mainfunc.cpp
SOURCES      += $$JBADIR/jba/c++/jba_match_landmarks.cpp
SOURCES      += $$JBADIR/jba/c++/remove_nonaffine_points.cpp
SOURCES      += q_interpolate.cpp
SOURCES      += q_imgwarp_tps_quicksmallmemory.cpp
SOURCES      += main_imgwarp_tps_quicksmallmemory.cpp


