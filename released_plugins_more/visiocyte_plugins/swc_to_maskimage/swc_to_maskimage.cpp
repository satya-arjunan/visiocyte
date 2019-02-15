/*
 *  swc_to_maskimage .cpp
 *  swc_to_maskimage
 *
 *  Created by Yang, Jinzhu, on 11/27/10.
 *  Last change: by Hanchuan peng, 2010-Dec-8
 */

#include <QtGlobal>

#include "swc_to_maskimage.h"
#include "customary_structs/visiocyte_neurontoolbox_para.h"
#include "visiocyte_message.h"
#include "basic_surf_objs.h"
#include "stackutil.h"
#include <vector>
using namespace std;
static filter_dialog *dialog=0;


//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(swc_to_maskimage_sphere, SWC_TO_MASKIMAGElugin);


//plugin funcs
const QString title = "swc_to_maskimage";
QStringList SWC_TO_MASKIMAGElugin::menulist() const
{
    return QStringList()
    <<tr("swc_to_maskimage")
    <<tr("swc_filter")
    <<tr("About");

}
QStringList SWC_TO_MASKIMAGElugin::funclist() const
{
	return QStringList()
	<<tr("swc_to_maskimage")
    <<tr("swc_filter")
    <<tr("Help");
}
bool SWC_TO_MASKIMAGElugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
    if (func_name==tr("swc_to_maskimage"))
    {
        swc_to_maskimage(callback, input, output);
    }
    else if (func_name==tr("Help"))
    {
        printHelp();
    }
    else if (func_name==tr("swc_filter"))
    {
        swc_filter_image(callback,input,output);
        return true;
    }
    return false;
}

void SWC_TO_MASKIMAGElugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("swc_to_maskimage"))
	{
        swc_to_maskimage(callback, parent);
    }

    else if (menu_name == tr("swc_filter"))
    {
        dialog=new filter_dialog(&callback);
        dialog->setWindowTitle("swc_filter");
        dialog->show();
    }
    else if (menu_name == tr("About"))
	{
        visiocyte_msg("(version 1.0)<p> <b>'swc_to_maskimage'</b> converts a swc file to a mask image where the masked areas are 255 while the others are 0.<br>"
                "<b>'swc_filter'</b> directly uses a swc file to mask an image. The swc masked area in the image are non-zeros while other areas are set to 0.<p>"
                "The tool is developed by Jingzhu Yang and reimplemented by Yujie Li. Further question please contact yujie.jade@gmail.com");
    }

}

void swc_to_maskimage(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	NeuronTree neuron;
	double x_min,x_max,y_min,y_max,z_min,z_max;
	x_min=x_max=y_min=y_max=z_min=z_max=0;
	VISIOCYTELONG sx,sy,sz;
    unsigned char *pImMask = 0;
	QString filename;
	QStringList filenames;
    VISIOCYTELONG nx,ny,nz;

    filenames = QFileDialog::getOpenFileNames(0, 0,"","Supported file (*.swc)" ";;Neuron structure(*.swc)",0,0);

    if(filenames.isEmpty())
    {
        visiocyte_msg("You don't have any SWC file open in the main window.");
        return;
    }
    NeuronSWC *p_cur=0;

    for (VISIOCYTELONG i = 0; i < filenames.size();i++)
    {
        filename = filenames[i];
        if (filename.isEmpty())
        {
            visiocyte_msg("You don't have any SWC file open in the main window.");
            return;
        }
        neuron = readSWC_file(filename);
        for (VISIOCYTELONG ii=0; ii<neuron.listNeuron.size(); ii++)
        {
            p_cur = (NeuronSWC *)(&(neuron.listNeuron.at(ii)));
            if (p_cur->r<=0)
            {
                visiocyte_msg("You have illeagal radius values. Check your data.");
                return;
            }
         }
        BoundNeuronCoordinates(neuron,x_min,x_max,y_min,y_max,z_min,z_max);
        sx=x_max;
        sy=y_max;
        sz=z_max;
        VISIOCYTELONG stacksz = sx*sy*sz;
        pImMask = new unsigned char [stacksz];
        memset(pImMask,0,stacksz*sizeof(unsigned char));
        ComputemaskImage(neuron, pImMask, sx, sy, sz);
	}


	// compute coordinate region
    nx=sx;ny=sy;nz=sz;
	if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want set the image size?"), QMessageBox::Yes, QMessageBox::No))
	{
		SetsizeDialog dialog(callback, parent);
		dialog.coord_x->setValue(sx);
		dialog.coord_y->setValue(sy);
		dialog.coord_z->setValue(sz);
        if (dialog.exec()==QDialog::Accepted){
			nx = dialog.coord_x->text().toLong();
			ny = dialog.coord_y->text().toLong();
			nz = dialog.coord_z->text().toLong();
		}
    }

    unsigned char *pData = new unsigned char[nx*ny*nz];
    memset(pData,0,nx*ny*nz*sizeof(unsigned char));

    if(nx>=sx && ny>=sy && nz>=sz)
     {
         for (VISIOCYTELONG k1 = 0; k1 < sz; k1++){
            for(VISIOCYTELONG j1 = 0; j1 < sy; j1++){
                for(VISIOCYTELONG i1 = 0; i1 < sx; i1++){
                    pData[k1 * nx*ny + j1*nx + i1] = pImMask[k1*sx*sy + j1*sx +i1];
                }
            }
         }
     }
     else
     {
        visiocyte_msg("The input image size is smaller than the default size of swc file. You might only"
                " get part of the swc mask");
        for (VISIOCYTELONG k1 = 0; k1 < sz; k1++){
            for(VISIOCYTELONG j1 = 0; j1 < sy; j1++){
               for(VISIOCYTELONG i1 = 0; i1 < sx; i1++){
                   if ((i1>nx-1)||(j1>ny-1)||(k1>nz-1)) continue;
                   pData[k1 * nx*ny + j1*nx + i1] = pImMask[k1*sx*sy + j1*sx +i1];
               }
           }
        }
     }
    Image4DSimple tmp;
    tmp.setData(pData, nx, ny, nz, 1, VISIOCYTE_UINT8);
    visiocytehandle newwin = callback.newImageWindow();
    callback.setImage(newwin, &tmp);
    callback.setImageName(newwin, QString("Output_swc_mask"));
    callback.updateImageWindow(newwin);
    callback.open3DWindow(newwin);

}

bool swc_to_maskimage(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
    vector<char*> * pinfiles = (input.size() >= 1) ? (vector<char*> *) input[0].p : 0;
    vector<char*> * poutfiles = (output.size() >= 1) ? (vector<char*> *) output[0].p : 0;
    vector<char*> * pparas = (input.size() >= 2) ? (vector<char*> *) input[1].p : 0;

    vector<char*> infiles = (pinfiles != 0) ? * pinfiles : vector<char*>();
    vector<char*> outfiles = (poutfiles != 0) ? * poutfiles : vector<char*>();
    vector<char*> paras = (pparas != 0) ? * pparas : vector<char*>();
    if(infiles.size() != 1) return false;
    if(paras.size() != 0 && paras.size() != 3) return false;

    QString qs_input(infiles[0]);
    NeuronTree neuron = readSWC_file(qs_input);
    QString qs_output = outfiles.empty() ? qs_input + "_out.raw" : QString(outfiles[0]);
    NeuronSWC *p_cur=0;
    double x_min,x_max,y_min,y_max,z_min,z_max;
    x_min=x_max=y_min=y_max=z_min=z_max=0;
    VISIOCYTELONG sx,sy,sz;
    unsigned char* pImMask = 0;
    VISIOCYTELONG nx,ny,nz;

    for (VISIOCYTELONG ii=0; ii<neuron.listNeuron.size(); ii++)
    {
        p_cur = (NeuronSWC *)(&(neuron.listNeuron.at(ii)));
        if (p_cur->r<=0)
        {
            qDebug()<<"You have illeagal radius values. Check your data.";
            return false;
        }
     }
    BoundNeuronCoordinates(neuron,x_min,x_max,y_min,y_max,z_min,z_max);
    sx=x_max;
    sy=y_max;
    sz=z_max;
    VISIOCYTELONG stacksz = sx*sy*sz;
    pImMask = new unsigned char [stacksz];
    memset(pImMask,0,stacksz*sizeof(unsigned char));
    ComputemaskImage(neuron, pImMask, sx, sy, sz);

    // compute coordinate region
    if (paras.empty()) {nx = sx; ny = sy; nz = sz;}
    else {
        nx = atoi(paras[0]);
        ny = atoi(paras[1]);
        nz = atoi(paras[2]);
    }

    unsigned char *pData = new unsigned char[nx*ny*nz];
    memset(pData,0,nx*ny*nz*sizeof(unsigned char));

    if(nx>=sx && ny>=sy && nz>=sz)
     {
         for (VISIOCYTELONG k1 = 0; k1 < sz; k1++){
            for(VISIOCYTELONG j1 = 0; j1 < sy; j1++){
                for(VISIOCYTELONG i1 = 0; i1 < sx; i1++){
                    pData[k1 * nx*ny + j1*nx + i1] = pImMask[k1*sx*sy + j1*sx +i1];
                }
            }
         }
     }
     else
     {
        visiocyte_msg("The input image size is smaller than the default size of swc file. You might only"
                " get part of the swc mask",0);
        for (VISIOCYTELONG k1 = 0; k1 < sz; k1++){
            for(VISIOCYTELONG j1 = 0; j1 < sy; j1++){
               for(VISIOCYTELONG i1 = 0; i1 < sx; i1++){
                   if ((i1>=nx)||(j1>=ny)||(k1>=nz)) continue;
                   pData[k1 * nx*ny + j1*nx + i1] = pImMask[k1*sx*sy + j1*sx +i1];
               }
           }
        }
     }
    VISIOCYTELONG siz[4];
    siz[0] = nx; siz[1] = ny; siz[2] = nz; siz[3] = 1;
    simple_saveimage_wrapper(callback, qPrintable(qs_output), pData, siz, VISIOCYTE_UINT8);

    if (pData) {delete []pData; pData=0;}
    return true;
}

void swc_filter_image(VISIOCYTEPluginCallback2 & callback,const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
    vector<char*> * pinfiles = (input.size() >= 1) ? (vector<char*> *) input[0].p : 0;
    vector<char*> * poutfiles = (output.size() >= 1) ? (vector<char*> *) output[0].p : 0;

    vector<char*> infiles = (pinfiles != 0) ? * pinfiles : vector<char*>();
    vector<char*> outfiles = (poutfiles != 0) ? * poutfiles : vector<char*>();

    if(infiles.size() != 2) return;

    QString qs_input_image(infiles[0]);
    QString qs_input_swc(infiles[1]);

    unsigned char *image_data=0;
    VISIOCYTELONG sz_img[4];
    int intype=0;

    if (!qs_input_image.isEmpty())
    {
        if (!simple_loadimage_wrapper(callback, qs_input_image.toStdString().c_str(), image_data, sz_img, intype))
        {
            qDebug()<<"Loading error";
            return;
        }

        if (sz_img[3]>3)
        {
            sz_img[3]=3;
            visiocyte_msg("More than 3 channels were loaded."
                                     "The first 3 channel will be applied for analysis.",0);
            return;
        }

        VISIOCYTELONG size_tmp=sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3];
        if(intype!=1)
        {
            if (intype == 2) //VISIOCYTE_UINT16;
            {
                convert2UINT8((unsigned short*)image_data, image_data, size_tmp);
            }
            else if(intype == 4) //VISIOCYTE_FLOAT32;
            {
                convert2UINT8((float*)image_data, image_data, size_tmp);
            }
            else
            {
                visiocyte_msg("Currently this program only supports UINT8, UINT16, and FLOAT32 data type.",0);
                return;
            }
        }
    }

    NeuronTree neuron = readSWC_file(qs_input_swc);
    NeuronSWC *p_cur=0;
    for (VISIOCYTELONG ii=0; ii<neuron.listNeuron.size(); ii++)
    {
        p_cur = (NeuronSWC *)(&(neuron.listNeuron.at(ii)));
        if (p_cur->r<=0)
        {
            qDebug()<<"You have illeagal radius values. Check your data.";
            return;
        }
     }
    QString qs_output = outfiles.empty() ? qs_input_image + "_out.raw" : QString(outfiles[0]);


    double x_min,x_max,y_min,y_max,z_min,z_max;
    x_min=x_max=y_min=y_max=z_min=z_max=0;
    VISIOCYTELONG sx,sy,sz;
    unsigned char* pImMask = 0;
    VISIOCYTELONG nx,ny,nz;
    nx=sz_img[0]; ny=sz_img[1]; nz=sz_img[2];

    BoundNeuronCoordinates(neuron,x_min,x_max,y_min,y_max,z_min,z_max);
    sx=x_max;
    sy=y_max;
    sz=z_max;
    VISIOCYTELONG stacksz = sx*sy*sz;
    pImMask = new unsigned char [stacksz];
    memset(pImMask,0,stacksz*sizeof(unsigned char));
    ComputemaskImage(neuron, pImMask, sx, sy, sz);
    unsigned char * image_filter=new unsigned char[nx*ny*nz*sz_img[3]];
    memset(image_filter,0,nx*ny*nz*sz_img[3]*sizeof(unsigned char));

    for (VISIOCYTELONG k1 = 0; k1 < sz; k1++){
        for(VISIOCYTELONG j1 = 0; j1 < sy; j1++){
            for(VISIOCYTELONG i1 = 0; i1 < sx; i1++){
                 if ((i1>nx-1)||(j1>ny-1)||(k1>nz-1)) continue;

                 if (pImMask[k1*sx*sy + j1*sx +i1]>0)
                 {
                     image_filter[k1*nx*ny + j1*nx +i1]=image_data[k1*nx*ny + j1*nx +i1];
                     if (sz_img[3]>1){
                     image_filter[nx*ny*nz+k1*nx*ny + j1*nx +i1]=image_data[nx*ny*nz+k1*nx*ny + j1*nx +i1];
                     }
                     if (sz_img[3]>2){
                     image_filter[2*nx*ny*nz+k1*nx*ny + j1*nx +i1]=image_data[2*nx*ny*nz+k1*nx*ny + j1*nx +i1];
                     }
                 }

             }
         }
     }

    if (!simple_saveimage_wrapper(callback, qPrintable(qs_output), image_filter,sz_img, VISIOCYTE_UINT8))
    {
        visiocyte_msg("File saving error");
        return;
    }

    if (pImMask!=0) {delete []pImMask; pImMask=0;}
    if (image_filter!=0) {delete []image_filter; image_filter=0;}
    if (image_data!=0) {delete []image_data; image_data=0;}

}


void printHelp()
{
	printf("\nswc to mask image using sphere unit\n");
    printf("Usage visiocyte -x swc_to_maskimage_sphere_unit -f swc_to_maskimage -i <input.swc> [-p <sz0> <sz1> <sz2>] [-o <output_image.raw>]\n");
    printf("\nswc_filter -needs two input- 1) image 2) swc file \n");
    printf("Usage visiocyte -x swc_to_maskimage_sphere_unit -f swc_filter -i <input.tif> <input.swc> [-o <output_image.raw>]\n");
}


