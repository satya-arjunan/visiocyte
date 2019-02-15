#GFP Puncta Statistics
#by Yang Yu
#20090726
#updated 20100715

TEMPLATE = app
CONFIG += qt warn_off x86_64

HEADERS += ../../../visiocyte_main/basic_c_fun/visiocyte_message.h \
../../../visiocyte_main/basic_c_fun/basic_surf_objs.h \
../../../visiocyte_main/basic_c_fun/stackutil.h \
../../../visiocyte_main/basic_c_fun/mg_image_lib.h \
../../../visiocyte_main/basic_c_fun/mg_utilities.h \
../../../visiocyte_main/basic_c_fun/volimg_proc.h \
../../../visiocyte_main/basic_c_fun/img_definition.h \
../../../visiocyte_main/basic_c_fun/basic_landmark.h \
y_ytree.h

LIBS += -lm
LIBS += -L../../../visiocyte_main/common_lib/lib_mac64 -ltiff64

SOURCES += ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp \
../../../visiocyte_main/basic_c_fun/basic_surf_objs.cpp \
../../../visiocyte_main/basic_c_fun/stackutil.cpp \
../../../visiocyte_main/basic_c_fun/mg_utilities.cpp \
../../../visiocyte_main/basic_c_fun/mg_image_lib.cpp \
y_tm_puncta_statistics.cpp



