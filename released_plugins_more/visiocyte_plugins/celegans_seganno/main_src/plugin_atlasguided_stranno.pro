
TEMPLATE      = lib
CONFIG       += plugin 
CONFIG       += x86_64

INCLUDEPATH  += ../../visiocyte_main/basic_c_fun
INCLUDEPATH  += ../../visiocyte_main/jba/newmat11
INCLUDEPATH  += ../../visiocyte_main/common_lib/include

LIBS 	     += -L../../visiocyte_main/common_lib/lib -lvisiocytetiff
LIBS         += -L../../visiocyte_main/jba/c++ -lvisiocytenewmat

FORMS         = paradialog_stranno.ui

HEADERS      += ../../visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS	     += ../../visiocyte_main/basic_c_fun/stackutil.h
HEADERS      += ../../visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS      += ../../visiocyte_main/basic_c_fun/mg_utilities.h
HEADERS      += ../../visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS      += ../../visiocyte_main/worm_straighten_c/spline_cubic.h
HEADERS      += q_imresize.h
HEADERS      += ../celegans_straighten/q_morphology.h
HEADERS      += ../celegans_straighten/q_principalskeleton_detection.h
HEADERS      += ../celegans_straighten/q_celegans_straighten.h
HEADERS      += q_atlasguided_seganno.h
HEADERS      += q_paradialog_stranno.h
HEADERS      += plugin_atlasguided_stranno.h

SOURCES      += ../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES      += ../../visiocyte_main/worm_straighten_c/spline_cubic.cpp
SOURCES      += q_imresize.cpp
SOURCES      += ../celegans_straighten/q_morphology.cpp
SOURCES      += ../celegans_straighten/q_principalskeleton_detection.cpp
SOURCES      += ../celegans_straighten/q_celegans_straighten.cpp
SOURCES      += q_atlasguided_seganno.cpp
SOURCES      += q_paradialog_stranno.cpp
SOURCES      += plugin_atlasguided_stranno.cpp

TARGET        = $$qtLibraryTarget(plugin_atlasguided_stranno)


DESTDIR       = ../../../../visiocyte_external/bin/plugins/atlasguided_stranno/ #win32 qmake couldn't handle space in path

