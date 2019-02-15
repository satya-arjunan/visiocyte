#ifndef __COMPUTE_TUBULARITY_H__
#define __COMPUTE_TUBULARITY_H__

#include <visiocyte_interface.h>


double compute_anisotropy_sphere(const unsigned char * data1d, VISIOCYTELONG N, VISIOCYTELONG M, VISIOCYTELONG P, VISIOCYTELONG c,
                                                  VISIOCYTELONG x0, VISIOCYTELONG y0, VISIOCYTELONG z0,
                                                  VISIOCYTELONG radius);


#endif // __COMPUTE_TUBULARITY_H__

