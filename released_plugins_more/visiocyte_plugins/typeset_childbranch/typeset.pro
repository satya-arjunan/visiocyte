
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH     += $$VISIOCYTEPATH/visiocyte_main/common_lib/include


HEADERS	= typeset_plugin.h
HEADERS	+= typeset_func.h
HEADERS += typeset.h
HEADERS += my_surf_objs.h
HEADERS += sort_func.h
HEADERS += openSWCDialog.h
HEADERS += sort_swc.h

HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_interface.h

SOURCES	= typeset_plugin.cpp
SOURCES	+= typeset_func.cpp
SOURCES += sort_func.cpp
SOURCES += openSWCDialog.cpp

SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(typeset)
#DESTDIR = C:\Users\rummig\Desktop\Visiocyte_allfiles\visiocyte_win7_32bit_v2.707\plugins\typeset_childbranch #to test
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/typeset_childbranch
