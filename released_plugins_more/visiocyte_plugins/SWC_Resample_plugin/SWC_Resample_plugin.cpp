/* SWC_Resample_plugin.cpp
 * This plugin allows users to resample displayed SWCs.
 * 2015-02-05 : by Xiaoxiao Liu
 */
 
#include "visiocyte_message.h"
#include <vector>
#include <QObject>
#include "SWC_Resample_plugin.h"
#include "../../../released_plugins/visiocyte_plugins/resample_swc/resampling.h"  // this resampling implementation should be in trunk
#include "../../../visiocyte_main/neuron_editing/neuron_xforms.h"



using namespace std;
Q_EXPORT_PLUGIN2(SWC_Resample, SWCResample);
 
static resampleDialog * mydialog = 0;


void MyComboBox::enterEvent(QEvent *e)
{
    updateList();
    QComboBox::enterEvent(e);
}

void MyComboBox::updateList()
{
    if (!m_visiocyte)
        return;

    QString lastDisplayfile = currentText();


    QList <VisiocyteR_MainWindow *> cur_list_3dviewer = m_visiocyte->getListAll3DViewers();

    QStringList items;
    int i;

    for (i=0; i<cur_list_3dviewer.count(); i++)
    {
        items << m_visiocyte->getImageName(cur_list_3dviewer[i]);
    }


    clear();
    addItems(items);

    int curDisplayIndex = -1; //-1 for invalid index
    for (i = 0; i < items.size(); i++)
        if (items[i] == lastDisplayfile)
        {
            curDisplayIndex = i;
            break;
        }

    if (curDisplayIndex>=0)
        setCurrentIndex(curDisplayIndex);

    update();

    return;
}

resampleDialog :: resampleDialog(VISIOCYTEPluginCallback2 &_visiocyte, QWidget *parent) :
    QDialog(parent), m_visiocyte(_visiocyte)
{
    this->combobox_win = new MyComboBox(&m_visiocyte); // combo box for selecting the window that contains swc objects
    this->combobox_win->updateList();

    QLabel *label_surface = new QLabel(QObject::tr("3D Viewer Window List: "));

    QLabel *label_steplength = new QLabel(QObject::tr("Fixed Step Length (distance between neighboring nodes"));

    this->spinbox_steplength = new QSpinBox;
    this->spinbox_steplength->setRange(1, 1024);
    this->spinbox_steplength->setSingleStep(1);
    this->spinbox_steplength->setValue(5);

    QPushButton *btn_Run = new QPushButton("Run");

    gridLayout = new QGridLayout();
    gridLayout->addWidget(label_surface, 1,0,1,5);
    gridLayout->addWidget(this->combobox_win, 2,0,1,5);
    gridLayout->addWidget(label_steplength,3,0,1,0);
    gridLayout->addWidget(this->spinbox_steplength, 4,0,1,0);
    gridLayout->addWidget(btn_Run, 5,0,1,0);

    setLayout(this->gridLayout);
    setWindowTitle(QString("Resample Parameters"));

    connect(btn_Run, SIGNAL(clicked()), this, SLOT(_slot_run()));
}


resampleDialog::~resampleDialog()
{
    if ( mydialog )
    {
        delete mydialog;
        mydialog = 0;
    }
}


void resampleDialog::_slot_run()
{
    int steplength = this->spinbox_steplength->value();

    //obtain the selected 3D viewer
    list_3dviewer = m_visiocyte.getListAll3DViewers();
    if (list_3dviewer.size() < 1)
    {
        visiocyte_msg("Please open up a SWC file from the main menu first!");
        return;
    }

    surface_win = list_3dviewer[combobox_win->currentIndex()];

    if (!surface_win){
        visiocyte_msg("Please open up a SWC file from the main menu first!");
        return;
    }

    //get the current displayed neurons in the selected 3d viewer
    QList<NeuronTree> * mTreeList = m_visiocyte.getHandleNeuronTrees_Any3DViewer(surface_win);


    // open up a new 3D viewer window
    QString originalTitle = m_visiocyte.getImageName(surface_win).remove("3D View [").remove("]");
    QString title = QString("Resampled ") +  originalTitle;


    bool b_found = false;
    VisiocyteR_MainWindow * new3DWindow = NULL;
    for (int j= 0; j < list_3dviewer.size(); j++)
    {
        if ( m_visiocyte.getImageName(list_3dviewer[j]).contains(title))
        {
            b_found = true;
            new3DWindow = list_3dviewer[j];
            break;
        }
    }

    if ( ! b_found)
    {
        new3DWindow = m_visiocyte.createEmpty3DViewer();
        if (!new3DWindow)
        {
            visiocyte_msg(QString("Failed to open an empty window!"));
            return;
        }
    }


    QList<NeuronTree> * new_treeList = m_visiocyte.getHandleNeuronTrees_Any3DViewer (new3DWindow);
    if (!new_treeList)
    {
        visiocyte_msg(QString("New 3D viewer has invalid neuron tree list"));
        return;
    }

    new_treeList->clear();
    for (int i = 0 ; i < mTreeList->size(); i++)
    {
        NeuronTree resultTree = resample( mTreeList->at(i),double(steplength));

        // need to reset the color to zero of display with color (using the types)
        resultTree.color.r = 0;
        resultTree.color.g = 0;
        resultTree.color.b = 0;
        resultTree.color.a = 0;

        new_treeList->push_back(resultTree);
    }

    //make a copy of the original tree list and shif them for visual comparision
    for (int i = 0 ; i < mTreeList->size(); i++)
       {
           NeuronTree  resultTree;
           QList <NeuronSWC> listNeuron;
           QHash <int, int>  hashNeuron;
           listNeuron.clear();
           hashNeuron.clear();

           for(int j = 0; j < mTreeList->at(i).listNeuron.size(); j++)
           {
               listNeuron.append(mTreeList->at(i).listNeuron.at(j));
               hashNeuron.insert(mTreeList->at(i).listNeuron.at(j).n, listNeuron.size()-1);
           }

           resultTree.listNeuron = listNeuron;
           resultTree.hashNeuron = hashNeuron;
           proc_neuron_add_offset(&resultTree, 0, 0, 100);

           // need to reset the color to zero of display with color (using the types)
           resultTree.color.r = 0;
           resultTree.color.g = 0;
           resultTree.color.b = 0;
           resultTree.color.a = 0;

        new_treeList->push_back( resultTree);
    }

    //m_visiocyte.pushObjectIn3DWindow(new3DWindow);  this does not work
    // in XFormWidget::pushObjectIn3DWindow, because the triView is updated?

    m_visiocyte.setWindowDataTitle(new3DWindow, title);

    //m_visiocyte.update_3DViewer(new3DWindow); // this will not update the bounding box ?, and adding this will only cuase
    //the window to delay the refreshing until user interaction

    m_visiocyte.update_NeuronBoundingBox(new3DWindow); //without this step, the neuron won't show...


}



QStringList SWCResample::menulist() const
{
    return QStringList()
            <<tr("Resample SWC Displayed in 3D Views")
           <<tr("about");
}

QStringList SWCResample::funclist() const
{
    return QStringList()
            <<tr("resample")
           <<tr("help");
}

void SWCResample::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("Resample SWC Displayed in 3D Views"))
    {
        mydialog = new resampleDialog(callback, parent);
        mydialog->show();

        return;
    }
    else
    {
        visiocyte_msg(tr("This plugin allows users to resample displayed SWCs.. "
                   "Developed by Xiaoxiao Liu, 2015-02-05"));
    }
}

bool SWCResample::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("resample"))
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

