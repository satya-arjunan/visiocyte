
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= color_render_ESWC_features_plugin.h \
    color_render_eswc_dialog.h
SOURCES	+= color_render_ESWC_features_plugin.cpp \
    color_render_eswc_dialog.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(color_render_ESWC_features)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/color_render_ESWC_features/
