
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
unix {VISIOCYTEMAINPATH = ../../../visiocyte_main
}
win32{VISIOCYTEMAINPATH = ..\\..\\..\\visiocyte_main
}
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS	+= movieConverter_plugin.h
SOURCES	+= movieConverter_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(movie_format_convert)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/movies_and_snapshots/Movie_Converter

macx{
    QMAKE_POST_LINK = cp ./bin/mac_ffmpeg $$VISIOCYTEMAINPATH/../bin/.
}

unix:!macx {
    QMAKE_POST_LINK = cp ./bin/linux_ffmpeg $$VISIOCYTEMAINPATH/../bin/.
}

win32{
    QMAKE_POST_LINK = copy .\\bin\\windows_ffmpeg.exe $$VISIOCYTEMAINPATH\\..\\bin\\.
}

