
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun

HEADERS	+= Paint_plugin.h \
    scribblearea.h \
   # mainwindow.h \
   # mydialog.h \
    paint_dialog.h
SOURCES	+= Paint_plugin.cpp \
    scribblearea.cpp \
   # mainwindow.cpp \
   # mydialog.cpp \
    paint_dialog.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(paint)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/misc/paint/

RESOURCES += \
    paint.qrc


