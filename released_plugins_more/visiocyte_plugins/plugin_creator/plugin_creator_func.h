/* plugin_creator_func.h
 * This plugin is used to produce visiocyte plugin project from a template file
 * 2012-01-27 : by Hang Xiao
 */
 
#ifndef __PLUGIN_CREATOR_FUNC_H__
#define __PLUGIN_CREATOR_FUNC_H__

#include <visiocyte_interface.h>

int create_plugin(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
int create_plugin_neuronrec(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
int create_plugin_neuronML(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
int create_demo1(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
int create_demo2(VISIOCYTEPluginCallback2 &callback, QWidget *parent);

void produce_demo1(string dir, string visiocyte_path);
void produce_demo2(string dir, string visiocyte_path, string plugin_name, string menu_name, string func_name);

QString getVisiocytePath();

#endif

