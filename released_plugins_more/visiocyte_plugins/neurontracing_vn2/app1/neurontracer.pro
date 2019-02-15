# #####################################################################
# 2010-05-24
# visiocyteneuron_tracing.pro
# by Hanchuan Peng
# separate from main visiocyte program as a command line program
#
# 2010-Dec-21: copy dijk_bgl.cpp as gd.cpp here and make further refinement
# 2012-April-2: change a few folders
# ######################################################################

TEMPLATE = app
TARGET += 
DEPENDPATH += . 
INCLUDEPATH += . 
INCLUDEPATH += ../app2
SOURCES += ../app2/my_surf_objs.cpp

#cross-OS-platform, cross-Qt-version, 
QT_DIR = $$dirname(QMAKE_QMAKE)/..  # cross-Qt-version
LOCAL_DIR = ../../../../../visiocyte_external/visiocyte_main 				# unix-liked platform: macx, unix, win32-msys-mingw
INCLUDEPATH += $$LOCAL_DIR/common_lib/include

MINGW_DIR = /mingw # platform: win32-msys-mingw
win32 { # platform: win32-command-mingw
	MINGW_DIR = c:/mingw
	LOCAL_DIR = ../common_lib/      # c:/msys/local
	CONFIG = $$unique(CONFIG)
	CONFIG -= debug # for Qt-win32 which only has release install(no debug)
	CONFIG += console
LIBS += -L$$MINGW_DIR/lib \
	-L$$LOCAL_DIR/lib_win32
}

message(CONFIG=$$unique(CONFIG))
macx {
#     CONFIG += x86_64
     CONFIG -= app_bundle
}

INCLUDEPATH += $$LOCAL_DIR/basic_c_fun 
INCLUDEPATH += $$LOCAL_DIR/common_lib/include 
INCLUDEPATH += $$LOCAL_DIR/neuron_editing
INCLUDEPATH += $$LOCAL_DIR/worm_straighten_c
INCLUDEPATH += $$LOCAL_DIR/cellseg

LIBS += -L$$LOCAL_DIR/lib


SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets
include($$SHARED_FOLDER/shared.pri)
INCLUDEPATH += $$SHARED_FOLDER
LIBS += -L$$SHARED_FOLDER \
	-L$$SHARED_FOLDER/release # for Qt-win32 which only has release install(no debug)

CONFIG += warn_off  # only work for complier

VISIOCYTEMAINDIR = ../../../visiocyte_external/visiocyte_main

# Input
HEADERS += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_memory.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/img_definition.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/volimg_proc_declare.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/volimg_proc.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/color_xyz.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.h \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_landmark.h \
    $$VISIOCYTEMAINDIR/graph/graph.h \
    $$VISIOCYTEMAINDIR/graph/graph_basic.h \
    $$VISIOCYTEMAINDIR/graph/dijk.h \
    $$VISIOCYTEMAINDIR/worm_straighten_c/bdb_minus.h \
    $$VISIOCYTEMAINDIR/worm_straighten_c/mst_prim_c.h \
    $$VISIOCYTEMAINDIR/worm_straighten_c/bfs.h \
    $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.h \
    $$VISIOCYTEMAINDIR/neuron_editing/apo_xforms.h \
    $$VISIOCYTEMAINDIR/neuron_editing/neuron_xforms.h \
    $$VISIOCYTEMAINDIR/neuron_editing/neuron_sim_scores.h \
    $$VISIOCYTEMAINDIR/neuron_editing/v_neuronswc.h \
    $$VISIOCYTEMAINDIR/cellseg/template_matching_seg.h \ 
    FL_bwdist.h \
    gd.h \
    tip_detection.h \
    autoseed.h \
    visiocyteneuron_gd_tracing.h
#    $$VISIOCYTEMAINDIR/visiocyte/dialog_curve_trace_para.h 
unix:HEADERS += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.h

SOURCES += $$VISIOCYTEMAINDIR/basic_c_fun/mg_utilities.cpp \
    $$VISIOCYTEMAINDIR/basic_c_fun/mg_image_lib.cpp \
    $$VISIOCYTEMAINDIR/basic_c_fun/stackutil.cpp \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_memory.cpp \
    $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp \
    $$VISIOCYTEMAINDIR/basic_c_fun/basic_4dimage.cpp \
    $$VISIOCYTEMAINDIR/graph/dijk.cpp \
    $$VISIOCYTEMAINDIR/worm_straighten_c/bdb_minus.cpp \
    $$VISIOCYTEMAINDIR/worm_straighten_c/mst_prim_c.cpp \
    $$VISIOCYTEMAINDIR/worm_straighten_c/bfs_1root.cpp \
    $$VISIOCYTEMAINDIR/worm_straighten_c/spline_cubic.cpp \
    $$VISIOCYTEMAINDIR/neuron_editing/apo_xforms.cpp \
    $$VISIOCYTEMAINDIR/neuron_editing/neuron_xforms.cpp \
    $$VISIOCYTEMAINDIR/neuron_editing/neuron_sim_scores.cpp \
    $$VISIOCYTEMAINDIR/neuron_editing/v_neuronswc.cpp \
    $$VISIOCYTEMAINDIR/cellseg/template_matching_seg.cpp \
    gd.cpp \
    tip_detection.cpp \
    autoseed.cpp \
    calculate_cover_scores.cpp \
	visiocyteneuron_gd_tracing.cpp \
	main_visiocyteneuron_tracing.cpp
unix:SOURCES += $$VISIOCYTEMAINDIR/basic_c_fun/imageio_mylib.cpp

FORMS += $$VISIOCYTEMAINDIR/visiocyte/dialog_curve_trace.ui 

RESOURCES +=
#QT += opengl

LIBS += -L$$VISIOCYTEMAINDIR/jba/c++ -L$$VISIOCYTEMAINDIR/common_lib/lib
LIBS += -lm -lvisiocytetiff -lvisiocytenewmat

unix:LIBS += -L$$VISIOCYTEMAINDIR/common_lib/src_packages/mylib_tiff -lmylib
    
#win32:LIBS += -lm -lvisiocytetiff 
#    -lglut32 # win32-mingw, on unix link libglut.a

INCLUDEPATH = $$unique(INCLUDEPATH)
LIBS = $$unique(LIBS)
# CONFIG = $$unique(CONFIG) # this only DOESN'T work on macx, very strange, by RZC 20080923
message(CONFIG=$$CONFIG)
