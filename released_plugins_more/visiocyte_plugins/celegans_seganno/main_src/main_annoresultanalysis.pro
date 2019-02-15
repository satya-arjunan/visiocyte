
TEMPLATE      = app
CONFIG       += qt
macx: CONFIG	     -= app_bundle
#CONFIG       += x86_64  #this cannot be added as there will be 32bit compatibility issue. Should add on command line. by PHC, 101223
#QT          -= gui # Only the core module is used

VISIOCYTEMAINDIR = ../../visiocyte_main
INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun

HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.h
HEADERS      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.h

SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += $$VISIOCYTEMAINDIR/basic_c_fun/basic_surf_objs.cpp
SOURCES      += main_annoresultanalysis.cpp


