#ifndef READ_RAW_FILE_H
#define READ_RAW_FILE_H

#include "basic_surf_objs.h"
int loadRawRegion(char * filename, unsigned char * & img, VISIOCYTELONG * & sz,VISIOCYTELONG * & region_sz, int & datatype,
                  VISIOCYTELONG startx, VISIOCYTELONG starty, VISIOCYTELONG startz,
                  VISIOCYTELONG endx, VISIOCYTELONG endy, VISIOCYTELONG endz);

#endif // READ_RAW_FILE_H
