#last change by Hanchuan Peng, 2011-08-27 for porting this plugin to visiocyte main release folder

TEMPLATE      = lib
CONFIG       += plugin 
#CONFIG       += x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINDIR/jba/newmat11
INCLUDEPATH += $$VISIOCYTEMAINDIR/common_lib/include

unix {
#	LIBS += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
	LIBS += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat
}

win32 {
#	LIBS 	     += -L$$VISIOCYTEMAINDIR/common_lib/winlib -llibtiff
        LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibnewmat
}

FORMS         = paradialog.ui

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
#HEADERS	     += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
#HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
#HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/jba/c++/convert_type2uint8.h
HEADERS      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.h
HEADERS      += q_morphology.h
HEADERS      += q_principalskeleton_detection.h
HEADERS      += q_celegans_straighten.h
HEADERS      += q_paradialog_straighten.h
HEADERS      += plugin_celegans_straighten.h

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.cpp
SOURCES      += ../celegans_seganno/main_src/q_imresize.cpp
SOURCES      += q_morphology.cpp
SOURCES      += q_principalskeleton_detection.cpp
SOURCES      += q_celegans_straighten.cpp
SOURCES      += q_paradialog_straighten.cpp
SOURCES      += plugin_celegans_straighten.cpp

TARGET        = $$qtLibraryTarget(celegans_straighten)

DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/celegans/celegans_straighten/ #win32 qmake couldn't handle space in path


