/* hier_label_func.h
 * This plugin heirachically segments the input neuron tree and label the nodes as features in eswc file.
 * 2012-05-04 : by Yinan Wan
 */
 
#ifndef __HIER_LABEL_FUNC_H__
#define __HIER_LABEL_FUNC_H__

#include <visiocyte_interface.h>

int hierachical_labeling_io(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool hierachical_labeling_io(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
bool hierachical_labeling_toolbox(const VISIOCYTEPluginArgList & input);

void printHelp();

#endif

