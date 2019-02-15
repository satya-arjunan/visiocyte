
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= N3DFix_plugin.h \
    Dend_Section.h \
    myHeader.h \
    openSWCDialog.h
SOURCES	+= N3DFix_plugin.cpp \
    remove_artifacts.cpp \
    main.cpp \
    Struct_from_swc.cpp \
    myFuncs.cpp \
    Load_and_Write_DATA.cpp \
    openSWCDialog.cpp \
    ../../../../visiocyte_external/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(N3DFix)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/N3DFix/
