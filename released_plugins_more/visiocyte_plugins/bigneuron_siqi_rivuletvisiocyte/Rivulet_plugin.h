/* Rivulet_plugin.h
 * Rivulet algorithm for 3D neuron tracing. 
 * 2015-8-25 : by Siqi Liu, Donghao Zhang
 */
 
#ifndef __RIVULET_PLUGIN_H__
#define __RIVULET_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class RivuletPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

