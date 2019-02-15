/* gsdt_plugin.h
 * Perform distance transformation on grayscale image.
 * 2012-03-02 : by Hang Xiao
 */
 
#ifndef __GSDT_PLUGIN_H__
#define __GSDT_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class GrayScaleDistanceTransformationPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

