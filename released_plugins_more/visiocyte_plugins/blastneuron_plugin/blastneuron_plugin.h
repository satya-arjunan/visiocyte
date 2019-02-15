/* blastneuron_plugin.h
 * BlastNeuron plugin for rapid retrieve and alignment of neuron morphologies, also includes pre-processing, inverse projection and batch feature computation
 * 2016-06-03 : by Yinan Wan
 */
 
#ifndef __BLASTNEURON_PLUGIN_H__
#define __BLASTNEURON_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class BlastNeuronPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

#endif

