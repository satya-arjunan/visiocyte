
set(PLUGIN_DIRECTORY_NAME VISIOCYTE_PluginInterface_Demos/call_each_other )

set(PLUGIN_SOURCES ../visiocyteplugin_matrix_example/ex_matrix.cpp )
set(PLUGIN_HEADERS ../visiocyteplugin_matrix_example/ex_matrix.h )

configure_visiocyte_plugin(ex_matrix)

set(PLUGIN_SOURCES ex_call.cpp )
set(PLUGIN_HEADERS ex_call.h )

configure_visiocyte_plugin(ex_call)

