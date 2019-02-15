/* consensus_skeleton_plugin.cpp
 * a plugin to merge multiple neurons by generating a consensus skeleton
 * 2012-05-02 : by Yinan Wan
 */
 
#include "visiocyte_message.h"

#include "consensus_skeleton_plugin.h"
#include "consensus_skeleton_func.h"
 
Q_EXPORT_PLUGIN2(consensus_skeleton, ConsSkelPlugin);
 
QStringList ConsSkelPlugin::menulist() const
{
	return QStringList() 
		<<tr("consensus_skeleton")
		<<tr("about");
}

QStringList ConsSkelPlugin::funclist() const
{
	return QStringList()
		<<tr("consensus_skeleton")
		<<tr("help");
}

void ConsSkelPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("consensus_skeleton"))
	{
		consensus_skeleton_io(callback,parent);
	}
	else if (menu_name == tr("help"))
	{
		visiocyte_msg(tr("a plugin to merge multiple neurons by generating a consensus skeleton. "
			"Developed by Yinan Wan, 2012-05-02"));
	}
	else
	{
		visiocyte_msg(tr("a plugin to merge multiple neurons by generating a consensus skeleton. "
			"Developed by Yinan Wan, 2012-05-02"));
	}
}

bool ConsSkelPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("consensus_skeleton"))
	{
		return consensus_skeleton_io(input, output);
	}
	else if (func_name == tr("help"))
	{
		printHelp();
	}
}

