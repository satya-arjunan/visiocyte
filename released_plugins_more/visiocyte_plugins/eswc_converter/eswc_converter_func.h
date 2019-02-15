/* eswc_converter_func.h
 * This is a plugin to convert between swc and eswc.
 * 2012-02-16 : by Yinan Wan
 */
 
#ifndef __ESWC_CONVERTER_FUNC_H__
#define __ESWC_CONVERTER_FUNC_H__

#include <visiocyte_interface.h>
#include "visiocyte_message.h"
#include "eswc_core.h"
#include <vector>
#include <iostream>
using namespace std;

int swc2eswc_io(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool swc2eswc_io(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
int eswc2swc_io(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool eswc2swc_io(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
int check_eswc_io(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool check_eswc_io(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

int swc2eswc_toolbox(const VISIOCYTEPluginArgList & input);
int eswc2swc_toolbox(const VISIOCYTEPluginArgList & input);
int check_eswc_toolbox(const VISIOCYTEPluginArgList & input);

bool export_eswc(NeuronTree & input, vector<VISIOCYTELONG> seg_id, vector<VISIOCYTELONG> seg_layer, const char* infile_name, const char* outfile_name);
bool export_swc(NeuronTree & neuron, const char* filename);

void printHelp();
#endif

