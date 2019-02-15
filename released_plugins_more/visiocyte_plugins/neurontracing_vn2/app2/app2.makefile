all:
	g++ -g -I../../../visiocyte_external/visiocyte_main/basic_c_fun \
   		-I../../../visiocyte_external/visiocyte_main/common_lib/include \
		-L../../../visiocyte_external/visiocyte_main/common_lib/lib -lvisiocytetiff\
		../../../visiocyte_external/visiocyte_main/basic_c_fun/stackutil.cpp \
		../../../visiocyte_external/visiocyte_main/basic_c_fun/mg_utilities.cpp \
		../../../visiocyte_external/visiocyte_main/basic_c_fun/mg_image_lib.cpp \
		*.cpp -o app2
