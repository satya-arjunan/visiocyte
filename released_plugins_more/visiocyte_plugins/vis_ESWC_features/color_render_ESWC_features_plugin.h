/* color_render_ESWC_features_plugin.h
 * This plugin will render the feature values in ESWC file.
 * 2015-6-22 : by Hanbo Chen
 */
 
#ifndef __COLOR_RENDER_ESWC_FEATURES_PLUGIN_H__
#define __COLOR_RENDER_ESWC_FEATURES_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class neuron_render_ESWC_features : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    void docolor(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

