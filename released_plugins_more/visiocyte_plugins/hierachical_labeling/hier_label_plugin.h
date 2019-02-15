/* hier_label_plugin.h
 * This plugin heirachically segments the input neuron tree and label the nodes as features in eswc file.
 * 2012-05-04 : by Yinan Wan
 */
 
#ifndef __HIER_LABEL_PLUGIN_H__
#define __HIER_LABEL_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class HierLablPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

