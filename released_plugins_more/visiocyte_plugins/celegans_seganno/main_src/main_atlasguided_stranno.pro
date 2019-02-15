
TEMPLATE      = app
CONFIG       += qt
macx: CONFIG         -= app_bundle
#CONFIG       += x86_64 #this cannot be added as there will be 32bit compatibility issue. Should add on command line. by PHC, 101223
#QT          -= gui # Only the core module is used

VISIOCYTEMAINDIR = ../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/jba/newmat11

LIBS 	     += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
LIBS         += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
HEADERS	     += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
HEADERS      += $$VISIOCYTEMAINDIR/jba/c++/convert_type2uint8.h
HEADERS      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.h
HEADERS      += ../celegans_straighten/q_morphology.h
HEADERS      += ../celegans_straighten/q_principalskeleton_detection.h
HEADERS      += ../celegans_straighten/q_celegans_straighten.h
HEADERS      += q_atlasguided_seganno.h

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.cpp
SOURCES      += ../celegans_straighten/q_morphology.cpp
SOURCES      += ../celegans_straighten/q_principalskeleton_detection.cpp
SOURCES      += ../celegans_straighten/q_celegans_straighten.cpp
SOURCES      += ../common/q_imresize.cpp
SOURCES      += q_atlasguided_seganno.cpp
SOURCES      += main_atlasguided_stranno.cpp


