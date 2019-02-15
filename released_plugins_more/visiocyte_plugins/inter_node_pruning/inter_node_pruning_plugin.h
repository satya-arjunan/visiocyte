/* inter_node_pruning_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2014-09-03 : by Zhi Zhou
 */
 
#ifndef __INTER_NODE_PRUNING_PLUGIN_H__
#define __INTER_NODE_PRUNING_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class inter_node_pruning : public QObject, public VISIOCYTEPluginInterface2_1
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

