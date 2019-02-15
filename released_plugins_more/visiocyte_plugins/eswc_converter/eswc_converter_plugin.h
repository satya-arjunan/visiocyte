/* eswc_converter_plugin.h
 * This is a plugin to convert between swc and eswc.
 * 2012-02-16 : by Yinan Wan
 */
 
#ifndef __ESWC_CONVERTER_PLUGIN_H__
#define __ESWC_CONVERTER_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class ESWCPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

