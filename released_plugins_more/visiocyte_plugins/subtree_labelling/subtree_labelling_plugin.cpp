/* subtree_labelling_plugin.cpp
 * This tree labels dendrite id, segment id and branch order
 * 2015-6-19 : by Yujie Li
 */
 
#include "subtree_labelling_plugin.h"


using namespace std;
Q_EXPORT_PLUGIN2(subtree_labelling, subtree_label);

QStringList subtree_label::menulist() const
{
	return QStringList() 
		<<tr("subtree_labelling")
		<<tr("about");
}

QStringList subtree_label::funclist() const
{
	return QStringList()
		<<tr("func1")
		<<tr("func2")
		<<tr("help");
}

void subtree_label::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("subtree_labelling"))
	{
        subtree_dialog *dialog=new subtree_dialog(&callback);
        if (!dialog->get_swc_marker())
            return;
        dialog->build_connt_LUT();
        dialog->connected_components();
        dialog->assign_marker_type();
	}
	else
	{
        QString info="The subtree label plug-in is used to label subtrees. Each node in the subtree is assigned a subtree type,subtree id,segment id,branch order and the distance between the node to subtree root node."
                "The plug-in does not assume the precence of soma or a complete neuron tree."
                "If soma is defined by the user, it will be the root node of this subtree. In the presence of multiple markers on one subtree, the root node will the the first marker. The segment id and branch order of the subree root node is set to 1 and increase when there is a bifurcation."
                " The input is a sorted swc file and markers. The output is a eswc file";
        QMessageBox::information(0,"subtree plug-in information",info,QMessageBox::Ok);
	}
}


bool subtree_label::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
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


