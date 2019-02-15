
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH     += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH     += app2
INCLUDEPATH     += app1
INCLUDEPATH     += $$VISIOCYTEMAINPATH/neuron_editing
INCLUDEPATH     += $$VISIOCYTEMAINPATH/worm_straighten_c

unix {
LIBS += -L$$VISIOCYTEMAINPATH/jba/c++
LIBS += -lvisiocytenewmat
}
win32 {
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/winlib64
LIBS += -llibnewmat
}

#LIBS += -llibm

HEADERS	+= visiocyteneuron2_plugin.h
HEADERS += vn_imgpreprocess.h
HEADERS += vn.h
HEADERS += vn_app2.h
HEADERS += vn_app1.h
HEADERS += app1/visiocyteneuron_gd_tracing.h
HEADERS += app1/gd.h
HEADERS += app2/fastmarching_tree.h
HEADERS += app2/hierarchy_prune.h
HEADERS += app2/fastmarching_dt.h


SOURCES += vn_imgpreprocess.cpp
SOURCES += app2_connector.cpp
SOURCES += app1_connector.cpp
SOURCES += visiocyteneuron2_plugin.cpp
SOURCES += swc_convert.cpp
SOURCES += app2/my_surf_objs.cpp
SOURCES += app1/visiocyteneuron_gd_tracing.cpp
SOURCES += app1/gd.cpp
SOURCES += app1/calculate_cover_scores.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_4dimage_create.cpp
SOURCES += $$VISIOCYTEMAINPATH/worm_straighten_c/bdb_minus.cpp \
    $$VISIOCYTEMAINPATH/worm_straighten_c/mst_prim_c.cpp \
    $$VISIOCYTEMAINPATH/worm_straighten_c/bfs_1root.cpp \
    $$VISIOCYTEMAINPATH/graph/dijk.cpp \
    $$VISIOCYTEMAINPATH/neuron_editing/neuron_sim_scores.cpp \
    $$VISIOCYTEMAINPATH/neuron_editing/v_neuronswc.cpp \
    $$VISIOCYTEMAINPATH/neuron_editing/global_feature_compute.cpp
    #$$VISIOCYTEMAINPATH/neuron_editing/apo_xforms.cpp \
    #$$VISIOCYTEMAINPATH/neuron_editing/neuron_xforms.cpp \

TARGET	= $$qtLibraryTarget(vn2)
DESTDIR = $$VISIOCYTEMAINPATH/../bin/plugins/neuron_tracing/Visiocyte_Neuron2

