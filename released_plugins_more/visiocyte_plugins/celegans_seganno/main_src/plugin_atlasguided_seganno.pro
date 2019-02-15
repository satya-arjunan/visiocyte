
TEMPLATE      = lib
CONFIG       += plugin 
CONFIG       += x86_64

INCLUDEPATH  += ../../visiocyte_main/basic_c_fun
INCLUDEPATH  += ../../visiocyte_main/jba/newmat11

LIBS 	     += -L../../visiocyte_main/common_lib/lib -lvisiocytetiff
LIBS         += -L../../visiocyte_main/jba/c++ -lvisiocytenewmat

FORMS         = paradialog.ui

HEADERS      += ../../visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS	     += ../../visiocyte_main/basic_c_fun/stackutil.h
HEADERS      += ../../visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS      += ../../visiocyte_main/basic_c_fun/mg_utilities.h
HEADERS      += ../../visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS      += q_imresize.h
HEADERS      += q_atlasguided_seganno.h
HEADERS      += q_paradialog.h
HEADERS      += plugin_atlasguided_seganno.h

SOURCES      += ../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES      += ../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES      += q_imresize.cpp
SOURCES      += q_atlasguided_seganno.cpp
SOURCES      += q_paradialog.cpp
SOURCES      += plugin_atlasguided_seganno.cpp

TARGET        = $$qtLibraryTarget(plugin_atlasguided_seganno)


DESTDIR       = ../../../../visiocyte_external/bin/plugins/atlasguided_seganno/ #win32 qmake couldn't handle space in path

