TEMPLATE	= lib
CONFIG	+= qt plugin warn_off

VISIOCYTEPATH = ../../../../visiocyte_external
VISIOCYTEMAINPATH = ../../../../visiocyte_external/visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINPATH/jba/newmat11

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


HEADERS	+= affine_transform_swc_plugin.h
HEADERS	+= apply_transform_func.h
HEADERS	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.h
HEADERS += io_affine_transform.h

SOURCES	+= affine_transform_swc_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES	+= apply_transform_func.cpp
SOURCES	+= io_affine_transform.cpp


TARGET	= $$qtLibraryTarget(affine_transform_swc)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/neuron_utilities/affine_transform_swc/
