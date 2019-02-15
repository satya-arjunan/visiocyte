/* resample_swc_func.h
 * This is a plugin to resample neuron swc subject to a fixed step length.
 * 2012-03-02 : by Yinan Wan
 */
 
#ifndef __RESAMPLE_SWC_FUNC_H__
#define __RESAMPLE_SWC_FUNC_H__

#include <visiocyte_interface.h>

int resample_swc(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool resample_swc(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
bool resample_swc_toolbox(const VISIOCYTEPluginArgList & input);

#endif

