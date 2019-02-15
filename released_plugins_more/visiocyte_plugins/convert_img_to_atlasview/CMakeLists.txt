
set(PLUGIN_HEADERS convert_img_to_atlasview.h )
set(PLUGIN_SOURCES 
    convert_img_to_atlasview.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/visiocyte_message.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/stackutil.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/mg_utilities.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/mg_image_lib.cpp
)
set(PLUGIN_DIRECTORY_NAME Convert_Image_to_AtlasViewMode)

include_directories(${TIFF_INCLUDE_DIR})
configure_visiocyte_plugin(convert_img_to_atlasview)
target_link_libraries(convert_img_to_atlasview ${TIFF_LIBRARY})

# TEMPLATE      = lib
# CONFIG       += qt plugin warn_off
# #CONFIG       += x86_64
# 
# VISIOCYTEMAINDIR = ../../../visiocyte_main
# 
# INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
# INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include
# 
# HEADERS       = convert_img_to_atlasview.h
# HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h
# HEADERS      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h
# HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h
# HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
# 
# SOURCES       = convert_img_to_atlasview.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
# SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp
# SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp
# SOURCES      +=	$$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp
# 
# LIBS         += -L$$VISIOCYTEMAINDIR/common_lib/lib -lvisiocytetiff
# 
# TARGET        = $$qtLibraryTarget(convert_img_to_atlasview)
# DESTDIR       = ../../visiocyte/plugins/Convert_Image_to_AtlasViewMode
# 
