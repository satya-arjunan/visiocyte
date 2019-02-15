
TEMPLATE      = lib
CONFIG       += plugin 

VISIOCYTEMAINDIR = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/jba/newmat11
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include


macx{
    LIBS += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat
#    CONFIG += x86_64
}


win32{
    LIBS += -L$$VISIOCYTEMAINDIR/common_lib/winlib64 -llibnewmat
}

unix:!macx {
    LIBS += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat
}

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
HEADERS      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.h
HEADERS      += q_morphology.h
HEADERS      += q_principalskeleton_detection.h
HEADERS      += q_neurontree_segmentation.h
HEADERS      += q_skeletonbased_warp_sub2tar.h
HEADERS      += plugin_principalskeleton_detection.h
#HEADERS      += main_principalskeleton_detection_domain_dofunc.h

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.cpp
SOURCES      += q_morphology.cpp
SOURCES      += q_principalskeleton_detection.cpp
SOURCES      += q_neurontree_segmentation.cpp
SOURCES      += q_skeletonbased_warp_sub2tar.cpp
SOURCES      += plugin_principalskeleton_detection.cpp
#SOURCES      += main_principalskeleton_detection_domain_dofunc.cpp


TARGET        = $$qtLibraryTarget(principalskeleton_detection)

DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/shape_analysis/principal_skeleton_detection #for visiocyte batch build
