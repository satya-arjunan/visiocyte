#plugin_image_registration.h
# by Lei Qu and Hanchuan Peng
# 2011-2013

TEMPLATE      = lib
CONFIG       += plugin 
#CONFIG       += x86_64

DEFINES += _ALLOW_WORKMODE_MENU_       #added by PHC, 2013-01-27

VISIOCYTEPATH = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEPATH/jba/newmat11
INCLUDEPATH  += $$VISIOCYTEPATH/common_lib/include

#LIBS 	     += -L$$VISIOCYTEPATH/common_lib/lib -lvisiocytetiff
LIBS         += -L$$VISIOCYTEPATH/jba/c++ -lvisiocytenewmat

FORMS         = paradialog_rigidaffine.ui

HEADERS      += $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.h
HEADERS	     += $$VISIOCYTEPATH/basic_c_fun/stackutil.h
HEADERS      += $$VISIOCYTEPATH/basic_c_fun/mg_image_lib.h
HEADERS      += $$VISIOCYTEPATH/basic_c_fun/mg_utilities.h
HEADERS      += $$VISIOCYTEPATH/basic_c_fun/basic_memory.h
HEADERS      += $$VISIOCYTEPATH/basic_c_fun/basic_memory.cpp
HEADERS      += $$VISIOCYTEPATH/basic_c_fun/visiocyte_interface.h
HEADERS      += common/q_convolve.h
HEADERS      += common/q_imresize.h
HEADERS      += histogram_matching/q_histogram_matching.h
HEADERS      += q_paradialog_rigidaffine.h
HEADERS      += q_registration_common.h
HEADERS      += q_rigidaffine_registraFlyWorkstation_utilitiestion.h
HEADERS      += plugin_image_registration.h

SOURCES      += $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
#SOURCES      += $$VISIOCYTEPATH/basic_c_fun/stackutil.cpp
#SOURCES      += $$VISIOCYTEPATH/basic_c_fun/mg_image_lib.cpp
#SOURCES      += $$VISIOCYTEPATH/basic_c_fun/mg_utilities.cpp

SOURCES      += common/q_imresize.cpp
SOURCES      += common/q_convolve.cpp
SOURCES      += histogram_matching/q_histogram_matching.cpp
SOURCES      += q_paradialog_rigidaffine.cpp
SOURCES      += q_registration_common.cpp
SOURCES      += q_rigidaffine_registration.cpp
SOURCES      += plugin_image_registration.cpp

TARGET        = $$qtLibraryTarget(Janelia_plugin_PQ_imagereg)

DESTDIR       = $$VISIOCYTEPATH/../bin/plugins/FlyWorkstation_utilities/Janelia_SSD_registration


# the following were added for pdb file support, the modularity must be improved in this future!

LIBS         += -L$$VISIOCYTEPATH/common_lib/src_packages/mylib_tiff -lmylib

QT_DIR = $$dirname(QMAKE_QMAKE)/..
SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets
INCLUDEPATH += $$SHARED_FOLDER

#HEADERS      += $$VISIOCYTEPATH/neuron_annotator/utility/ImageLoaderBasic.h
#SOURCES      += $$VISIOCYTEPATH/neuron_annotator/utility/ImageLoaderBasic.cpp
#SOURCES      += $$VISIOCYTEPATH/basic_c_fun/basic_4dimage.cpp
#SOURCES      += $$VISIOCYTEPATH/basic_c_fun/basic_4dimage_create.cpp
#SOURCES      += $$VISIOCYTEPATH/basic_c_fun/imageio_mylib.cpp
