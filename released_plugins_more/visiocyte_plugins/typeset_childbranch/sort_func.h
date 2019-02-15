/* sort_func.h
 * 2012-02-01 : by Yinan Wan
 */
 
#ifndef __SORT_FUNC_H__
#define __SORT_FUNC_H__

#include <visiocyte_interface.h>

void sort_menu(VISIOCYTEPluginCallback2 &callback);
bool sort_func(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void sort_toolbox(const VISIOCYTEPluginArgList & input);
void printHelp(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
void printHelp(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

#endif

