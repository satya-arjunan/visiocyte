
TEMPLATE      = app
CONFIG       += qt warn_off
CONFIG       -= app_bundle

VISIOCYTEMAINPATH = ../../../visiocyte_external/visiocyte_main
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINPATH/jba/newmat11

LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ -lvisiocytenewmat

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.h
HEADERS      += q_pointcloud_match_basic.h
HEADERS      += q_pointcloud_match_initialmatch.h
HEADERS      += q_pointcloud_match_refinematch_manifold.h
HEADERS      += q_pointcloud_match_refinematch_affine.h
HEADERS      += q_pointcloud_match.h

SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
#SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/io_ano_file.cpp
SOURCES      += q_pointcloud_match_basic.cpp
SOURCES      += q_pointcloud_match_initialmatch.cpp
SOURCES      += q_pointcloud_match_refinematch_manifold.cpp
SOURCES      += q_pointcloud_match_refinematch_affine.cpp
SOURCES      += q_pointcloud_match.cpp
SOURCES      += main_pointcloud_match.cpp



