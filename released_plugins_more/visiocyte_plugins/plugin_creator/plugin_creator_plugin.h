/* plugin_creator_plugin.h
 * This plugin is used to produce visiocyte plugin project from a template file
 * 2012-01-27 : by Hang Xiao
 */
 
#ifndef __PLUGIN_CREATOR_PLUGIN_H__
#define __PLUGIN_CREATOR_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class PluginCreatorPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const {return QStringList();}
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
	{return false;}
};


#endif

