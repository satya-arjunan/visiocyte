/* tip_detection_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2018-3-14 : by Chao Wang
 */
 
#include "visiocyte_message.h"
#include <vector>
#include "tip_detection_plugin.h"
using namespace std;
Q_EXPORT_PLUGIN2(tip_detection, TestPlugin);
 
QStringList TestPlugin::menulist() const
{
	return QStringList() 
		<<tr("menu1")
		<<tr("menu2")
		<<tr("about");
}

QStringList TestPlugin::funclist() const
{
	return QStringList()
		<<tr("func1")
		<<tr("func2")
		<<tr("help");
}

void TestPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("menu1"))
	{
		visiocyte_msg("To be implemented.");
	}
	else if (menu_name == tr("menu2"))
	{
		visiocyte_msg("To be implemented.");
	}
	else
	{
		visiocyte_msg(tr("This is a test plugin, you can use it as a demo.. "
			"Developed by Chao Wang, 2018-3-14"));
	}
}

bool TestPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("func1"))
	{
		visiocyte_msg("To be implemented.");
	}
	else if (func_name == tr("func2"))
	{
		visiocyte_msg("To be implemented.");
	}
	else if (func_name == tr("help"))
	{
		visiocyte_msg("To be implemented.");
	}
	else return false;

	return true;
}

