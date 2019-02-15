/* histogram_func.h
 * Display histogram of the image
 * 2012-03-01 : by Jianlong Zhou
 */
 
#ifndef __HISTOGRAM_FUNC_H__
#define __HISTOGRAM_FUNC_H__

#include <vector>

#include <visiocyte_interface.h>

int compute(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool compute(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

#endif

