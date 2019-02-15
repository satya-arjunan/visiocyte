/* smartTrace_plugin.h
 * This plugin will automatically trace and correct neuron reconstructions.
 * 2015-4-21 : by Hanbo Chen
 */
 
#ifndef __SMARTTRACE_PLUGIN_H__
#define __SMARTTRACE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class smartTrace_plugin : public QObject, public VISIOCYTEPluginInterface2_1
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

