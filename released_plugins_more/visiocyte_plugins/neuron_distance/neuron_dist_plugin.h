/* neuron_dist_plugin.h
 * The plugin to calculate distance between two neurons. Distance is defined as the average distance among all nearest pairs in two neurons.
 * 2012-05-04 : by Yinan Wan
 */
 
#ifndef __NEURON_DIST_PLUGIN_H__
#define __NEURON_DIST_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class NeuronDistPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 0.9f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

