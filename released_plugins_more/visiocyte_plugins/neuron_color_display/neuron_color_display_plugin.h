/* neuron_color_display_plugin.h
 * This plugin will set the color of the neuron displayed
 * 2015-6-12 : by Hanbo Chen
 */
 
#ifndef __NEURON_COLOR_DISPLAY_PLUGIN_H__
#define __NEURON_COLOR_DISPLAY_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>


class SetNeuronDisplayColor : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    void do1clickcolor(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

