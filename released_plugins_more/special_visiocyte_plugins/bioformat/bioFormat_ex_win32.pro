
TEMPLATE	= lib
CONFIG		+= plugin warn_off

INCLUDEPATH	  =  ../../../visiocyte_main/basic_c_fun \
                dependent/include \
                "C:\Program Files (x86)\Java\jdk1.6.0_21\include" \
                "C:\Program Files (x86)\Java\jdk1.6.0_21\include\win32" \
                "C:\work\bioformat\boost_1_43_0"
                
LIBS          = -L./dependent/winlib/x86/ -llibboost_thread-vc90-mt-1_43 \
				-llibboost_date_time-vc90-mt-1_43 \
				C:\work\visiocyte_2.0\plugin_demo\bioformat_lib\winlib\x86\jace.lib \
				C:\work\visiocyte_2.0\plugin_demo\bioformat_lib\winlib\x86\bfcpp.lib \
				"C:\Program Files (x86)\Java\jdk1.6.0_21\lib\jvm.lib"
				
HEADERS       = bioFormat_ex.h
SOURCES       = bioFormat_ex.cpp

HEADERS		 +=  ../../../visiocyte_main/basic_c_fun/mg_utilities.h \
				 ../../../visiocyte_main/basic_c_fun/mg_image_lib.h \
				 ../../../visiocyte_main/basic_c_fun/stackutil.h
				
SOURCES      +=  ../../../visiocyte_main/basic_c_fun/mg_utilities.cpp \
				 ../../../visiocyte_main/basic_c_fun/mg_image_lib.cpp \
				 ../../../visiocyte_main/basic_c_fun/stackutil.cpp
				
LIBS		 += -L../../../visiocyte_main/common_lib/winlib/ -llibtiff

TARGET        = $$qtLibraryTarget(readImage_bioformat)
#QMAKE_POST_LINK = copy ../bioformat_lib/winlib/x64/* ../../visiocyte;

DESTDIR       = ../../visiocyte/plugins/ImageIO_bioformat

