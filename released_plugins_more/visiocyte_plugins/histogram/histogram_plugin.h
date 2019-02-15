/* histogram_plugin.h
 * Display histogram of the image
 * 2012-03-01 : by Jianlong Zhou
 */
 
#ifndef __HISTOGRAM_PLUGIN_H__
#define __HISTOGRAM_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class HistogramPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.12f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

