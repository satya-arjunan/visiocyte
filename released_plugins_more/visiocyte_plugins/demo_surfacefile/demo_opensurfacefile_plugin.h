/* demo_opensurfacefile_plugin.h
 * demo for open a surface file
 * 2015-2-10 : by Hanchuan Peng
 */
 
#ifndef __DEMO_OPENSURFACEFILE_PLUGIN_H__
#define __DEMO_OPENSURFACEFILE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class DemoOpenSurfaceFilePlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

