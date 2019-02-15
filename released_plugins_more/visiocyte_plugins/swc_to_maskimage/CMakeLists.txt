
set(PLUGIN_HEADERS swc_to_maskimage.h filter_dialog.h )
set(PLUGIN_SOURCES 
    swc_to_maskimage.cpp
    filter_dialog.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/basic_surf_objs.cpp
    ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/visiocyte_message.cpp
)
set(PLUGIN_DIRECTORY_NAME neuron_utilities/swc_to_maskimage)

configure_visiocyte_plugin(swc_to_maskimage)

