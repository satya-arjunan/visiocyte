/* Advantra_plugin.h
 * Tool for automatic neuron reconstruction from microscopy image stacks.
 * 2015-8-19 : by Miroslav Radojevic
 */
 
#ifndef __ADVANTRA_PLUGIN_H__
#define __ADVANTRA_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class Advantra : public QObject, public VISIOCYTEPluginInterface2_1
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

