#ifndef __PRE_PROCESSING_MAIN_H__
#define __PRE_PROCESSING_MAIN_H__

#include <QtGlobal>
#include <vector>
#include "visiocyte_message.h"
#include <visiocyte_interface.h>
#include "prune_short_branch.h"
#include "resampling.h"
#include "align_axis.h"
using namespace std;

bool pre_processing_main(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void printHelp_pre_processing();
#endif
