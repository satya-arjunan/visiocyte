/* mostVesselTracer.cpp

   Copyright 2010 Jingpeng Wu

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   
 * the plugin for most vessel tracing
 * by jpwu@CBMP, 20100419
 * tojingpeng@gmail.com
 */


#include "mostVesselTracer.h"
#include <fstream>
#include <iostream>


//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(mostVesselTracer, mostVesselTracerPlugin);

bool autotrace(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback2 &callback);


//plugin funcs
const QString title = "Trace vessels of MOST volume!";
QStringList mostVesselTracerPlugin::menulist() const
{
    return QStringList()
            << tr("Start tracing")
            << tr("About");
}

void mostVesselTracerPlugin::domenu(const QString & menu_name, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
    if (menu_name == tr("Set Seeds"))
    {
        setSeeds(visiocyte, parent);
    }
    else if (menu_name == tr("Start tracing"))
    {
        set_dialog(visiocyte, parent);

       // startVesselTracing( visiocyte , parent );
    }
    else
    {
        QString msg = QString("MOST Vessel Tracing Plugin version %1\n initially developed by Jingpeng Wu (jpwu@CBMP) and refined slightly by Hanchuan Peng")
                      .arg(getPluginVersion(), 1, 'f', 1);
        QMessageBox::information(parent, "Version info", msg);
    }
}

bool mostVesselTracerPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
    vector<char*> infiles, inparas, outfiles;
    if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
    if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
    if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    if (func_name == tr("MOST_trace"))
    {
        return autotrace(input, output,callback);
    }
    else if (func_name == tr("help"))
    {
        cout<<"Usage : visiocyte -x dllname -f MOST_trace -i <inimg_file> -p <ch> <th> <seed> <slip>"<<endl;
        cout<<endl;
        cout<<"ch           the input channel value, start from 1, default 1"<<endl;
        cout<<"th           the threshold value, default 20"<<endl;
        cout<<"seed         window size of the seed, default 20"<<endl;
        cout<<"slip         window size to slip from seed, default 20"<<endl;
        cout<<"The output swc file will be named automatically based on the input image file nmae"<<endl;
        cout<<endl;
        cout<<endl;
    }
    else return false;

    return true;
}


void setSeeds(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent )
{
    visiocytehandle curwin = visiocyte.currentImageWindow();
    if (!curwin)
    {
        visiocyte_msg("You don't have any image open in the main window.");
        return;
    }

    //ensure the 3d viewer window is open; if not, then open it
    visiocyte.open3DWindow(curwin);
}

void set_dialog(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent)
{
    int x_begin;
    int x_end;
    int x_distance;
    int y_begin;
    int y_end;
    int y_distance;
    int z_begin;
    int z_end;
    int z_distance;
    int pruning_flag;
    int c;
    QString swcfile;
    int sslip;
    x_flag = 0;
    y_flag = 0;
    z_flag = 0;
    pruning_flag = 0;


    visiocytehandle curwin = visiocyte.currentImageWindow();
    if (!curwin)
    {
        visiocyte_msg("You don't have any image open in the main window.");
        return;
    }

    Image4DSimple* img = visiocyte.getImage(curwin);
    if (!img || !img->valid())
        return;

    AdaTDialog dialog;

    dialog.endx->setMaximum(img->getXDim()); dialog.endx->setMinimum(1); dialog.endx->setValue(img->getXDim());
    dialog.endy->setMaximum(img->getYDim()); dialog.endy->setMinimum(1); dialog.endy->setValue(img->getYDim());
    dialog.endz->setMaximum(img->getZDim()); dialog.endz->setMinimum(1); dialog.endz->setValue(img->getZDim());

    dialog.ds->setText(QString(img->getFileName()) + "_MOST.swc");

    dialog.channel->setMaximum(img->getCDim()); dialog.channel->setMinimum(1);dialog.channel->setValue(1);

    if (dialog.exec()!=QDialog::Accepted)
    return;
    else
    {
        InitThreshold = dialog.threshould_value->value();
        res_x_all = dialog.resx->value();
        res_y_all = dialog.resy->value();
        res_z_all = dialog.resz->value();
        seed_size_all = dialog.size->value();
        sslip = dialog.slipsize->value();
        swcfile = dialog.ds->text();
        c = dialog.channel->value();

        QFile file(swcfile);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            visiocyte_msg("Cannot create the swc file to write, please try it again! ");
            return;
        }

        if(dialog.x_select->isChecked())
        {
            x_flag =1;
            x_begin = dialog.beginx->value();
            x_end = dialog.endx->value();
            x_distance = dialog.distancex->value();
        }
        if(dialog.y_select->isChecked())
        {
            y_flag =1;
            y_begin = dialog.beginy->value();
            y_end = dialog.endy->value();
            y_distance = dialog.distancey->value();
        }
        if(dialog.z_select->isChecked())
        {
            z_flag =1;
            z_begin = dialog.beginz->value();
            z_end = dialog.endz->value();
            z_distance = dialog.distancez->value();
        }
      //  visiocyte.open3DWindow(curwin);
        if(dialog.pruning->isChecked())
        {
            pruning_flag =1;
        }
        startVesselTracing(visiocyte,x_flag,y_flag,z_flag,x_begin,x_end,x_distance,y_begin,y_end,y_distance,z_begin,z_end,z_distance,swcfile,sslip,pruning_flag,c);

    }

    return;

}

bool autotrace(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback2 &callback)
{
    cout<<"Welcome to MOST tracing"<<endl;
    unsigned int c=1;
    int InitThreshold = 20;
    seed_size_all = 20;
    int slipsize=20;

    if (input.size()>=2)
    {
        vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
        cout<<paras.size()<<endl;
        if(paras.size() >= 1) c = atoi(paras.at(0));
        if(paras.size() >= 2) InitThreshold = atoi(paras.at(1));
        if(paras.size() >= 3) seed_size_all = atoi(paras.at(2));
        if(paras.size() >= 4) slipsize = atoi(paras.at(3));
    }

    char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);

    cout<<"ch = "<<c<<endl;
    cout<<"threshold = "<<InitThreshold<<endl;
    cout<<"inimg_file = "<<inimg_file<<endl;
    cout<<"seedsize = "<<seed_size_all<<endl;
    cout<<"slipsize = "<<slipsize<<endl;

    Image4DSimple *subject = callback.loadImage(inimg_file);
    if(!subject || !subject->valid())
    {
         visiocyte_msg("Fail to load the input image.",0);
         if (subject) {delete subject; subject=0;}
         return false;
    }

    if( c < 1 || c > subject->getCDim())
    {
         visiocyte_msg("Invalid channel input.",0);
         if (subject) {delete subject; subject=0;}
         return false;
    }

    VISIOCYTELONG N = subject->getXDim();
    VISIOCYTELONG M = subject->getYDim();
    VISIOCYTELONG P = subject->getZDim();

    VISIOCYTELONG pagesz = N*M*P;
    int datatype = subject->getDatatype();
    unsigned char *data1d = subject->getRawDataAtChannel(c-1);

    unsigned char *output_image=0;
    switch (datatype)
    {
    case VISIOCYTE_UINT8:
        try {output_image = new unsigned char [pagesz];}
        catch(...)  {visiocyte_msg("cannot allocate memory for output_image.",0); return false;}
        for(VISIOCYTELONG i = 0; i<pagesz; i++)
            output_image[i] = data1d[i];


        break;
        default: visiocyte_msg("Invalid data type. Do nothing.",0); return false;
    }


    MOSTImage img;
    img.setData( (unsigned char*)output_image, subject->getXDim(),subject->getYDim(),subject->getZDim(),subject->getCDim(),subject->getDatatype());
    QString swcfile = QString(inimg_file) + "_MOST.swc";

    LandmarkList seedList;
    QTime qtime_seed;
    qtime_seed.start();
    if(1)
    {
        for(int i =1;i<=N;i+=20)
        img.auto_detect_seedx(seedList,i,InitThreshold,seed_size_all);
    }
    if(1)
    {
        for(int i =1;i<=M;i+=20)
        img.auto_detect_seedy(seedList,i,InitThreshold,seed_size_all);
    }
    if(1)
    {
        for(int i =1;i<=P;i+=20)
        img.auto_detect_seedz(seedList,i,InitThreshold,seed_size_all);
    }

    qDebug("  cost time seed = %g sec", qtime_seed.elapsed()*0.001);
    static long init_flag = 0;

    for(init_flag = 0;init_flag<subject->getTotalUnitNumberPerChannel();init_flag++)
    {
        visited.push_back(false);
    }

    // converte the formate
    NeuronTree vt;
    QTime qtime;
    qtime.start();
    vt = img.trace_seed_list(seedList, visited,InitThreshold,1.0,1.0,1.0,swcfile,slipsize,0);
    qDebug("  cost time totol= %g sec", qtime.elapsed()*0.001);

    visiocyte_msg(QString("\nNow you can drag and drop the generated swc fle [%1] into Visiocyte.").arg(swcfile),0);
    if (subject) {delete subject; subject=0;}
    return true;


}




//void startVesselTracing ( VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent )
void startVesselTracing(VISIOCYTEPluginCallback2 &visiocyte,int xflag,int yflag,int zflag,int xbegin, int xend,int xdis,int ybegin,int yend,int ydis,int zbegin,int zend,int zdis,QString swcfile,int slipsize,int pruning_flag,int c)
{
    visiocytehandle curwin = visiocyte.currentImageWindow();
    if (!curwin)
    {
        visiocyte_msg("You don't have any image open in the main window.");
        return;
    }
    //ensure the 3d viewer window is open; if not, then open it
   // visiocyte.open3DWindow(curwin);

    // get land mark list

    LandmarkList seedList = visiocyte.getLandmark(curwin);

    Image4DSimple* oldimg = visiocyte.getImage(curwin);
    unsigned char* data1d = oldimg->getRawDataAtChannel(c-1);

    ImagePixelType pixeltype = oldimg->getDatatype();
    VISIOCYTELONG pagesz = oldimg->getTotalUnitNumberPerChannel();
    unsigned char *output_image=0;
    switch (pixeltype)
    {
    case VISIOCYTE_UINT8:
        try {output_image = new unsigned char [pagesz];}
        catch(...)  {visiocyte_msg("cannot allocate memory for output_image."); return;}

        for(VISIOCYTELONG i = 0; i<pagesz; i++)
            output_image[i] = data1d[i];

        break;
        default: visiocyte_msg("Invalid data type. Do nothing."); return;
    }

    VISIOCYTELONG in_sz[4];
    in_sz[0] =  oldimg->getXDim(); in_sz[1] =  oldimg->getYDim(); in_sz[2] =  oldimg->getZDim();in_sz[3] = 1;

   // simple_saveimage_wrapper(visiocyte, "temp.visiocyteraw",  (unsigned char *)output_image, in_sz, pixeltype);


    MOSTImage img;
    // set data
    img.setData( (unsigned char*)output_image, oldimg->getXDim(),oldimg->getYDim(),oldimg->getZDim(),oldimg->getCDim(),oldimg->getDatatype());


    if ( seedList.isEmpty() )
            {
               QTime qtime_seed;
               qtime_seed.start();
               //img.auto_detect_seedz(seedList,img.getZDim()/2);
               if(xflag)
               {
                   for(int i =xbegin;i<=xend;i+=xdis)
                   img.auto_detect_seedx(seedList,i,InitThreshold,seed_size_all);
                  // img.auto_detect_seedx(seedList,xend);
               }
               if(yflag)
               {
                   for(int i =ybegin;i<=yend;i+=ydis)
                   img.auto_detect_seedy(seedList,i,InitThreshold,seed_size_all);
                  // img.auto_detect_seedy(seedList,yend);
               }
               if(zflag)
               {
                   for(int i =zbegin;i<=zend;i+=zdis)
                   img.auto_detect_seedz(seedList,i,InitThreshold,seed_size_all);
                   //img.auto_detect_seedz(seedList,zend);
               }

               qDebug("  cost time seed = %g sec", qtime_seed.elapsed()*0.001);
            }
    // clear visited, only excute once
    static long init_flag = 0;
   /* if ( init_flag <= 0 )
    {
        visited.fill( false, oldimg->getTotalUnitNumber());
        init_flag ++;
    }*/
    for(init_flag = 0;init_flag<oldimg->getTotalUnitNumber();init_flag++)
        {
            visited.push_back(false);
        }

    // converte the formate
    NeuronTree vt;
    QTime qtime;
     qtime.start();
     vt = img.trace_seed_list(seedList, visited,InitThreshold,res_x_all,res_y_all,res_z_all,swcfile,slipsize,pruning_flag);
     qDebug("  cost time totol= %g sec", qtime.elapsed()*0.001);

     visiocyte_msg(QString("Now you can drag and drop the generated swc fle [%1] into Visiocyte.").arg(swcfile),1);


//    NeuronTree vt_old = visiocyte.getSWC(curwin);

    // visualization
  //  visiocyte.setLandmark(curwin, seedList);
   // visiocyte.setSWC(curwin,vt);
  //  visiocyte.pushObjectIn3DWindow(curwin);
  //  visiocyte.updateImageWindow(curwin);
    //img.~MOSTImage();
}

