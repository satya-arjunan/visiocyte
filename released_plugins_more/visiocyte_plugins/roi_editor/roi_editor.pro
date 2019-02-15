TEMPLATE      = lib
CONFIG       += qt plugin warn_off
INCLUDEPATH  += ../../../visiocyte_main/basic_c_fun
HEADERS       = roi_editor.h
SOURCES       = roi_editor.cpp
SOURCES      += ../../../visiocyte_main/basic_c_fun/visiocyte_message.cpp
TARGET        = $$qtLibraryTarget(roi_editor)
DESTDIR       = ../../../../visiocyte_external/bin/plugins/image_ROI/ROI_Editor
