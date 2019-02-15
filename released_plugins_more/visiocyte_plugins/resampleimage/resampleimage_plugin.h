/* resampleimage_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2015-02-02 : by Zhi Zhou 
 */
 
#ifndef __RESAMPLEIMAGE_PLUGIN_H__
#define __RESAMPLEIMAGE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class resampleimage : public QObject, public VISIOCYTEPluginInterface2_1
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

