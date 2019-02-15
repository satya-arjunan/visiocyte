
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include

HEADERS	+= CWlab_method1_version1_plugin.h
HEADERS += vn_imgpreprocess.h
HEADERS += fastmarching_tree.h
HEADERS += hierarchy_prune.h
HEADERS += fastmarching_dt.h
HEADERS += my_surf_objs.h

SOURCES	+= CWlab_method1_version1_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES += vn_imgpreprocess.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_4dimage_create.cpp
SOURCES += my_surf_objs.cpp


TARGET	= $$qtLibraryTarget(CWlab_method1_version1)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_tracing/CWlab_method1_version1/
