
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= \
    mean_shift_dialog.h \
    mean_shift_center_plugin.h \
    mean_shift_extr_template.h \
    ray_shoot_dialog.h \
    ../gsdt/main/heap.h \
    mean_shift_fun.h \
    gradient_transform_dialog.h \
    fastmarching_dt.h

SOURCES	+= \
    mean_shift_dialog.cpp \
    mean_shift_center_plugin.cpp \
    ray_shoot_dialog.cpp \
    mean_shift_fun.cpp \
    gradient_transform_dialog.cpp

SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp


TARGET	= $$qtLibraryTarget(mean_shift_center_finder)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/image_analysis/mean_shift_center_finder/
