/* assemble_neuron_live_plugin.h
 * This plugin will connect fragments, break loop, and combine neuron files in live.
 * 2015-7-8 : by Hanbo Chen
 */
 
#ifndef __ASSEMBLE_NEURON_LIVE_PLUGIN_H__
#define __ASSEMBLE_NEURON_LIVE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class neuron_assembler_live : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

