TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= $$VISIOCYTEPATH/../visiocyte_tools/released_plugins/visiocyte_plugins/eswc_converter
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/jba/newmat11

macx{
    LIBS += -L$$VISIOCYTEPATH/visiocyte_main/jba/c++ -lvisiocytenewmat
#    CONFIG += x86_64
}

win32{
    LIBS += -L$$VISIOCYTEPATH/visiocyte_main/common_lib/winlib64 -llibnewmat
}

unix:!macx {
    LIBS += -L$$VISIOCYTEPATH/visiocyte_main/jba/c++ -lvisiocytenewmat
}


HEADERS	+= neuron_image_profiling_plugin.h \
    compute_tubularity.h
SOURCES	+= neuron_image_profiling_plugin.cpp \
    compute_tubularity.cpp
SOURCES	+= profile_swc.cpp
HEADERS	+= profile_swc.h
SOURCES += openSWCDialog.cpp
HEADERS += openSWCDialog.h
SOURCES += $$VISIOCYTEPATH/../visiocyte_tools/released_plugins/visiocyte_plugins/eswc_converter/eswc_core.cpp
HEADERS += $$VISIOCYTEPATH/../visiocyte_tools/released_plugins/visiocyte_plugins/eswc_converter/eswc_core.h


SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
HEADERS	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.h





TARGET	= $$qtLibraryTarget(neuron_image_profiling)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/neuron_image_profiling/
