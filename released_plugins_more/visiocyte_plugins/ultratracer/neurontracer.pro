
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH  += $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEPATH/common_lib/include
INCLUDEPATH  += $$VISIOCYTEPATH/common_lib/include/hdf5
INCLUDEPATH     += ../neurontracing_vn2
INCLUDEPATH     += ../neurontracing_vn2/app2
INCLUDEPATH     += ../neurontracing_vn2/app1
INCLUDEPATH     += ../neuron_image_profiling
INCLUDEPATH	+= ../eswc_converter
INCLUDEPATH	+= $$VISIOCYTEPATH/jba/newmat11

INCLUDEPATH     += $$VISIOCYTEPATH/neuron_editing
INCLUDEPATH     += $$VISIOCYTEPATH/worm_straighten_c

macx{
    LIBS += -L$$VISIOCYTEPATH/common_lib/lib_mac64 -lvisiocytetiff
    LIBS += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat
}

unix:!macx {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
    LIBS += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat

}



HEADERS	+= neurontracer_plugin.h
HEADERS	+= tracing_func.h

HEADERS	+= $$VISIOCYTEPATH/../../visiocyte_tools/hackathon/zhi/APP2_large_scale/readrawfile_func.h

HEADERS += ../neurontracing_vn2/vn_imgpreprocess.h
HEADERS += ../neurontracing_vn2/vn.h
HEADERS += ../neurontracing_vn2/vn_app2.h
HEADERS += ../neurontracing_vn2/vn_app1.h

HEADERS += ../neurontracing_vn2/app2/fastmarching_tree.h
HEADERS += ../neurontracing_vn2/app2/hierarchy_prune.h
HEADERS += ../neurontracing_vn2/app2/fastmarching_dt.h
HEADERS += ../neuron_image_profiling/openSWCDialog.h

SOURCES	+= neurontracer_plugin.cpp
SOURCES	+= tracing_func.cpp

SOURCES	+= $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES += $$VISIOCYTEPATH/basic_c_fun/stackutil.cpp

SOURCES	+= $$VISIOCYTEPATH/../../visiocyte_tools/hackathon/zhi/APP2_large_scale/readrawfile_func.cpp
SOURCES += $$VISIOCYTEPATH/../../visiocyte_tools/hackathon/zhi/APP2_large_scale/my_surf_objs.cpp

SOURCES += ../neurontracing_vn2/vn_imgpreprocess.cpp
SOURCES += ../neurontracing_vn2/app2_connector.cpp
SOURCES += ../neurontracing_vn2/app1_connector.cpp
SOURCES += ../neurontracing_vn2/app1/visiocyteneuron_gd_tracing.cpp
SOURCES += ../neurontracing_vn2/app1/gd.cpp
SOURCES += ../neurontracing_vn2/app1/calculate_cover_scores.cpp

SOURCES += ../neurontracing_vn2/swc_convert.cpp
SOURCES += ../neuron_image_profiling/profile_swc.cpp
SOURCES += ../neuron_image_profiling/compute_tubularity.cpp
SOURCES += ../neuron_image_profiling/openSWCDialog.cpp


SOURCES += $$VISIOCYTEPATH/basic_c_fun/basic_4dimage_create.cpp
SOURCES += $$$$VISIOCYTEPATH/neuron_editing/v_neuronswc.cpp \
    $$VISIOCYTEPATH/graph/dijk.cpp

#meanshift
HEADERS += $$VISIOCYTEPATH/../../visiocyte_tools/released_plugins/visiocyte_plugins/mean_shift_center/mean_shift_fun.h
SOURCES += $$VISIOCYTEPATH/../../visiocyte_tools/released_plugins/visiocyte_plugins/mean_shift_center/mean_shift_fun.cpp

TARGET	= $$qtLibraryTarget(ultratracer)
DESTDIR	= $$VISIOCYTEPATH/../bin/plugins/neuron_tracing/ultratracer/
