//histeq.h
//by Hanchuan Peng
//2006-2011

#ifndef __HISTOGRAM_EQ__
#define __HISTOGRAM_EQ__

#include "../../basic_c_fun/visiocyte_basicdatatype.h"

bool hist_eq_uint8(unsigned char * data1d, VISIOCYTELONG len); 
bool hist_eq_range_uint8(unsigned char * data1d, VISIOCYTELONG len, unsigned char lowerbound, unsigned char upperbound); 

#endif

