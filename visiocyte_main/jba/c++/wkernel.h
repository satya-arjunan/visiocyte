//wkernel.h: the program to define kernels
// by Hanchuan Peng
// 2006-2011

#ifndef __WEIGHT_KERNEL__
#define __WEIGHT_KERNEL__

#include "visiocyte_basicdatatype.h"

enum KernelType {
    KT_CUBE_ALL1, 
    KT_SPHERE_ALL1};

class WeightKernel3D
{
public:
  double * data1d;
  VISIOCYTELONG totalLen;
  double *** data3d;
  VISIOCYTELONG xwid, ywid, zwid, rwid; 
  VISIOCYTELONG x0, y0, z0; 
  WeightKernel3D(VISIOCYTELONG xw, VISIOCYTELONG yw, VISIOCYTELONG zw);
  WeightKernel3D(VISIOCYTELONG r);
  ~WeightKernel3D();

  void cleanData();
  void initData();
  int createInternalData(VISIOCYTELONG xw, VISIOCYTELONG yw, VISIOCYTELONG zw, VISIOCYTELONG xw0, VISIOCYTELONG yw0, VISIOCYTELONG zw0);
  
  void generateKernel(KernelType t, int pp, int qq, int rr);
  bool valid() {return (!data1d || !data3d || totalLen<=0 || xwid<=0 || ywid<=0 || zwid<=0 || totalLen!=xwid*ywid*zwid) ? false : true; }
};


#endif

