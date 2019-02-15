/*
 *  ada_threshold.cpp
 *
 *  Created by Yang, Jinzhu and Hanchuan Peng, on 11/22/10.
 *  Add dofunc() interface by Jianlong Zhou, 2012-04-18.
 */

#include <iostream>
#include "ada_threshold.h"
#include "visiocyte_message.h"

#include "stackutil.h"

using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(threshold, ThPlugin);

template <class T>
void BinaryProcess(T *apsInput, T * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer, VISIOCYTELONG h, VISIOCYTELONG d)
{
	VISIOCYTELONG i, j,k,n,count;
	double t, temp;

	VISIOCYTELONG mCount = iImageHeight * iImageWidth;
	for (i=0; i<iImageLayer; i++)
	{
		for (j=0; j<iImageHeight; j++)
		{
			for (k=0; k<iImageWidth; k++)
			{
				VISIOCYTELONG curpos = i * mCount + j*iImageWidth + k;
				VISIOCYTELONG curpos1 = i* mCount + j*iImageWidth;
				VISIOCYTELONG curpos2 = j* iImageWidth + k;
				temp = 0;
				count = 0;
				for(n =1 ; n <= d  ;n++)
				{
					if (k>h*n) {temp += apsInput[curpos1 + k-(h*n)]; count++;}
					if (k+(h*n)< iImageWidth) { temp += apsInput[curpos1 + k+(h*n)]; count++;}
                    if (j>h*n) {temp += apsInput[i* mCount + (j-(h*n))*iImageWidth + k]; count++;}//
					if (j+(h*n)<iImageHeight) {temp += apsInput[i* mCount + (j+(h*n))*iImageWidth + k]; count++;}//
					if (i>(h*n)) {temp += apsInput[(i-(h*n))* mCount + curpos2]; count++;}//
					if (i+(h*n)< iImageLayer) {temp += apsInput[(i+(h*n))* mCount + j* iImageWidth + k ]; count++;}
				}
				t =  apsInput[curpos]-temp/(count);
				aspOutput[curpos]= (t > 0)? t : 0;
			}
		}
	}
}

void thimg(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);
bool thimg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

//plugin funcs
const QString title = "adaptive threshold transform";
QStringList ThPlugin::menulist() const
{
     return QStringList()
          << tr("3D (w/o parameters)")
          << tr("3D (set parameters)")
          << tr("Help");
}

void ThPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("3D (w/o parameters)"))
	{
          thimg(callback, parent,1 );
     }
	else if (menu_name == tr("3D (set parameters)"))
	{
		thimg(callback, parent, 2 );
	}
	else if (menu_name == tr("Help"))
	{
		visiocyte_msg("Simple adaptive thresholding: for each voxel, compute a threshold which is the average intensity of its neighboring voxels and then subtract the threshold from the current voxel's intensity value. If the result is <0, then set it as 0. The neighborhood is defined along 6 axial directions in 3D, with N samples of each direction (N -- the 'number of sampling points' in the parameter setting dialog), and M voxels between every nearest pair of samples (M -- the 'sampling interval' in the parameter setting dialog).");
		return;
	}

}

QStringList ThPlugin::funclist() const
{
	return QStringList()
		<<tr("adath")
		<<tr("help");
}


bool ThPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     if (func_name == tr("adath"))
	{
        return thimg(callback, input, output);
	}
	else if(func_name == tr("help"))
	{
		cout<<"Usage : visiocyte -x threshold -f adath -i <inimg_file> -o <outimg_file> -p <h> <d>"<<endl;
		cout<<endl;
		cout<<"h       sampling interval, default 5,"<<endl;
		cout<<"d       number of sampling points, default 3,"<<endl;
		cout<<endl;
		cout<<"e.g. visiocyte -x threshold -f adath -i input.raw -o output.raw -p 5 3"<<endl;
		cout<<endl;
		return true;
	}
}

bool thimg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	cout<<"Welcome to Gaussian filter"<<endl;
	if (input.size()<1 || output.size() != 1) return false;

	VISIOCYTELONG h = 5, d = 3;
     if (input.size()>=2)
     {
          vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
          if(paras.size() >= 1) h = atoi(paras.at(0));
          if(paras.size() >= 2) d = atoi(paras.at(1));
	}

	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
	cout<<"h = "<<h<<endl;
     cout<<"d = "<<d<<endl;
	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;

    Image4DSimple *subject = callback.loadImage(inimg_file);
    if(!subject || !subject->valid())
    {
         visiocyte_msg("Fail to load the input image.");
         if (subject) {delete subject; subject=0;}
         return false;
    }

     clock_t start_t = clock(); // record time point
     // =====================================================================>>>>>>>>>
     VISIOCYTELONG sz0 = subject->getXDim();
     VISIOCYTELONG sz1 = subject->getYDim();
     VISIOCYTELONG sz2 = subject->getZDim();
     VISIOCYTELONG sz3 = subject->getCDim();
    VISIOCYTELONG pagesz_sub = sz0*sz1*sz2;

	//----------------------------------------------------------------------------------------------------------------------------------
	VISIOCYTELONG channelsz = sz0*sz1*sz2;
	void *pData=NULL;

	VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
        switch (subject->getDatatype())
		{
            case VISIOCYTE_UINT8:
				try
				{
					pData = (void *)(new unsigned char [sz3*channelsz]);
				}
					catch (...)
				{
					visiocyte_msg("Fail to allocate memory in Distance Transform.",0);
					if (pData) {delete []pData; pData=0;}
					return false;
				}

				{
                    unsigned char * pSubtmp_uint8 = subject->getRawData();

					for (VISIOCYTELONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz, sz0, sz1, sz2, h, d  );
				}
				break;

            case VISIOCYTE_UINT16:
				try
				{
					pData = (void *)(new short int [sz3*channelsz]);
				}
					catch (...)
				{
					visiocyte_msg("Fail to allocate memory in Distance Transform.",0);
					if (pData) {delete []pData; pData=0;}
					return false;
				}

				{
                    short int * pSubtmp_uint16 = (short int *)subject->getRawData();

					for (VISIOCYTELONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint16+ich*channelsz, (short int *)pData+ich*channelsz, sz0, sz1, sz2, h, d );
				}

				break;

            case VISIOCYTE_FLOAT32:
				try
				{
					pData = (void *)(new float [sz3*channelsz]);
				}
					catch (...)
				{
					visiocyte_msg("Fail to allocate memory in Distance Transform.", 0);
					if (pData) {delete []pData; pData=0;}
					return false;
				}

				{
                    float * pSubtmp_float32 = (float *)subject->getRawData();

					for (VISIOCYTELONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2, h, d );
				}

				break;

			default:
				break;
		}

	//----------------------------------------------------------------------------------------------------------------------------------

	clock_t end_t = clock();
	printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);

     // =====================================================================<<<<<<<<<
    Image4DSimple outimg;
    outimg.setData((unsigned char *)pData, sz0, sz1, sz2, sz3, subject->getDatatype());

    callback.saveImage(&outimg, outimg_file);
    visiocyte_msg("Finish saving output file.",0);

     if(subject) {delete []subject; subject=0;}

     return true;
}





void thimg(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code)
{
	visiocytehandle curwin = callback.currentImageWindow();
	VISIOCYTELONG h;
	VISIOCYTELONG d;
	if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}

	if (method_code == 1)
	{
		h = 5;
		d = 3;
	}
	else
	{
		if( method_code == 2)
		{
			AdaTDialog dialog(callback, parent);
			if (dialog.exec()!=QDialog::Accepted)
			return;
			else
			{
				h = dialog.Ddistance->text().toLong()-1;
				d = dialog.Dnumber->text().toLong()-1;
				printf("d% h,d% d \n ",h,d);
			}
		}
	}

	clock_t start_t = clock(); // record time point

	Image4DSimple* subject = callback.getImage(curwin);
	QString m_InputFileName = callback.getImageName(curwin);

	if (!subject)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}
	Image4DProxy<Image4DSimple> pSub(subject);

	VISIOCYTELONG sz0 = subject->getXDim();
    VISIOCYTELONG sz1 = subject->getYDim();
    VISIOCYTELONG sz2 = subject->getZDim();
	VISIOCYTELONG sz3 = subject->getCDim();
	VISIOCYTELONG pagesz_sub = sz0*sz1*sz2;

	//----------------------------------------------------------------------------------------------------------------------------------
	VISIOCYTELONG channelsz = sz0*sz1*sz2;
	void *pData=NULL;

	VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
		switch (subject->getDatatype())
		{
			case VISIOCYTE_UINT8:
				try
				{
					pData = (void *)(new unsigned char [sz3*channelsz]);
				}
					catch (...)
				{
					visiocyte_msg("Fail to allocate memory in Distance Transform.");
					if (pData) {delete []pData; pData=0;}
					return;
				}

				{
					unsigned char * pSubtmp_uint8 = pSub.begin();

					for (VISIOCYTELONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz, sz0, sz1, sz2, h, d  );
				}
				break;

			case VISIOCYTE_UINT16:
				try
				{
					pData = (void *)(new short int [sz3*channelsz]);
				}
					catch (...)
				{
					visiocyte_msg("Fail to allocate memory in Distance Transform.");
					if (pData) {delete []pData; pData=0;}
					return;
				}

				{
					short int * pSubtmp_uint16 = (short int *)pSub.begin();

					for (VISIOCYTELONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint16+ich*channelsz, (short int *)pData+ich*channelsz, sz0, sz1, sz2, h, d );
				}

				break;

			case VISIOCYTE_FLOAT32:
				try
				{
					pData = (void *)(new float [sz3*channelsz]);
				}
					catch (...)
				{
					visiocyte_msg("Fail to allocate memory in Distance Transform.");
					if (pData) {delete []pData; pData=0;}
					return;
				}

				{
					float * pSubtmp_float32 = (float *)pSub.begin();

					for (VISIOCYTELONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2, h, d );
				}

				break;

			default:
				break;
		}

	//----------------------------------------------------------------------------------------------------------------------------------

	clock_t end_t = clock();
	printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);

	Image4DSimple p4DImage;
	p4DImage.setData((unsigned char*)pData, sz0, sz1, sz2, sz3, subject->getDatatype());

	visiocytehandle newwin;
	if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
		newwin = callback.currentImageWindow();
	else
		newwin = callback.newImageWindow();

	callback.setImage(newwin, &p4DImage);
	callback.setImageName(newwin, QString("thresholded image"));
	callback.updateImageWindow(newwin);
}

void AdaTDialog::update()
{
	//get current data
	Dn = Dnumber->text().toLong()-1;
	Dh = Ddistance->text().toLong()-1;
		//printf("channel %ld val %d x %ld y %ld z %ld ind %ld \n", c, data1d[ind], nx, ny, nz, ind);
}
