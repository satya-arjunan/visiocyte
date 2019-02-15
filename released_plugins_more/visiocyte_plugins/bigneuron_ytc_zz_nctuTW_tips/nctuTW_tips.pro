
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH     += $$VISIOCYTEPATH/neuron_editing
INCLUDEPATH     += $$VISIOCYTEPATH/worm_straighten_c
INCLUDEPATH     += $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH     += $$VISIOCYTEPATH/cellseg
INCLUDEPATH     += ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app2

macx{
    LIBS += -L$$VISIOCYTEPATH/common_lib/lib_mac64 -lvisiocytetiff
    LIBS += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat
}

win32 {
    contains(QMAKE_HOST.arch, x86_64) {
    LIBS     += -L$$VISIOCYTEPATH/common_lib/winlib64 -llibtiff
    LIBS     += -L$$VISIOCYTEPATH/common_lib/winlib64 -llibnewmat
    } else {
    LIBS     += -L$$VISIOCYTEPATH/common_lib/winlib -llibtiff
    LIBS     += -L$$VISIOCYTEPATH/common_lib/winlib -llibnewmat
    }
}

unix:!macx {
    LIBS += -L$$VISIOCYTEPATH/common_lib/lib -ltiff
    LIBS += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
}


HEADERS	+= nctuTW_tips_plugin.h\
    Dijkstra.h \
    DistanceTransTwo.h \
    Geo.h \
    Graph_AdjacencyList3D.h \
    Image3D.h \
    IPT.h \
    LineSeg3D.h \
    main.h \
    Point3D.h \
    UnionFind.h \
    findingtips.h

HEADERS += $$VISIOCYTEPATH/basic_c_fun/mg_utilities.h \
    $$VISIOCYTEPATH/basic_c_fun/basic_memory.h \
    $$VISIOCYTEPATH/basic_c_fun/stackutil.h\
    $$VISIOCYTEPATH/basic_c_fun/mg_image_lib.h\
    $$VISIOCYTEPATH/basic_c_fun/img_definition.h \
    $$VISIOCYTEPATH/basic_c_fun/volimg_proc_declare.h \
    $$VISIOCYTEPATH/basic_c_fun/volimg_proc.h \
    $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.h \
    $$VISIOCYTEPATH/basic_c_fun/color_xyz.h \
    $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.h \
    $$VISIOCYTEPATH/basic_c_fun/basic_landmark.h \
    $$VISIOCYTEPATH/graph/graph.h \
    $$VISIOCYTEPATH/graph/graph_basic.h \
    $$VISIOCYTEPATH/graph/dijk.h \
    $$VISIOCYTEPATH/worm_straighten_c/bdb_minus.h \
    $$VISIOCYTEPATH/worm_straighten_c/mst_prim_c.h \
    $$VISIOCYTEPATH/worm_straighten_c/bfs.h \
    $$VISIOCYTEPATH/worm_straighten_c/spline_cubic.h \
    $$VISIOCYTEPATH/neuron_editing/apo_xforms.h \
    $$VISIOCYTEPATH/neuron_editing/neuron_xforms.h \
    $$VISIOCYTEPATH/neuron_editing/neuron_sim_scores.h \
    $$VISIOCYTEPATH/neuron_editing/v_neuronswc.h \
    $$VISIOCYTEPATH/cellseg/template_matching_seg.h \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/FL_bwdist.h \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/gd.h \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/tip_detection.h \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/autoseed.h \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/visiocyteneuron_gd_tracing.h
SOURCES	+= nctuTW_tips_plugin.cpp\
    Dijkstra.cpp \
    DistanceTransTwo.cpp \
    Geo.cpp \
    Graph_AdjacencyList3D.cpp \
    Image3D.cpp \
    IPT.cpp \
    LineSeg3D.cpp \
    Point3D.cpp \
    main.cpp


SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp

SOURCES += $$VISIOCYTEPATH/basic_c_fun/mg_utilities.cpp \
    $$VISIOCYTEPATH/basic_c_fun/basic_memory.cpp \
    $$VISIOCYTEPATH/basic_c_fun/stackutil.cpp\
    $$VISIOCYTEPATH/basic_c_fun/mg_image_lib.cpp\
    $$VISIOCYTEPATH/graph/dijk.cpp \
    $$VISIOCYTEPATH/worm_straighten_c/bdb_minus.cpp \
    $$VISIOCYTEPATH/worm_straighten_c/mst_prim_c.cpp \
    $$VISIOCYTEPATH/worm_straighten_c/bfs_1root.cpp \
    $$VISIOCYTEPATH/worm_straighten_c/spline_cubic.cpp \
    $$VISIOCYTEPATH/neuron_editing/apo_xforms.cpp \
    $$VISIOCYTEPATH/neuron_editing/neuron_xforms.cpp \
    $$VISIOCYTEPATH/neuron_editing/neuron_sim_scores.cpp \
    $$VISIOCYTEPATH/neuron_editing/v_neuronswc.cpp \
    $$VISIOCYTEPATH/cellseg/template_matching_seg.cpp \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/gd.cpp \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/tip_detection.cpp \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/autoseed.cpp \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/calculate_cover_scores.cpp \
    ../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app1/visiocyteneuron_gd_tracing.cpp \

TARGET	= $$qtLibraryTarget(tips_GD)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/tips_GD/

