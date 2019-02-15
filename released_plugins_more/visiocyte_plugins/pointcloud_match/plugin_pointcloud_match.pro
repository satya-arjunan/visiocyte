
TEMPLATE      = lib
CONFIG       += plugin warn_off

VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINPATH/jba/newmat11


win32 {
    contains(QMAKE_HOST.arch, x86_64) {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibnewmat
    } else {
    LIBS     += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibnewmat
    }
}

unix {
    LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
}

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.h
HEADERS      += src/q_pointcloud_match_basic.h
HEADERS      += src/q_pointcloud_match_initialmatch.h
HEADERS      += src/q_pointcloud_match_refinematch_manifold.h
HEADERS      += src/q_pointcloud_match_refinematch_affine.h
HEADERS      += src/q_pointcloud_match.h
HEADERS      += src/q_pointcloud_match_dialogs.h
HEADERS      += src/plugin_pointcloud_match.h

SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
#SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/io_ano_file.cpp
SOURCES      += src/q_pointcloud_match_basic.cpp
SOURCES      += src/q_pointcloud_match_initialmatch.cpp
SOURCES      += src/q_pointcloud_match_refinematch_manifold.cpp
SOURCES      += src/q_pointcloud_match_refinematch_affine.cpp
SOURCES      += src/q_pointcloud_match.cpp
SOURCES      += src/q_pointcloud_match_dialogs.cpp
SOURCES      += src/plugin_pointcloud_match.cpp

TARGET        = $$qtLibraryTarget(plugin_pointcloud_match)

DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_registration/pointcloud_matcher/ 
#win32 qmake couldn't handle space in path

