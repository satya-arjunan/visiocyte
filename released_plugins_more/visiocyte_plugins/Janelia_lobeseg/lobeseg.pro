
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64

VISIOCYTEMAINFOLER = ../../../visiocyte_main
INCLUDEPATH     += $$VISIOCYTEMAINFOLER/basic_c_fun $$VISIOCYTEMAINFOLER/common_lib/include
LIBS += -L. -lvisiocytetiff -L$$VISIOCYTEMAINFOLER/common_lib/lib

HEADERS = lobeseg_plugin.h
HEADERS += lobeseg_func.h
HEADERS += lobeseg_gui.h
HEADERS += getopt.h

HEADERS += lobeseg_main/lobeseg.h
HEADERS += $$VISIOCYTEMAINFOLER/basic_c_fun/basic_memory.h
HEADERS += $$VISIOCYTEMAINFOLER/basic_c_fun/volimg_proc.h


HEADERS += $$VISIOCYTEMAINFOLER/worm_straighten_c/bdb_minus.h
HEADERS += $$VISIOCYTEMAINFOLER/worm_straighten_c/mst_prim_c.h
HEADERS += $$VISIOCYTEMAINFOLER/worm_straighten_c/graphsupport.h
HEADERS += $$VISIOCYTEMAINFOLER/worm_straighten_c/graph.h
HEADERS += $$VISIOCYTEMAINFOLER/worm_straighten_c/bfs.h

SOURCES	= lobeseg_plugin.cpp
SOURCES	+= lobeseg_func.cpp
SOURCES += getopt.c
SOURCES	+= $$VISIOCYTEMAINFOLER/basic_c_fun/visiocyte_message.cpp

SOURCES += lobeseg_main/lobeseg.cpp
SOURCES	+= $$VISIOCYTEMAINFOLER/basic_c_fun/basic_memory.cpp
SOURCES += $$VISIOCYTEMAINFOLER/worm_straighten_c/bdb_minus.cpp
SOURCES += $$VISIOCYTEMAINFOLER/worm_straighten_c/mst_prim_c.cpp
SOURCES += $$VISIOCYTEMAINFOLER/worm_straighten_c/bfs_1root.cpp

TARGET	= $$qtLibraryTarget(Janelia_lobeseg)
DESTDIR	= $$VISIOCYTEMAINFOLER/../bin/plugins/FlyWorkstation_utilities/Janelia_lobeseger
 
