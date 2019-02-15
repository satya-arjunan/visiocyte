
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
#VISIOCYTEPATH = C:\Users\rummig\Desktop\Visiocyte_allfiles\Visiocyte_Downloads\visiocyte_external
VISIOCYTEPATH = ../../../../visiocyte_external

INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/visiocyte_main/common_lib/include

HEADERS	= node_connect_plugin.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_interface.h \
    openSWCDialog.h \
    my_surf_objs.h \
    node_connect_func.h


SOURCES	= node_connect_plugin.cpp

SOURCES += openSWCDialog.cpp \
    node_connect_func.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp


TARGET	= $$qtLibraryTarget(node_connect)
DESTDIR = $$VISIOCYTEPATH/bin/plugins/neuron_utilities/node_connect
#DESTDIR = C:\Users\rummig\Desktop\Visiocyte_allfiles\visiocyte_win7_32bit_v2.707\plugins\node_connect
