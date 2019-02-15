TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += x86_64
VISIOCYTEMAINDIR = ../../../visiocyte_main 

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun
INCLUDEPATH  += $$VISIOCYTEMAINDIR/common_lib/include
INCLUDEPATH  += $$VISIOCYTEMAINDIR/neuron_editing

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h
HEADERS      += Neuron_tracing.h
HEADERS      += parser.h
HEADERS      += NeuronSegmentation.h
HEADERS      += NeuronEnhancementFilter.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_editing/neuron_format_converter.h
HEADERS      += $$VISIOCYTEMAINDIR/neuron_editing/v_neuronswc.h

SOURCES      += NeuronSegmentation.cpp
SOURCES      += NeuronEnhancementFilter.cpp


unix {
    LIBS     += -lm -lpthread
}

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
SOURCES      += Neuron_tracing.cpp
SOURCES      += parser.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
TARGET        = $$qtLibraryTarget(SimpleTracing)
DESTDIR       = $$VISIOCYTEMAINDIR/../bin/plugins/neuron_tracing/SimpleTracing
