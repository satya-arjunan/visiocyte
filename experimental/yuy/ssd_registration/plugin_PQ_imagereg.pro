#plugin_image_registration.h
# by Lei Qu and Hanchuan Peng
# 2011

TEMPLATE      = lib
CONFIG       += plugin 
#CONFIG       += x86_64

VISIOCYTEPATH = ../../../visiocyte_main

INCLUDEPATH  += $$VISIOCYTEPATH/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEPATH/jba/newmat11

LIBS 	     += -L$$VISIOCYTEPATH/common_lib/lib -lvisiocytetiff
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
HEADERS      += q_rigidaffine_registration.h
HEADERS      += plugin_image_registration.h

SOURCES      += common/q_imresize.cpp
SOURCES      += common/q_convolve.cpp
SOURCES      += $$VISIOCYTEPATH/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEPATH/basic_c_fun/stackutil.cpp
SOURCES      += $$VISIOCYTEPATH/basic_c_fun/mg_image_lib.cpp
SOURCES      += $$VISIOCYTEPATH/basic_c_fun/mg_utilities.cpp
SOURCES      += histogram_matching/q_histogram_matching.cpp
SOURCES      += q_paradialog_rigidaffine.cpp
SOURCES      += q_registration_common.cpp
SOURCES      += q_rigidaffine_registration.cpp
SOURCES      += plugin_image_registration.cpp

TARGET        = $$qtLibraryTarget(plugin_PQ_imagereg)

DESTDIR       = ../../visiocyte/plugins/optic_lobe_aligner/ 

