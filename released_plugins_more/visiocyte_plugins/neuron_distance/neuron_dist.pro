
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/neuron_editing


HEADERS	+= neuron_dist_plugin.h
HEADERS	+= neuron_dist_func.h
HEADERS	+= neuron_dist_gui.h
HEADERS += $$VISIOCYTEPATH/../../visiocyte_tools/released_plugins/visiocyte_plugins/swc_to_maskimage/filter_dialog.h


SOURCES	= neuron_dist_plugin.cpp
SOURCES	+= neuron_dist_func.cpp
SOURCES	+= neuron_dist_gui.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/neuron_editing/neuron_sim_scores.cpp
SOURCES	+= $$VISIOCYTEPATH/neuron_editing/v_neuronswc.cpp
SOURCES += $$VISIOCYTEPATH/../../visiocyte_tools/released_plugins/visiocyte_plugins/swc_to_maskimage/filter_dialog.cpp


TARGET	= $$qtLibraryTarget(neuron_dist)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_utilities/neuron_distance
