
set(PLUGIN_HEADERS plugin_principalskeleton_detection.h )
set(PLUGIN_SOURCES 
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/visiocyte_message.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/stackutil.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/mg_image_lib.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/mg_utilities.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/basic_surf_objs.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/../worm_straighten_c/spline_cubic.cpp
    q_morphology.cpp
    q_principalskeleton_detection.cpp
    q_neurontree_segmentation.cpp
    q_skeletonbased_warp_sub2tar.cpp
    plugin_principalskeleton_detection.cpp
)
set(PLUGIN_DIRECTORY_NAME principal_skeleton_detection)

include_directories(${TIFF_INCLUDE_DIR})
include_directories(${VISIOCYTEMAINDIR}/jba/newmat11)
configure_visiocyte_plugin(principalskeleton_detection)
target_link_libraries(principalskeleton_detection ${TIFF_LIBRARY})
find_library(newmat_LIBRARY newmat11 PATH ${VISIOCYTE_PLUGIN_LIBRARIES_PATH} REQUIRED)
target_link_libraries(principalskeleton_detection newmat11)


# TEMPLATE      = lib
# CONFIG       += plugin 
# 
# VISIOCYTEMAINDIR = ../../../visiocyte_main
# 
# INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
# INCLUDEPATH  += $$VISIOCYTEMAINDIR/jba/newmat11
# INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include
# 
# LIBS 	     += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
# LIBS         += -L$$VISIOCYTEMAINDIR/jba/c++ -lvisiocytenewmat
# 
# HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
# HEADERS	     += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
# HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
# HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
# HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
# HEADERS      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.h
# HEADERS      += q_morphology.h
# HEADERS      += q_principalskeleton_detection.h
# HEADERS      += q_neurontree_segmentation.h
# HEADERS      += q_skeletonbased_warp_sub2tar.h
# HEADERS      += plugin_principalskeleton_detection.h
# 
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.cpp
# SOURCES      += q_morphology.cpp
# SOURCES      += q_principalskeleton_detection.cpp
# SOURCES      += q_neurontree_segmentation.cpp
# SOURCES      += q_skeletonbased_warp_sub2tar.cpp
# SOURCES      += plugin_principalskeleton_detection.cpp
# 
# TARGET        = $$qtLibraryTarget(principalskeleton_detection)
# 
# DESTDIR       = ../../visiocyte/plugins/principal_skeleton_detection #win32 qmake couldn't handle space in path
# #DESTDIR       = ../../../visiocyte_main/visiocyte/plugins/principal_skeleton_detection #win32 qmake couldn't handle space in path
