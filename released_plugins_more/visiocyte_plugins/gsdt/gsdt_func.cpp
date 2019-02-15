/* gsdt_func.cpp
 * Perform distance transformation on grayscale image.
 * 2012-03-02 : by Hang Xiao
 */

#include <visiocyte_interface.h>
#include "visiocyte_message.h"
#include "gsdt_func.h"
#include <vector>
#include <iostream>

#include "stackutil.h"
#include "fastmarching_dt.h"
#include "common_dialog.h"

using namespace std;

const QString title = QObject::tr("Grayscale Distance Transformation Plugin");

bool gsdt(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	visiocytehandleList win_list = callback.getImageWindowList();

	if(win_list.size()<1)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return -1;
    }
	visiocytehandle curwin = callback.currentImageWindow();
	Image4DSimple * p4DImage = callback.getImage(curwin);
    if (!p4DImage || !p4DImage->valid())
    {
        visiocyte_msg("The image is not valid. Do nothing.");
        return false;
    }

	VISIOCYTELONG sz0 = p4DImage->getXDim();
	VISIOCYTELONG sz1 = p4DImage->getYDim();
	VISIOCYTELONG sz2 = p4DImage->getZDim();
	VISIOCYTELONG sz3 = p4DImage->getCDim();

	vector<string> items;
	items.push_back("Background Threshold (0 ~ 255)");
	items.push_back("Connection Type (1 ~ 3)");
	items.push_back("Channel (0 ~ )");
	items.push_back("Z_thickness");
	CommonDialog dialog(items);
	dialog.setWindowTitle(title);
	if(dialog.exec() != QDialog::Accepted) return 0;

	int bkg_thresh = 0, cnn_type = 2, channel = 0, z_thickness = 1.0;
	dialog.get_num("Background Threshold (0 ~ 255)", bkg_thresh);
	dialog.get_num("Connection Type (1 ~ 3)", cnn_type);
	dialog.get_num("Channel (0 ~ )", channel);
	dialog.get_num("Z_thickness", z_thickness);
	if(bkg_thresh < 0) bkg_thresh = 0;
	if(z_thickness == 0.0) z_thickness = 1.0;
	if(cnn_type < 1 || cnn_type > 3 || channel < 0 || channel >= sz3)
	{
		visiocyte_msg(QObject::tr("Connection type or channel value is out of range").arg(sz3-1));
        return false;
	}

	cout<<"bkg_thresh = "<<bkg_thresh<<endl;
	cout<<"cnn_type = "<<cnn_type<<endl;
	cout<<"channel = "<<channel<<endl;
	cout<<"z_thickness = "<<z_thickness<<endl;

	unsigned char * inimg1d = p4DImage->getRawDataAtChannel(channel);
    float * phi = 0;

    switch(p4DImage->getDatatype())
    {
    case VISIOCYTE_UINT8:
        fastmarching_dt(inimg1d, phi, sz0, sz1, sz2, cnn_type, bkg_thresh, z_thickness);
        break;
    case VISIOCYTE_UINT16:
        fastmarching_dt((unsigned short int *)inimg1d, phi, sz0, sz1, sz2, cnn_type, bkg_thresh, z_thickness);
        break;
    case VISIOCYTE_FLOAT32:
        fastmarching_dt((float *)inimg1d, phi, sz0, sz1, sz2, cnn_type, bkg_thresh, z_thickness);
        break;
    default:
        visiocyte_msg("You should have never seen this warning in GSDT.");
        return false;
    }

	float min_val = phi[0], max_val = phi[0];
    VISIOCYTELONG tol_sz = sz0 * sz1 * sz2;

	unsigned char * outimg1d = new unsigned char[tol_sz];
    for(VISIOCYTELONG i = 0; i < tol_sz; i++) {if(phi[i] == INF) continue; min_val = MIN(min_val, phi[i]); max_val = MAX(max_val, phi[i]);}
	cout<<"min_val = "<<min_val<<" max_val = "<<max_val<<endl;
	max_val -= min_val; if(max_val == 0.0) max_val = 0.00001;
    for(VISIOCYTELONG i = 0; i < tol_sz; i++)
	{
		if(phi[i] == INF) outimg1d[i] = 0;
		else if(phi[i] ==0) outimg1d[i] = 0;
		else
		{
			outimg1d[i] = (phi[i] - min_val)/max_val * 255 + 0.5;
			outimg1d[i] = MAX(outimg1d[i], 1);
		}
	}
	delete [] phi; phi = 0;

	Image4DSimple * new4DImage = new Image4DSimple();
    new4DImage->setData(outimg1d, sz0, sz1, sz2, 1, VISIOCYTE_UINT8);
	visiocytehandle newwin = callback.newImageWindow();
	callback.setImage(newwin, new4DImage);
	callback.setImageName(newwin, title);
	callback.updateImageWindow(newwin);

    return true;
}

bool gsdt(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback2 &callback)
{
	cout<<"Welcome to gsdt!"<<endl;
	if(input.size() != 2 || output.size() != 1) return false;
	int bkg_thresh = 0, cnn_type = 2, channel = 0, z_thickness = 1.0;
	vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
	if(paras.size() >= 1) bkg_thresh = atoi(paras.at(0));
	if(paras.size() >= 2) cnn_type = atoi(paras.at(1));
    if(paras.size() >= 3) channel = atoi(paras.at(2));
	if(paras.size() >= 4) z_thickness = atof(paras.at(3));

	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
	cout<<"bkg_thresh = "<<bkg_thresh<<endl;
	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;
	cout<<"channel = "<<channel<<endl;
	cout<<"z_thickness = "<<z_thickness<<endl;

    unsigned char * inimg1d = 0,  * outimg1d = 0;
    float * phi = 0;
    VISIOCYTELONG  in_sz[4];
	int datatype;
    if(!simple_loadimage_wrapper(callback,inimg_file, inimg1d, in_sz, datatype)) {cerr<<"load image "<<inimg_file<<" error!"<<endl; return 1;}
    if(in_sz[3] < channel+1)
    {
        cerr<<"invalid channel number!"<<endl;
        delete [] inimg1d; inimg1d = 0;
        return false;
    }

    VISIOCYTELONG pagesz = in_sz[0]*in_sz[1]*in_sz[2];
    unsigned char * inimg1d_ch1 = 0;
    try {inimg1d_ch1 = new unsigned char [pagesz];}
    catch(...)  {visiocyte_msg("cannot allocate memory for inimg1d_ch1.",0); return false;}
    for(VISIOCYTELONG i = 0; i<pagesz; i++)
        inimg1d_ch1[i] = inimg1d[channel*pagesz+i];
    delete [] inimg1d; inimg1d = 0;

     if(datatype == 1)
     {
          if(!fastmarching_dt(inimg1d_ch1, phi, in_sz[0], in_sz[1], in_sz[2], cnn_type, bkg_thresh, z_thickness)) return false;
     }
     else if(datatype == 2)
     {
          if(!fastmarching_dt((unsigned short int*)inimg1d_ch1, phi, in_sz[0], in_sz[1], in_sz[2], cnn_type, bkg_thresh, z_thickness)) return false;
     }
     else if(datatype == 4)
     {
          if(!fastmarching_dt((float*)inimg1d_ch1, phi, in_sz[0], in_sz[1], in_sz[2], cnn_type, bkg_thresh, z_thickness)) return false;
     }
	float min_val = phi[0], max_val = phi[0];
    VISIOCYTELONG tol_sz = in_sz[0] * in_sz[1] * in_sz[2];
	outimg1d = new unsigned char[tol_sz];
    for(VISIOCYTELONG i = 0; i < tol_sz; i++) {if(phi[i] == INF) continue; min_val = MIN(min_val, phi[i]); max_val = MAX(max_val, phi[i]);}
	cout<<"min_val = "<<min_val<<" max_val = "<<max_val<<endl;
	max_val -= min_val; if(max_val == 0.0) max_val = 0.00001;
    for(VISIOCYTELONG i = 0; i < tol_sz; i++)
	{
		if(phi[i] == INF) outimg1d[i] = 0;
		else if(phi[i] ==0) outimg1d[i] = 0;
		else
		{
			outimg1d[i] = (phi[i] - min_val)/max_val * 255 + 0.5;
			outimg1d[i] = MAX(outimg1d[i], 1);
		}
	}
     in_sz[3]=1;
    simple_saveimage_wrapper(callback,outimg_file, outimg1d, in_sz, 1);

	delete [] phi; phi = 0;
    delete [] inimg1d_ch1; inimg1d_ch1 = 0;
	delete [] outimg1d; outimg1d = 0;
	return true;
}


