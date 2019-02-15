/* multi_channel_swc_plugin.h
 * This plugin will creat swc files based on multiple channel information in the neuron image.
 * 2015-6-18 : by Sumit and Hanbo
 */
 
#ifndef __MULTI_CHANNEL_SWC_PLUGIN_H__
#define __MULTI_CHANNEL_SWC_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class MultiChannelSWC : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

void do_calculate_render_eswc();
void printHelp();
#endif

