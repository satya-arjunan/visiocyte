#general principal skeleton detection
# by Lei Qu
# 2009-12-2

TEMPLATE = app
CONFIG += qt 
#QT -= gui # Only the core module is used

INCLUDEPATH += ../../../visiocyte_main/basic_c_fun/
INCLUDEPATH += ../../../visiocyte_main/jba/newmat11

LIBS += -ltiff
LIBS += -L../../../visiocyte_main/jba/c++ -lvisiocytenewmat

HEADERS += ../../../visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS += ../../../visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS += ../../../visiocyte_main/basic_c_fun/stackutil.h
HEADERS += ../../../visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS += ../../../visiocyte_main/basic_c_fun/mg_utilities.h
HEADERS += ../../../visiocyte_main/worm_straighten_c/spline_cubic.h
#HEADERS += ../../../visiocyte_main/q_skeletonbased_warp_sub2tar/q_neurontree_segmentation.h
HEADERS += q_morphology.h
HEADERS += q_principalskeleton_detection.h

SOURCES += ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += ../../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES += ../../../visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES += ../../../visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES += ../../../visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES += ../../../visiocyte_main/worm_straighten_c/spline_cubic.cpp
#SOURCES += ../../../visiocyte_main/q_skeletonbased_warp_sub2tar/q_neurontree_segmentation.cpp
SOURCES += q_morphology.cpp
SOURCES += q_principalskeleton_detection.cpp

SOURCES += main_principalskeleton_detection_domain.cpp


