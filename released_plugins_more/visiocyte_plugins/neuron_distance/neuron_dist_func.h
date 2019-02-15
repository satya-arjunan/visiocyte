/* neuron_dist_func.h
 * The plugin to calculate distance between two neurons. Distance is defined as the average distance among all nearest pairs in two neurons.
 * 2012-05-04 : by Yinan Wan
 */
 
#ifndef __NEURON_DIST_FUNC_H__
#define __NEURON_DIST_FUNC_H__

#include <visiocyte_interface.h>

int neuron_dist_io(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool neuron_dist_io(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
bool neuron_dist_toolbox(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginCallback2 & callback);
void printHelp();

int neuron_dist_mask(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool neuron_dist_mask(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);


#endif

