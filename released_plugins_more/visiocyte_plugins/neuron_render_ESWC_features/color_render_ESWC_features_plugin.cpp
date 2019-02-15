/* color_render_ESWC_features_plugin.cpp
 * This plugin will render the feature values in ESWC file.
 * 2015-6-22 : by Hanbo Chen
 */
 
#include "visiocyte_message.h"
#include <vector>
#include "color_render_ESWC_features_plugin.h"
#include "color_render_eswc_dialog.h"
using namespace std;
Q_EXPORT_PLUGIN2(color_render_ESWC_features, neuron_render_ESWC_features);
 
QStringList neuron_render_ESWC_features::menulist() const
{
	return QStringList() 
		<<tr("color_render_ESWC_features")
		<<tr("about");
}

QStringList neuron_render_ESWC_features::funclist() const
{
	return QStringList()
		<<tr("func1")
		<<tr("func2")
		<<tr("help");
}

void neuron_render_ESWC_features::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("color_render_ESWC_features"))
	{
        docolor(callback, parent);
	}
	else
	{
		visiocyte_msg(tr("This plugin will render the feature values in ESWC file.. "
			"Developed by Hanbo Chen, 2015-6-22"));
	}
}

bool neuron_render_ESWC_features::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
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

void neuron_render_ESWC_features::docolor(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    //select the window to operate
    QList <VisiocyteR_MainWindow *> allWindowList = callback.getListAll3DViewers();
    QList <VisiocyteR_MainWindow *> selectWindowList;
    VisiocyteR_MainWindow * visiocytewin;
    QList<NeuronTree> * ntTreeList;
    int winid;
    qDebug("search for 3D windows");
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++)
    {
        ntTreeList = callback.getHandleNeuronTrees_Any3DViewer(allWindowList[i]);
        if(ntTreeList->size()>0){
            qDebug()<<"cojoc:";
            for(VISIOCYTELONG j=0; j<ntTreeList->size(); j++){
                if(ntTreeList->at(j).listNeuron.at(0).fea_val.size()>0){
                    selectWindowList.append(allWindowList[i]);
                    break;
                }
            }
            qDebug()<<"cojoc:";
            qDebug()<<ntTreeList->at(0).listNeuron.at(0).fea_val.size();
        }
    }
    qDebug("match and select 3D windows");
    if(selectWindowList.size()<1){
        visiocyte_msg("Cannot find 3D view with eligible ESWC file. Please load the ESWC files with features you want to display 3D view");
        return;
    }else if(selectWindowList.size()>1){
        //pop up a window to select

        QStringList items;
        for(int i=0; i<selectWindowList.size(); i++){
            items.append(callback.getImageName(selectWindowList[i]));
        }
        bool ok;
        QString selectitem = QInputDialog::getItem(parent, QString::fromUtf8("Color Render ESWC"), QString::fromUtf8("Select A Window to Operate"), items, 0, false, &ok);
        if(!ok) return;
        for(int i=0; i<selectWindowList.size(); i++){
            if(selectitem==callback.getImageName(selectWindowList[i]))
            {
                winid=i;
                break;
            }
        }
    }else{
        winid=0;
    }
    visiocytewin = selectWindowList[winid];
    VISIOCYTELONG nid=0;
    ntTreeList = callback.getHandleNeuronTrees_Any3DViewer(visiocytewin);
    QList<VISIOCYTELONG> selectNeuronList;
    for(VISIOCYTELONG j=0; j<ntTreeList->size(); j++){
        if(ntTreeList->at(j).listNeuron.at(0).fea_val.size()>0){
            selectNeuronList.append(j);
        }
    }
    if(selectNeuronList.size()>1){
        //pop up a window to select

        QStringList items;
        for(int i=0; i<selectNeuronList.size(); i++){
            items.append(QString::number(i+1)+": "+ntTreeList->at(selectNeuronList.at(i)).file);
        }
        bool ok;
        QString selectitem = QInputDialog::getItem(parent, QString::fromUtf8("Color Render ESWC"), QString::fromUtf8("Select A Neuron to Color"), items, 0, false, &ok);
        if(!ok) return;
        for(int i=0; i<selectNeuronList.size(); i++){
            if(selectitem==items.at(i))
            {
                nid==selectNeuronList.at(i);
                break;
            }
        }
    }else{
        nid=selectNeuronList.at(0);
    }

    color_render_ESWC_dialog * myDialog = NULL;
    myDialog = new color_render_ESWC_dialog(&callback, visiocytewin, nid);
    myDialog->show();
}
