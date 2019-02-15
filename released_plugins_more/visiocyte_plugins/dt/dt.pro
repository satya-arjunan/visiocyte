
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEMAINPATH = ../../../visiocyte_main
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

INCLUDEPATH += main 

HEADERS += dt.h
HEADERS += my_surf_objs.h

SOURCES  = dt.cpp
SOURCES += my_surf_objs.cpp

SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET        = $$qtLibraryTarget(fast_dt)
DESTDIR       = $$VISIOCYTEMAINPATH/../bin/plugins/image_filters/Fast_Distance_Transform
