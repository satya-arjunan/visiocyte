
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = c:/work/visiocyte_64bit/visiocyte_main
INCLUDEPATH     += $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEMAINPATH/common_lib/include
INCLUDEPATH     += app2
INCLUDEPATH     += app1
INCLUDEPATH     += $$VISIOCYTEMAINPATH/neuron_editing
INCLUDEPATH     += $$VISIOCYTEMAINPATH/worm_straighten_c

macx {
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib_mac64 -lvisiocytetiff
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/src_packages/mylib_tiff -lmylib
LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ 
LIBS += -lm -lvisiocytenewmat
}

unix:!macx {
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/lib -lvisiocytetiff
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/src_packages/mylib_tiff -lmylib
LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ 
LIBS += -lm -lvisiocytenewmat
}

win32 {
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibtiff
LIBS += -L$$VISIOCYTEMAINPATH/common_lib/winlib -llibnewmat
#LIBS += -L$$VISIOCYTEMAINPATH/common_lib/src_packages/mylib_tiff -llibmylib
LIBS += -L$$VISIOCYTEMAINPATH/jba/c++ 
#LIBS += -lm 

}



HEADERS	+= visiocyteneuron2_plugin.h
HEADERS += $$VISIOCYTEMAINPATH/basic_c_fun/basic_memory.cpp
HEADERS += vn_imgpreprocess.h
HEADERS += vn.h
HEADERS += vn_app2.h
HEADERS += vn_app1.h
HEADERS += app1/visiocyteneuron_gd_tracing.h
HEADERS += app1/gd.h
HEADERS += app2/fastmarching_tree.h
HEADERS += app2/hierarchy_prune.h
HEADERS += app2/fastmarching_dt.h


!win32 {
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/imageio_mylib.h
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/imageio_mylib.cpp 
}

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
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_4dimage_create.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_4dimage.cpp
SOURCES += $$VISIOCYTEMAINPATH/worm_straighten_c/bdb_minus.cpp \
    $$VISIOCYTEMAINPATH/worm_straighten_c/mst_prim_c.cpp \
    $$VISIOCYTEMAINPATH/worm_straighten_c/bfs_1root.cpp \
    $$VISIOCYTEMAINPATH/graph/dijk.cpp \
    \ #$$VISIOCYTEMAINPATH/neuron_editing/apo_xforms.cpp \
    \ #$$VISIOCYTEMAINPATH/neuron_editing/neuron_xforms.cpp \
    $$VISIOCYTEMAINPATH/neuron_editing/neuron_sim_scores.cpp \
    $$VISIOCYTEMAINPATH/neuron_editing/v_neuronswc.cpp 



TARGET	= $$qtLibraryTarget(vn2)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/neuron_tracing/Visiocyte_Neuron2
