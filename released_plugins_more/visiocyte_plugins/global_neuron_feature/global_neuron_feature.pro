TEMPLATE      = lib
CONFIG       += qt plugin warn_off

VISIOCYTEMAINPATH  += ../../../../visiocyte_external/visiocyte_main/
INCLUDEPATH  += $$VISIOCYTEMAINPATH/basic_c_fun

HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.h
HEADERS      += global_neuron_feature.h

HEADERS      += Nfmain.h
HEADERS      += $$VISIOCYTEMAINPATH/../visiocyte_main/neuron_editing/global_feature_compute.h

HEADERS      += ../sort_neuron_swc/openSWCDialog.h


SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += global_neuron_feature.cpp
SOURCES      += Nfmain.cpp
SOURCES      += $$VISIOCYTEMAINPATH/../visiocyte_main/neuron_editing/global_feature_compute.cpp
SOURCES      += ../sort_neuron_swc/openSWCDialog.cpp


TARGET        = $$qtLibraryTarget(global_neuron_feature)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_utilities/global_neuron_feature
