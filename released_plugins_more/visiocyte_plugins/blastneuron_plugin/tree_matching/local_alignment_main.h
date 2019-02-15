#ifndef __LOCAL_ALIGNMENT_H__
#define __LOCAL_ALIGNMENT_H__
#include <vector>
#include <visiocyte_interface.h>
#include "neuron_tree_align.h"
#include "my_surf_objs.h"

using namespace std;
void printHelp_local_alignment();
int local_alignment_main(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
#endif
