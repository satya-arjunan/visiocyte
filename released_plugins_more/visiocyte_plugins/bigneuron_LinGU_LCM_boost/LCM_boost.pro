TEMPLATE    = lib
CONFIG  += debug qt plugin warn_off
#CONFIG += x86_64

VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun 
INCLUDEPATH += $$VISIOCYTEPATH/visiocyte_main/common_lib/include

INCLUDEPATH += ./include/opencv
INCLUDEPATH += ./include


LIBS += -L. -lvisiocytetiff -L$$VISIOCYTEPATH/visiocyte_main/common_lib/lib

LIBS += -L"$$_PRO_FILE_PWD_/lib_centos"

LIBS+= -lopencv_core -lopencv_imgproc -lopencv_ml  \

HEADERS =  LCM_boost_plugin.h
HEADERS += LCM_boost_func.h
HEADERS += vn_imgpreprocess.h
HEADERS += fastmarching_tree.h
HEADERS += hierarchy_prune.h
HEADERS += fastmarching_dt.h
HEADERS += my_surf_objs.h
HEADERS += swc_to_maskimage.h

SOURCES = LCM_boost_plugin.cpp
SOURCES += LCM_boost_func.cpp
SOURCES += my_surf_objs.cpp
SOURCES += vn_imgpreprocess.cpp
SOURCES += swc_to_maskimage.cpp

HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_memory.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/stackutil.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_utilities.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_image_lib.h
HEADERS += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.h

# put all visiocyte related cpp file in the end !!!

SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_memory.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/stackutil.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_utilities.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/mg_image_lib.cpp
SOURCES += $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/basic_surf_objs.cpp

TARGET  = $$qtLibraryTarget(LCM_boost)
DESTDIR = $$VISIOCYTEPATH/bin/plugins/neuron_tracing/LCM_boost/

QMAKE_POST_LINK = cp -r LCM_model LCM_trn_data $$VISIOCYTEPATH/bin/ 

