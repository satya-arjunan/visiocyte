/* GVFplugin_plugin.h
 * Implementation of Gradient Vector Flow cell body segmentation
 * 2014.01.06 : by BRL
 */
 
#ifndef __GVFPLUGIN_PLUGIN_H__
#define __GVFPLUGIN_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class GVFplugin : public QObject, public VISIOCYTEPluginInterface2_1
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

