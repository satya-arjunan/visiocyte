TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
VISIOCYTEPATH = ../../../../visiocyte_external
INCLUDEPATH	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun
INCLUDEPATH	+= ./include
macx{
    LIBS += ./lib/mac/libz.a    
    LIBS += ./lib/mac/libniftiio.a
    LIBS += ./lib/mac/libznz.a
    LIBS += ./lib/mac/libnifticdf.a
}

win32{
    LIBS += ./lib/win32/zlib.lib    
    LIBS += ./lib/win32/nifticdf.lib
    LIBS += ./lib/win32/niftiio.lib
    LIBS += ./lib/win32/znz.lib
}

unix:!macx {
    LIBS += ./lib/unix/libz.a
    LIBS += ./lib/unix/libniftiio.a
    LIBS += ./lib/unix/libznz.a
    LIBS += ./lib/unix/libnifticdf.a
}


HEADERS	+= NifTi_reader_plugin.h
SOURCES	+= NifTi_reader_plugin.cpp
SOURCES	+= $$VISIOCYTEPATH/visiocyte_main/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(NifTi_reader)
DESTDIR	= $$VISIOCYTEPATH/bin/plugins/data_IO/NIFTI_file_reader
