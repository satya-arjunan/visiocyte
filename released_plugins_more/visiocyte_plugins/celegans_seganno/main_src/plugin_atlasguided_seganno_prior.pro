
TEMPLATE      = lib
CONFIG       += plugin 
CONFIG       += x86_64

INCLUDEPATH  += ../../visiocyte_main/basic_c_fun
INCLUDEPATH  += ../../visiocyte_main/jba/newmat11

LIBS 	     += -L../../visiocyte_main/common_lib/lib -lvisiocytetiff
LIBS         += -L../../visiocyte_main/jba/c++ -lvisiocytenewmat

FORMS         = paradialog_prior.ui

HEADERS      += ../../visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS	     += ../../visiocyte_main/basic_c_fun/stackutil.h
HEADERS      += ../../visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS      += ../../visiocyte_main/basic_c_fun/mg_utilities.h
HEADERS      += ../../visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS      += ../common/q_imresize.h
HEADERS      += q_atlasguided_seganno.h
HEADERS      += q_paradialog_prior.h
HEADERS      += plugin_atlasguided_seganno_prior.h

SOURCES      += ../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES      += q_imresize.cpp
SOURCES      += q_atlasguided_seganno.cpp
SOURCES      += q_paradialog_prior.cpp
SOURCES      += plugin_atlasguided_seganno_prior.cpp

TARGET        = $$qtLibraryTarget(plugin_atlasguided_seganno_prior)


DESTDIR       = ../../../trunk/visiocyte/plugins/unfinished/atlasguided_seganno_prior/ #win32 qmake couldn't handle space in path

