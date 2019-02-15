//by Hanchuan Peng
// 2012-Dec

#ifndef __VN_IMGPREPROCESS__
#define __VN_IMGPREPROCESS__

#include <visiocyte_interface.h>

#include "basic_memory.cpp"
#include "volimg_proc.h"


bool downsampling_img_xyz(unsigned char *inimg, VISIOCYTELONG in_sz[4], double dfactor_xy, double dfactor_z, unsigned char *&outimg, VISIOCYTELONG out_sz[4]);
void downsampling_marker_xyz(LandmarkList & listLandmarks, double dfactor_xy, double dfactor_z);
bool scaleintensity(Image4DSimple * p4dImage, int channo, double lower_th, double higher_th, double target_min, double target_max);
bool scale_img_and_convert28bit(Image4DSimple * p4dImage, int lb, int ub); //lb, ub: lower bound, upper bound

bool subvolumecopy(Image4DSimple * dstImg,
                   Image4DSimple * srcImg,
                   VISIOCYTELONG x0, VISIOCYTELONG szx,
                   VISIOCYTELONG y0, VISIOCYTELONG szy,
                   VISIOCYTELONG z0, VISIOCYTELONG szz,
                   VISIOCYTELONG c0, VISIOCYTELONG szc);

bool invertedsubvolumecopy(Image4DSimple * dstImg,
                   Image4DSimple * srcImg,
                   VISIOCYTELONG x0, VISIOCYTELONG szx,
                   VISIOCYTELONG y0, VISIOCYTELONG szy,
                   VISIOCYTELONG z0, VISIOCYTELONG szz,
                   VISIOCYTELONG c0, VISIOCYTELONG szc);

#endif

