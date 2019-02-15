# affine+TPS transfer the swc based on the given point sets
# by Lei Qu
# 2010-03-18

TEMPLATE = app
CONFIG += qt 
#QT -= gui # Only the core module is used

INCLUDEPATH += ../../visiocyte_main/jba/newmat11
LIBS += -L../../visiocyte_main/jba/c++ -lvisiocytenewmat

HEADERS += ../../visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS += ../../visiocyte_main/basic_c_fun/basic_surf_objs.h
HEADERS += q_warp_affine_tps.h

SOURCES += ../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += ../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES += q_warp_affine_tps.cpp
SOURCES += main_swcwarp_affine_tps.cpp


