#ifndef __FUNC_H__
#define __FUNC_H__

#include <visiocyte_interface.h>
using namespace std;
int simple_func();

bool* shared_lib_func(unsigned char* raw, VISIOCYTELONG total_bytes,VISIOCYTELONG unit_bytes, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z, VISIOCYTELONG t,int paran, double* para, string fileDir);

#endif
