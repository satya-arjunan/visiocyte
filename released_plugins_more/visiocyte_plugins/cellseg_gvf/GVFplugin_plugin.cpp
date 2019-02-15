/* GVFplugin_plugin.cpp
 * Implementation of Gradient Vector Flow cell body segmentation
 * 2014.01.06 : by BRL
 */
 
#include "visiocyte_message.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "dialog_watershed_para.h"
#include "volimg_proc.h"
#include "img_definition.h"
#include "GVFplugin_plugin.h"
#include "FL_gvfCellSeg.h"

#define USHORTINT16 unsigned short int

using namespace std;
Q_EXPORT_PLUGIN2(gvf_cellseg, GVFplugin);
 

QStringList GVFplugin::menulist() const
{
	return QStringList() 
        <<tr("Gradient vector flow based Segmentation")
        <<tr("About");
}

QStringList GVFplugin::funclist() const
{
	return QStringList()
        <<tr("gvf_segmentation")
        <<tr("help");
}

void GVFplugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("Gradient vector flow based Segmentation"))
    {


        // the GVF function wants a pointer to a Vol3DSimple, which I haven't seen before.
        // this code below generates it  (take from plugin_FL_cellseg)


        // check what's up with the current window: is there a valid image pointer?
        visiocytehandle curwin = callback.currentImageWindow();
        if (!curwin)
        {
            visiocyte_msg("Please open an image.");
            return;
        }



        Image4DSimple* subject = callback.getImage(curwin);
        QString m_InputFileName = callback.getImageName(curwin);

        if (!subject)
        {
            QMessageBox::information(0, "", QObject::tr("No image is open."));
            return;
        }
        if (subject->getDatatype()!=VISIOCYTE_UINT8)
        {
            QMessageBox::information(0, "", QObject::tr("This demo program only supports 8-bit data. Your current image data type is not supported."));
            return;
        }

        VISIOCYTELONG sz0 = subject->getXDim();
        VISIOCYTELONG sz1 = subject->getYDim();
        VISIOCYTELONG sz2 = subject->getZDim();
        VISIOCYTELONG sz3 = subject->getCDim();

        Image4DProxy<Image4DSimple> pSub(subject);

        VISIOCYTELONG channelsz = sz0*sz1*sz2;

        float *pLabel = 0;
        unsigned char *pData = 0;



        gvfsegPara segpara;  // set these fields one at a time:

        segpara.diffusionIteration=  5;
        segpara.fusionThreshold = 10;
        segpara.minRegion = 10;
        segpara.sigma = 3;  // doesn't seem to be used in the actual function?


        //input parameters
        bool ok1;
        int c=1;

        if (sz3>1) //only need to ask if more than one channel
        {
            c = QInputDialog::getInteger(parent, "Channel",
                                             "Choose channel for segmentation:",
                                             1, 1, sz3, 1, &ok1);
            c = c-1; //channels are indexed to 0 in Image4DSimple->getRawDataAtChannel
            if (!ok1)
                return;
        }

        // read in parameters

        segpara.diffusionIteration = QInputDialog::getInteger(parent, "Diffusion Iterations",
                                         "Choose Number of Diffusion Iterations:",
                                         5, 1, 10, 1, &ok1);
        if (!ok1)
            return;


        segpara.fusionThreshold = QInputDialog::getInteger(parent, "Fusion Threshold",
                                         "Choose Fusion Threshold :",
                                         2, 1, 10, 1, &ok1);
        if (!ok1)
            return;


        segpara.minRegion= QInputDialog::getInteger(parent, "Minimum Region",
                                         "Choose Minimum Region Size (voxels):",
                                         10, 1, 1000, 1, &ok1);
        if (!ok1)
            return;


        // allocate memory for the images

        Vol3DSimple <unsigned char> * tmp_inimg = 0;
        Vol3DSimple <USHORTINT16> * tmp_outimg = 0;
        try
        {
            tmp_inimg = new Vol3DSimple <unsigned char> (sz0, sz1, sz2);
            tmp_outimg = new Vol3DSimple <USHORTINT16> (sz0, sz1, sz2);
        }
        catch (...)
        {
            visiocyte_msg("Unable to allocate memory for processing.");
            if (tmp_inimg) {delete tmp_inimg; tmp_inimg=0;}
            if (tmp_outimg) {delete tmp_outimg; tmp_outimg=0;}
            return;
        }

        //copy image data into our new memory
        memcpy((void *)tmp_inimg->getData1dHandle(), (void *)subject->getRawDataAtChannel(c), sz0*sz1*sz2);

        //now do computation

        //bool b_res = gvfCellSeg(img3d, outimg3d, segpara);
        bool b_res = gvfCellSeg(tmp_inimg, tmp_outimg, segpara);

        // clear out temporary space
        if (tmp_inimg) {delete tmp_inimg; tmp_inimg=0;}

        if (!b_res)
        {
            visiocyte_msg("image segmentation  using gvfCellSeg()  failed \n");
        }
        else
        {   // now display the results

       // parameters for the new image data
            VISIOCYTELONG new_sz0 = tmp_outimg->sz0();
            VISIOCYTELONG new_sz1 = tmp_outimg->sz1();
            VISIOCYTELONG new_sz2 = tmp_outimg->sz2();
            VISIOCYTELONG new_sz3 = 1;
            VISIOCYTELONG tunits = new_sz0*new_sz1*new_sz2*new_sz3;

            //
            USHORTINT16 * outvol1d = new USHORTINT16 [tunits];

         //   USHORTINT16 * tmpImg_d1d = (USHORTINT16 *)(tmp_outimg->getData1dHandle());

            memcpy((void *)outvol1d, (void *)tmp_outimg->getData1dHandle(), tunits*sizeof(USHORTINT16));
            if (tmp_outimg) {delete tmp_outimg; tmp_outimg=0;} //free the space immediately for better use of memory

            Image4DSimple p4DImage;
            p4DImage.setData((unsigned char*)outvol1d, sz0, sz1, sz2, 1, VISIOCYTE_UINT16);

            visiocytehandle newwin = callback.newImageWindow();
            callback.setImage(newwin, &p4DImage);
            callback.setImageName(newwin, QString("Segmented Image"));
            callback.updateImageWindow(newwin);
        }

        return;
    }
	else
	{
        visiocyte_msg(tr("A plugin for cell segmentation using Gradient Vector Flow. "
            "Developed based on the source code developed by Tianming Liu, Fuhui Long, and Hanchuan Peng (2010-2014)"));
	}
}

bool GVFplugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{


    if (func_name == tr("gvf_segmentation"))
	{
        int c=1;

        gvfsegPara segpara;  // default values
        segpara.diffusionIteration = 5;
        segpara.fusionThreshold = 3;
        segpara.minRegion = 10;

        vector<char*> infiles, inparas, outfiles;
        if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
        if(input.size() >= 2)
        {
            inparas = *((vector<char*> *)input.at(1).p);
        if(inparas.size() >= 1) c = atoi(inparas.at(0));
        if(inparas.size() >= 2) segpara.diffusionIteration = atoi(inparas.at(1));
        if(inparas.size() >= 3) segpara.fusionThreshold = atoi(inparas.at(2));
        if(inparas.size() >= 4) segpara.minRegion = atoi(inparas.at(3));
        if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);
        }
        char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
        char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);




        cout<<"channel choice "<<c<<endl;
        cout<<"diffusion Iterations "<<segpara.diffusionIteration<<endl;
        cout<<"fusion threshold "<<segpara.fusionThreshold<<endl;
        cout<<"minimum region size "<<segpara.minRegion<<endl;
        cout<<"inimg_file = "<<inimg_file<<endl;
        cout<<"outimg_file = "<<outimg_file<<endl;

        // check the input parameters for invalid choices:

        if ((segpara.diffusionIteration<1)||(segpara.fusionThreshold<1)||(segpara.minRegion)<0)
        {
            cout<< "* * * * * * * * * * * * *  "<<endl;
            cout<< "*"<<endl;
            cout<< "INVALID parameter selection, type "<<endl;
            cout<<" visiocyte -x gvf_cellseg -f help "<<endl;
            cout<<"for more information"<<endl;
            cout<< "* * * * * * * * * * * * *  "<<endl;
            cout<<""<<endl;

return false;
        }


        VISIOCYTELONG in_sz[4];
        int datatype;
        unsigned char * data1d = 0;
        if(!simple_loadimage_wrapper(callback, inimg_file, data1d, in_sz, datatype))
        {
            cerr<<"load image "<<inimg_file<<" error!"<<endl;
            return false;
        }



        // allocate memory for the images

        Vol3DSimple <unsigned char> * tmp_inimg = 0;
        Vol3DSimple <USHORTINT16> * tmp_outimg = 0;
        try
        {
            tmp_inimg = new Vol3DSimple <unsigned char> (in_sz[0], in_sz[1], in_sz[2]);
            tmp_outimg = new Vol3DSimple <USHORTINT16> (in_sz[0], in_sz[1], in_sz[2]);
        }
        catch (...)
        {
            visiocyte_msg("Unable to allocate memory for processing.");
            if (tmp_inimg) {delete tmp_inimg; tmp_inimg=0;}
            if (tmp_outimg) {delete tmp_outimg; tmp_outimg=0;}
            return false;
        }



        //load image
        //
        Image4DSimple * temp4D = callback.loadImage(inimg_file);
        in_sz[0]=temp4D->getXDim();
        in_sz[1]=temp4D->getYDim();
        in_sz[2]=temp4D->getZDim();
        in_sz[3]=temp4D->getCDim();
                if (in_sz[3]==1)
                {
                    c=1;
                    cout<< "Image only has 1 channel, segmenting channel 1"<<endl;
                }

                if ((c>in_sz[3])||(c<1))
                {
                    cout<< "* * * * * * * * * * * * *  "<<endl;
                    cout<< "*"<<endl;
                    cout<< "INVALID CHANNEL SELECTION: User selected channel "<<c<< "; image has "<< in_sz[3] << "  channels."<<endl;
                    cout<< " aborting segmentation !"<<endl;
                    cout<<"*"<<endl;
                    cout<< "* * * * * * * * * * * * *  "<<endl;
                    cout<<""<<endl;

return false;
                }



//transfer single channel of data from Image4DSimple to old Vol3DSimple class

        memcpy((void *)tmp_inimg->getData1dHandle(), (void *)temp4D->getRawDataAtChannel(c-1), in_sz[0]*in_sz[1]*in_sz[2]);
        //now do computation using external function call

        bool b_res = gvfCellSeg(tmp_inimg, tmp_outimg, segpara);

        // clear out temporary space
        if (tmp_inimg) {delete tmp_inimg; tmp_inimg=0;}




        // now write to file
        in_sz[3]=1;//for single channel output from gvfCellSeg
        simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)tmp_outimg->getData1dHandle(), in_sz, 2); //the output of gvfCellSeg is in 16bit


        // clear out temporary space
        if (tmp_outimg) {delete tmp_outimg; tmp_outimg=0;}

        if (temp4D) {delete temp4D; temp4D=0;}






        cout<<"Finished GVF Segmentation"<<endl;
	}
	else if (func_name == tr("help"))
	{
        cout<<endl;
        cout<<endl;
        cout<<" GVF segmentation plugin usage :  visiocyte -x gvf_cellseg  -f  gvf_segmentation -i <input filename> -o <output filename -p <c> <iter> <fusion> <size>"<<endl;
        cout<<" parameters:    (default values in parentheses) "<<endl;
        cout<<"c        =  channel of input file to use for segmentation    (1) "<<endl;
        cout<<"iter     =  number of diffusion iterations  (must be > 0)    (5)    "<<endl;
        cout<<"fusion   =  fusion size threshold         (must be > 0)      (3)  "<<endl;
        cout<<"size     =  minimum region size in voxels                   (10)"<<endl;


    }
	else return false;

	return true;
}

