/* resample_swc_plugin.h
 * This is a plugin to resample neuron swc subject to a fixed step length.
 * 2012-03-02 : by Yinan Wan
 */
 
#ifndef __RESAMPLE_SWC_PLUGIN_H__
#define __RESAMPLE_SWC_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class ResamplePlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

