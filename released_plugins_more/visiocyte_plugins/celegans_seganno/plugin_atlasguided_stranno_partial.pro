#LAST CHANGE BY HANCHUAN PENG. 2011-08-27. PORTING TO VISIOCYTE PLUGIN FOLDER


TEMPLATE      = lib
CONFIG       += plugin 
#CONFIG       += x86_64

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/jba/newmat11
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include

unix {
#	LIBS += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
	LIBS += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat
}

win32 {
#	LIBS 	+= -L$$VISIOCYTEMAINDIR/common_lib/winlib -llibtiff
        LIBS += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibnewmat
}

FORMS         = main_src/paradialog_stranno.ui

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
#HEADERS	     += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
#HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
#HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
HEADERS      += $$VISIOCYTEMAINDIR/jba/c++/convert_type2uint8.h
HEADERS      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.h
HEADERS      += ../celegans_straighten/q_morphology.h
HEADERS      += ../celegans_straighten/q_principalskeleton_detection.h
HEADERS      += ../celegans_straighten/q_celegans_straighten.h
HEADERS      += main_src/q_atlasguided_seganno.h
HEADERS      += main_src/q_paradialog_stranno.h
HEADERS      += main_src/plugin_atlasguided_stranno_partial.h

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
#SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.cpp
SOURCES      += ../celegans_straighten/q_morphology.cpp
SOURCES      += ../celegans_straighten/q_principalskeleton_detection.cpp
SOURCES      += ../celegans_straighten/q_celegans_straighten.cpp
SOURCES      += main_src/q_imresize.cpp
SOURCES      += main_src/q_atlasguided_seganno.cpp
SOURCES      += main_src/q_paradialog_stranno.cpp
SOURCES      += main_src/plugin_atlasguided_stranno_partial.cpp

TARGET        = $$qtLibraryTarget(plugin_atlasguided_stranno_partial)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/celegans/atlasguided_seganno/

