/* subtree_labelling_plugin.h
 * This tree labels dendrite id, segment id and branch order
 * 2015-6-19 : by Yujie Li
 */
 
#ifndef __SUBTREE_LABELLING_PLUGIN_H__
#define __SUBTREE_LABELLING_PLUGIN_H__

#include "subtree_dialog.h"

class subtree_label : public QObject, public VISIOCYTEPluginInterface2_1
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

