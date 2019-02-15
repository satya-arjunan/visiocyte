#ifndef __BATCH_COMPUTE_MAIN_H__
#define __BATCH_COMPUTE_MAIN_H__

#include <QtGlobal>
#include <vector>
#include "visiocyte_message.h"
#include <visiocyte_interface.h>
using namespace std;
bool batch_compute_main(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void printHelp_batch_compute();
#endif
