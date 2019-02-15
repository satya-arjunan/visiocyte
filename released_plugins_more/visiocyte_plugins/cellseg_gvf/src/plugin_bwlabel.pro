
TEMPLATE      = lib
CONFIG       += qt plugin warn_off

VISIOCYTEMAINDIR    = ../../../../visiocyte_external/visiocyte_main/

INCLUDEPATH  += $$VISIOCYTEMAINDIR/basic_c_fun

HEADERS       = $$VISIOCYTEMAINDIR/basic_c_fun/img_definition.h
HEADERS       = $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_basicdatatype.h
HEADERS       = $$VISIOCYTEMAINDIR/basic_c_fun/volimg_proc.h
HEADERS      += FL_defType.h
HEADERS      += FL_accessory.h
HEADERS      += FL_neighborhood.h
HEADERS      += FL_unionFind.h
HEADERS      += FL_neighborhoodWalker.h
HEADERS      += FL_bwlabel2D3D.h
HEADERS      += bwlabel.h
HEADERS      += label_object_dialog.h


SOURCES       = $$VISIOCYTEMAINDIR/basic_c_fun/visiocyte_message.cpp
SOURCES      += bwlabel.cpp
SOURCES      += FL_bwlabel2D3D.cpp

TARGET        = $$qtLibraryTarget(fast_bwlabel)
DESTDIR       = $$VISIOCYTEMAINDIR/../visiocyte/plugins/unfinished/Fast_Label_Image_Objects

