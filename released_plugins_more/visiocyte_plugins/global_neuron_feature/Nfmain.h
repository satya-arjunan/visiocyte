#ifndef NFMAIN_H
#define NFMAIN_H
#include "basic_surf_objs.h"

void nf_main(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void nf_main(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
void nf_toolbox(const VISIOCYTEPluginArgList & input);
void printFeature(double * features);

#endif


