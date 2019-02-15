/* lobeseg_func.h
 * This plugin sperate the two optic lobes (OLs) and the center brain (CB) of fluit fly brain. Or seperate just one lobe and the center brain with suitable parameters.
 * June 20, 2011 : by Hanchuan Peng and Hang Xiao
 */

#ifndef __LOBESEG_FUNC_H__
#define __LOBESEG_FUNC_H__

#include <visiocyte_interface.h>

int lobeseg_two_sides(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
int lobeseg_one_side_only(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool lobeseg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void printHelp();
#endif

