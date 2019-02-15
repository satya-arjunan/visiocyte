# #####################################################################
# Created: 2009-10-12 visiocyte_mactiger v0.1 
# ######################################################################

message(CONFIG=$$unique(CONFIG))

include(visiocyte.pro)
macx {
LIBS -= -L../common_lib/lib_mac32
LIBS -= -L./common_lib/lib_mac32
LIBS += -L../common_lib/lib_mac32_tiger
}

