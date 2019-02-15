/* sync_swc_terafly_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2017-8-29 : by Zhi Zhou
 */
 
#include "visiocyte_message.h"
#include <vector>
#include "sync_swc_terafly_plugin.h"
using namespace std;
Q_EXPORT_PLUGIN2(sync_swc_terafly, sync_swc_terafly);

static lookPanel *panel = 0;

 
QStringList sync_swc_terafly::menulist() const
{
	return QStringList() 
		<<tr("sync")
		<<tr("about");
}

QStringList sync_swc_terafly::funclist() const
{
	return QStringList()
		<<tr("help");
}

void sync_swc_terafly::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("sync"))
	{
        if (panel)
        {
            panel->show();
            return;
        }
        else
        {
            panel = new lookPanel(callback, parent);
            if (panel)
            {
                panel->show();
                panel->raise();
                panel->move(100,100);
                panel->activateWindow();
            }
        }

    }
	else
	{
		visiocyte_msg(tr("This is a test plugin, you can use it as a demo.. "
			"Developed by Zhi Zhou, 2017-8-29"));
	}
}

lookPanel::lookPanel(VISIOCYTEPluginCallback2 &_visiocyte, QWidget *parent) :
    QDialog(parent), m_visiocyte(_visiocyte)
{
    gridLayout = new QGridLayout();
    QPushButton* sync     = new QPushButton("Sync (one shot)");
    QPushButton* set_markers     = new QPushButton("Set Annotations");

    gridLayout->addWidget(sync, 0,0);
    gridLayout->addWidget(set_markers, 1,0);
    setLayout(gridLayout);
    setWindowTitle(QString("Synchronize annotation "));
    connect(sync,     SIGNAL(clicked()), this, SLOT(_slot_sync_onetime()));
    connect(set_markers,     SIGNAL(clicked()), this, SLOT(_slot_set_markers()));

}

lookPanel::~lookPanel()
{
}


void lookPanel::_slot_sync_onetime()
{
    NeuronTree nt = m_visiocyte.getSWCTeraFly();
    NeuronTree  resultTree;
    QList <NeuronSWC> listNeuron;
    QHash <int, int>  hashNeuron;
    listNeuron.clear();
    hashNeuron.clear();

    for(int j = 0; j < nt.listNeuron.size(); j++)
    {
        listNeuron.append(nt.listNeuron.at(j));
        hashNeuron.insert(nt.listNeuron.at(j).n, listNeuron.size()-1);
    }

    resultTree.listNeuron = listNeuron;
    resultTree.hashNeuron = hashNeuron;
    resultTree.color.r = 0;
    resultTree.color.g = 0;
    resultTree.color.b = 0;
    resultTree.color.a = 0;

    QList <VisiocyteR_MainWindow *> list_3dviewer = m_visiocyte.getListAll3DViewers();
    int current_index = -1;
    for(int i=0; i<list_3dviewer.size();i++)
    {
        if(m_visiocyte.getImageName(list_3dviewer[i]) == "3D View [Annotations in TeraFly]")
        {
            current_index = i;
         }
    }

    LandmarkList terafly_landmarks = m_visiocyte.getLandmarkTeraFly();

    if(current_index == -1)
    {
        VisiocyteR_MainWindow * new3DWindow = NULL;
        new3DWindow = m_visiocyte.createEmpty3DViewer();
        QList<NeuronTree> * treeList = m_visiocyte.getHandleNeuronTrees_Any3DViewer (new3DWindow);
        treeList->push_back(resultTree);
        m_visiocyte.setWindowDataTitle(new3DWindow, "Annotations in TeraFly");
        m_visiocyte.setHandleLandmarkList_Any3DViewer(new3DWindow,terafly_landmarks);
        m_visiocyte.update_NeuronBoundingBox(new3DWindow);

    }else
    {
        VisiocyteR_MainWindow *surface_win = m_visiocyte.getListAll3DViewers()[current_index];
        QList<NeuronTree> * treeList = m_visiocyte.getHandleNeuronTrees_Any3DViewer (surface_win);
        treeList->clear();
        treeList->push_back(resultTree);
        m_visiocyte.setHandleLandmarkList_Any3DViewer(surface_win,terafly_landmarks);
        m_visiocyte.update_NeuronBoundingBox(surface_win);
    }
}

void lookPanel::_slot_set_markers()
{
    QList <VisiocyteR_MainWindow *> list_3dviewer = m_visiocyte.getListAll3DViewers();
    int current_index = -1;
    for(int i=0; i<list_3dviewer.size();i++)
    {
        if(m_visiocyte.getImageName(list_3dviewer[i]) == "3D View [Annotations in TeraFly]")
        {
            current_index = i;
         }
    }
    if(current_index != -1)
    {
        VisiocyteR_MainWindow *surface_win = m_visiocyte.getListAll3DViewers()[current_index];
        LandmarkList* updated_landmarks = m_visiocyte.getHandleLandmarkList_Any3DViewer(surface_win);
        QList<NeuronTree> * updated_treeList = m_visiocyte.getHandleNeuronTrees_Any3DViewer(surface_win);
        m_visiocyte.setLandmarkTeraFly(*updated_landmarks);
        if(updated_treeList->size()>0){
            NeuronTree updated_nt = updated_treeList->at(0);
            m_visiocyte.setSWCTeraFly(updated_nt);
        }

    }
}

bool sync_swc_terafly::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("help"))
	{
		visiocyte_msg("To be implemented.");
	}
	else return false;

	return true;
}

