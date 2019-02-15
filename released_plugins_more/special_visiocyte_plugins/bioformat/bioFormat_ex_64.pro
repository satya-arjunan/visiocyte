
TEMPLATE	= lib
CONFIG		+= plugin warn_off
CONFIG		+= x86_64

INCLUDEPATH	+= /System/Library/Frameworks/JavaVM.framework/Headers \
               ../../../visiocyte_main/basic_c_fun \
                dependent/include
LIBS		 = -framework JavaVM \
				-L/usr/local/lib/ -lboost_thread \
				dependent/maclib/x86_64/libbfcpp.dylib \
				dependent/maclib/x86_64/libjace.dylib

HEADERS       = bioFormat_ex.h
SOURCES       = bioFormat_ex.cpp

TARGET        = $$qtLibraryTarget(readImage_bioformat)
QMAKE_POST_LINK = cp dependent/maclib/x86_64/* ../../visiocyte;

DESTDIR       =  ../../visiocyte/plugins/ImageIO_bioformat

