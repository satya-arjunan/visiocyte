/* hier_label_plugin.cpp
 * This plugin heirachically segments the input neuron tree and label the nodes as features in eswc file.
 * 2012-05-04 : by Yinan Wan
 */
 
#include "visiocyte_message.h"

#include "hier_label_plugin.h"
#include "hier_label_func.h"
 
Q_EXPORT_PLUGIN2(hier_label, HierLablPlugin);
 
QStringList HierLablPlugin::menulist() const
{
	return QStringList() 
		<<tr("hierachical_labeling")
		<<tr("about");
}

QStringList HierLablPlugin::funclist() const
{
	return QStringList()
		<<tr("hierachical_labeling")
		<<tr("help");
}

void HierLablPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("hierachical_labeling"))
	{
		hierachical_labeling_io(callback,parent);
	}
	else
	{
		visiocyte_msg(tr("This plugin heirachically segments the input neuron tree and label the nodes as features in eswc file.. "
			"Developed by Yinan Wan, 2012-05-04"));
	}
}

bool HierLablPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("hierachical_labeling"))
	{
		return hierachical_labeling_io(input, output);
	}
	if (func_name == tr("TOOLBOXhierachical_labeling"))
	{
		hierachical_labeling_toolbox(input);
		return true;
	}
	else if (func_name == tr("help"))
	{
		printHelp();
	}
	return false;
}

