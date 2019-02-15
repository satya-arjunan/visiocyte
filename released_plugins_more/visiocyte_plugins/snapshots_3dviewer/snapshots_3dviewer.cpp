#include <iostream>

#include "basic_surf_objs.h"
#include "snapshots_3dviewer.h"
#include "../../../visiocyte_main/neuron_editing/neuron_xforms.h"
using namespace std;
//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(snapshots_3dviewer, SnapShots_3Dviewer)


void snapShots3Dviewer(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);
void tile(VISIOCYTEPluginCallback2 & callback,    VisiocyteR_MainWindow * visiocytewin, double spacing_x,double spacing_y,int num_per_row);
void  snapShots3Dviewer_tiled_groups(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);

//plugin funcs
const QString title = "Saving Snapshots From 3D Viewer";
QStringList SnapShots_3Dviewer::menulist() const
{
	return QStringList()
            << tr("take a snapshot of 3D viewer")
            << tr("take a snapshot of 3D viewer:tiled objects")
            << tr("about");
}

void SnapShots_3Dviewer::domenu(const QString & menu_name,	VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
    if (menu_name == tr("take a snapshot of 3D viewer"))
	{
        snapShots3Dviewer(visiocyte, parent);
	}
    if (menu_name == tr("take a snapshot of 3D viewer:tiled objects"))
    {
        snapShots3Dviewer_tiled_groups(visiocyte, parent);
    }

}

void SnapShots_3Dviewer(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
}

void snapShots3Dviewer(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
    QFileDialog inANO_d(parent);
    inANO_d.setWindowTitle(QObject::tr("Choose input ano filename"));
    inANO_d.setAcceptMode(QFileDialog::AcceptOpen);
    if (!inANO_d.exec()) return;
    QString inANO_fn = (inANO_d.selectedFiles())[0];

    P_ObjectFileType cc;
    if(! loadAnoFile(inANO_fn,cc)){
        cout <<"Fail to load ano file" <<endl;
        return;
    }


    QString output_d=QFileDialog::getExistingDirectory(parent,
    QString(QObject::tr("Choose the output directory, where the snapshots would be saved.")));


    float x_rot = QInputDialog::getDouble(parent, "Roation about x axis",
                                 "Rotation about x :",
                                 90, -180, 180);


    float  y_rot = QInputDialog::getDouble(parent, "Roation about y axis",
                                 "Rotation about y :",
                                0, -180, 180);



    float z_rot = QInputDialog::getDouble(parent, "Roation about z axis",
                                 "Rotation about z :",
                                 90, -180, 180);

    for(int i = 0; i <cc.swc_file_list.size(); i++)
    {
        QString swc_file = cc.swc_file_list[i];

        visiocyte.open3DViewerForSingleSurfaceFile(swc_file);

        QList<VisiocyteR_MainWindow * > list_3dviewer = visiocyte.getListAll3DViewers();
        VisiocyteR_MainWindow * surface_win = list_3dviewer[i];
        if (!surface_win)
        {
            cout << "surface_win is empty"<<endl;
        }
        View3DControl  *view = visiocyte.getView3DControl_Any3DViewer(surface_win);
        view->doAbsoluteRot(x_rot,y_rot,z_rot);
        swc_file = QFileInfo(swc_file).fileName();
        QString BMPfilename = QDir(output_d).absolutePath()+ '/'+swc_file;
        //visiocyte_msg(BMPfilename);
        visiocyte.update_3DViewer(surface_win);
        visiocyte.screenShot_Any3DViewer(surface_win, BMPfilename);

        visiocyte.close3DWindow(surface_win);
    }

}

void tile(VISIOCYTEPluginCallback2 & callback,    VisiocyteR_MainWindow * visiocytewin, double spacing_x,double spacing_y,int num_per_row)
{
    if(visiocytewin){
        QList<NeuronTree> *  ntList=callback.getHandleNeuronTrees_Any3DViewer(visiocytewin);

        double move_x, move_y, move_z;

        for(int i=0; i<ntList->size(); i++){
            if (spacing_x ==0){
                //row by row display
                move_x = spacing_x*(i/num_per_row);
                move_y = spacing_y*(i%num_per_row);

            }else{
                move_x = spacing_x*(i%num_per_row);
                move_y = spacing_y*(i/num_per_row);
            }

            move_z = 0;
            NeuronTree* p = (NeuronTree*)&(ntList->at(i));
            proc_neuron_add_offset(p, move_x, move_y, move_z);
        }

        callback.update_NeuronBoundingBox(visiocytewin);
        callback.update_3DViewer(visiocytewin);
    }
}

void  snapShots3Dviewer_tiled_groups(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
    QStringList ano_file_list = QFileDialog::getOpenFileNames(parent,"ano files",
                      QDir::currentPath(),"ano files (*.ano);;All files (*.*)" );

    QDir out_qdir = QFileInfo(ano_file_list[0]).absoluteDir();


            //QFileDialog::getExistingDirectory(parent,  QString(QObject::tr("Choose the output directory, where the snapshots would be saved.")));

    float x_rot = QInputDialog::getDouble(parent, "Roation about x axis",
                                 "Rotation about x :",
                                 90, -180, 180);


    float  y_rot = QInputDialog::getDouble(parent, "Roation about y axis",
                                 "Rotation about y :",
                                0, -180, 180);


    float z_rot = QInputDialog::getDouble(parent, "Roation about z axis",
                                 "Rotation about z :",
                                 0, -180, 180);


    double spacing_x = QInputDialog::getDouble(parent, "Spacing in x",
                                               "Spacing in x :",
                                               200, 0, 100000);
    double spacing_y = QInputDialog::getDouble(parent, "Spacing in y",
                                               "Spacing in y :",
                                               0, 0, 100000);


    double num_per_row = QInputDialog::getInteger(parent, "Numer of objects per row",
                                               "Number per row :",
                                               5, 0, 100);




    VisiocyteR_MainWindow * surface_win = visiocyte.createEmpty3DViewer();
    if (!surface_win)
    {
        visiocyte_msg(QString("Failed to open an empty window!"));
        return;
    }

     QList<NeuronTree> * new_treeList = visiocyte.getHandleNeuronTrees_Any3DViewer (surface_win);



    //set background to be white

    //tile according to the soma z coordinates

    // due to the limitation of the current plugin iterface, 3d viewer window without
    // associated triview image window can not be closed
    // here is the work around:

    View3DControl  *view = visiocyte.getView3DControl_Any3DViewer(surface_win);
    //view->setBackgroundColor(1.0,1.0,1.0);

    for(int i = 0; i <ano_file_list.size(); i++)
    {
        new_treeList->clear();
        QString ano_file = ano_file_list[i];
       // read ano file

        P_ObjectFileType linker_object;
        if (!loadAnoFile(ano_file,linker_object))
        {
            fprintf(stderr,"Error in reading the linker file.\n");
            return ;
        }
        QStringList nameList = linker_object.swc_file_list;
        int neuronNum = nameList.size();
        for (VISIOCYTELONG i=0;i<neuronNum;i++)
        {
            NeuronTree tmp = readSWC_file(nameList.at(i));
            new_treeList->push_back(tmp);
        }

        visiocyte.setWindowDataTitle(surface_win, ano_file);
        view->doAbsoluteRot(x_rot,y_rot,z_rot);

        ano_file = QFileInfo(ano_file).fileName();
        QString BMPfilename = out_qdir.absolutePath()+ '/'+ano_file;
        cout<<"Save snapshot to :"<< BMPfilename.toStdString().c_str()<<endl;
        //visiocyte.update_3DViewer(surface_win);

        tile(visiocyte, surface_win,spacing_x, spacing_y,num_per_row);
        visiocyte.screenShot_Any3DViewer(surface_win, BMPfilename);

    }


}


