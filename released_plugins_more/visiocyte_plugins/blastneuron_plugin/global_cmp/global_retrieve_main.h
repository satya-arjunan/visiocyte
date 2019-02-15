#ifndef __GLOBAL_RETRIEVE_MAIN_H__
#define	 __GLOBAL_RETRIEVE_MAIN_H__

#include <QtGlobal>
#include <vector>
#include "visiocyte_message.h"
#include <visiocyte_interface.h>
#include "neuron_retrieve.h"
using namespace std;
void printHelp_global_retrieve();
int global_retrieve_main(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
#endif
