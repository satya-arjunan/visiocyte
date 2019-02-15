
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEMAINPATH/common_lib/include/boost/graph
INCLUDEPATH     += $$VISIOCYTEMAINPATH/common_lib/include/

INCLUDEPATH     += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/include
HEADERS += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/include/ANN/ANN.h
HEADERS += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/bd_tree.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_fix_rad_search.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_pr_search.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_search.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_split.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_tree.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_util.h \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/pr_queue.h
SOURCES += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/ANN.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/bd_fix_rad_search.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/bd_pr_search.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/bd_search.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/bd_tree.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/brute.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_dump.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_fix_rad_search.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_pr_search.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_search.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_split.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_tree.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/kd_util.cpp \
    $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/src/perf.cpp

unix:!macx{
    PRE_TARGETDEPS = $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/libANN.a
    LIBS            += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/libANN.a
    ANN.target = $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/libANN.a
    ANN.commands = cd $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann && make linux-g++
    ANN.depends = $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/Makefile
    QMAKE_EXTRA_TARGETS += ANN
}
macx {
    PRE_TARGETDEPS = $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/libANN.a
    LIBS            += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/libANN.a
    ANN.target = $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/libANN.a
    ANN.commands = cd $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann && make macosx-g++
    ANN.depends = $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/Makefile
    QMAKE_EXTRA_TARGETS += ANN

}
win32 {
    win32-msvc:LIBS += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/lib/ANN.lib
    INCLUDEPATH += $$VISIOCYTEMAINPATH/../../visiocyte_tools/hackathon/xiaoxiaol/consensus_skeleton_2/ann/MS_Win32/dll/
}


HEADERS	+= marker_minspanningtree_plugin.h

SOURCES	+= marker_minspanningtree_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEMAINPATH/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(marker2mst)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/marker_utilities/markers_to_minimum_spanning_tree/
