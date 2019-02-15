TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += x86_64

VISIOCYTEPATH = ../../../../visiocyte_external
VISIOCYTEMAINPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS       = snapshots_3dviewer.h
SOURCES       = snapshots_3dviewer.cpp
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.h
SOURCES      += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
HEADERS      += $$VISIOCYTEMAINPATH/neuron_editing/neuron_xforms.h
SOURCES      += $$VISIOCYTEMAINPATH/neuron_editing/neuron_xforms.cpp

TARGET        = $$qtLibraryTarget(snapshots_3dviewer)

DESTDIR       = $$VISIOCYTEPATH/bin/plugins/movies_and_snapshots/snapshots_3dviewer

