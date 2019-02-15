# Create plugin
set(PLUGIN_HEADERS lobeseg_gui.h lobeseg_plugin.h) # Q_OBJECT macro containing files only
set(PLUGIN_DIRECTORY_NAME image_segmentation/lobeseger)
set(PLUGIN_SOURCES
    lobeseg_plugin.cpp
    lobeseg_func.cpp
    getopt.c
    lobeseg_main/lobeseg.cpp
    ${VISIOCYTEMAINDIR}/worm_straighten_c/bdb_minus.cpp
    ${VISIOCYTEMAINDIR}/worm_straighten_c/mst_prim_c.cpp
    ${VISIOCYTEMAINDIR}/worm_straighten_c/bfs_1root.cpp
)
include_directories("${TIFF_INCLUDE_DIR}")
configure_visiocyte_plugin(lobeseg)

# Create standalone executable
#if(NOT WIN32)
#add_executable(lobe_seger
#    ${VISIOCYTE_MAIN}/worm_straighten_c/bdb_minus.cpp
#    lobeseg_main/lobeseg.cpp
#    lobeseg_main/main_backgnd_lobeseg_bl.cpp
#    ${VISIOCYTE_MAIN}/basic_c_fun/mg_utilities.cpp
#    ${VISIOCYTE_MAIN}/basic_c_fun/mg_image_lib.cpp
#    ${VISIOCYTE_MAIN}/basic_c_fun/stackutil.cpp
#    ${VISIOCYTE_MAIN}/worm_straighten_c/mst_prim_c.cpp
#    ${VISIOCYTE_MAIN}/worm_straighten_c/bfs_1root.cpp
#)
#target_link_libraries(lobe_seger ${TIFF_LIBRARY})
#set_target_properties(lobe_seger PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#install(TARGETS lobe_seger
#    DESTINATION bin
#)
#endif(NOT WIN32)
