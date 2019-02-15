# affine+TPS transfer the image based on the given point sets
# by Lei Qu
# 2010-10-27

TEMPLATE = app
CONFIG += qt 
#QT -= gui # Only the core module is used

INCLUDEPATH += ../../visiocyte_main/jba/newmat11

LIBS += -ltiff
LIBS += -L../../visiocyte_main/jba/c++ -lvisiocytenewmat

HEADERS += ../../visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS += ../../visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS += ../../visiocyte_main/basic_c_fun/stackutil.h
HEADERS += ../../visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS += ../../visiocyte_main/basic_c_fun/mg_utilities.h
HEADERS += q_warp_affine_tps.h

SOURCES += ../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += ../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES += ../../visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES += ../../visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES += ../../visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES += q_warp_affine_tps.cpp
SOURCES += main_imgwarp_affine_tps.cpp


