
TEMPLATE      = lib
CONFIG       += plugin 
#CONFIG       += x86_64
CONFIG += qt warn_off
CONFIG -= app_bundle

VISIOCYTEMAINPATH  = ../../../visiocyte_main
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH  += $$VISIOCYTEMAINPATH/jba/newmat11

unix {
    LIBS         += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat
     }


win32 {
    contains(QMAKE_HOST.arch, x86_64) {
    LIBS         += -L$$VISIOCYTEMAINPATH/common_lib/winlib64 -llibnewmat
    } else {
    LIBS         += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibnewmat
    }
}

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.h
HEADERS      += q_warp_affine_tps.h
HEADERS      += q_warp_affine_tps_dialogs.h
HEADERS      += plugin_warp_affine_tps.h

SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES      += q_warp_affine_tps.cpp
SOURCES      += q_warp_affine_tps_dialogs.cpp
SOURCES      += plugin_warp_affine_tps.cpp

TARGET        = $$qtLibraryTarget(plugin_warp_affin_tps)

DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_registration/alignment_affine_TPS_warp  #win32 qmake couldn't handle space in path
#DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_utilities/alignment_affine_TPS_warp  #win32 qmake couldn't handle space in path #by Lei @20120722

