
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external

INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/common_lib/include


HEADERS	+= neutu_autotrace_plugin.h libNeuTu.h
SOURCES	+= neutu_autotrace_plugin.cpp libNeuTu.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp


TARGET	= $$qtLibraryTarget(neutu_autotrace)
PLUGIN_DIR = plugins/neuron_tracing/neutu_autotrace
PARAMDIR = bin
BINDIR	= $$VISIOCYTEPATH/$$PARAMDIR
DESTDIR	= $$BINDIR/$$PLUGIN_DIR

DEFINES += NEUTU_PARAM_DIR=\"\\\"$$PARAMDIR\\\"\"
unix {
DEPLOY_COMMAND = "$$PWD/deploy $$BINDIR"
QMAKE_POST_LINK += $$DEPLOY_COMMAND
}
unix:macx {
DEFINES += _MAC_APPLICATION_
}
