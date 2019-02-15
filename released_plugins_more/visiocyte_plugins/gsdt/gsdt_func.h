/* gsdt_func.h
 * Perform distance transformation on grayscale image.
 * 2012-03-02 : by Hang Xiao
 */
 
#ifndef __GSDT_FUNC_H__
#define __GSDT_FUNC_H__

#include <visiocyte_interface.h>

bool gsdt(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool gsdt(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback2 &callback);

#endif

