
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
unix {VISIOCYTEMAINPATH = ../../../visiocyte_main
}
win32{VISIOCYTEMAINPATH = ..\\..\\..\\visiocyte_main
}
INCLUDEPATH	+= $$VISIOCYTEMAINPATH/basic_c_fun
INCLUDEPATH += $$VISIOCYTEMAINPATH/common_lib/include

HEADERS	+= bioformats_loader_plugin.h
SOURCES	+= bioformats_loader_plugin.cpp
SOURCES	+= $$VISIOCYTEMAINPATH/basic_c_fun/visiocyte_message.cpp

TARGET	= $$qtLibraryTarget(bioformats_loader)
DESTDIR	= $$VISIOCYTEMAINPATH/../bin/plugins/data_IO/convert_and_load_with_Bioformats

win32 {
         QMAKE_POST_LINK += mkdir  $$VISIOCYTEMAINPATH\\..\\bin\\bioformats_tools &
         QMAKE_POST_LINK += xcopy  /f bioformats_tools  $$VISIOCYTEMAINPATH\\..\\bin\\bioformats_tools\\
      #  QMAKE_CLEAN += -r $$VISIOCYTEMAINDIR\\..\\bin\\visiocyte_external\\bin\\bioformats_tools
}
else {    #  copy my version of the Bioformats Tools package into the bin directory and then delete the .svn stuff
        QMAKE_POST_LINK = cp -r bioformats_tools $$VISIOCYTEMAINPATH/../../visiocyte_external/bin/
        QMAKE_CLEAN += -r $$VISIOCYTEMAINPATH/../../visiocyte_external/bin/bioformats_tools
}

