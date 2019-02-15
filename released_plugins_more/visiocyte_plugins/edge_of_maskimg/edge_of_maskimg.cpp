//by Hanchuan Peng
//2009-05-30

// Adapted and upgraded to add dofunc() by Jianlong Zhou, 2012-04-08


#include <QtGui>
#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "stackutil.h"
#include "visiocyte_message.h"
#include "edge_of_maskimg.h"



using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(edge_of_maskimg, EdgeOfMaskImgPlugin);

void findedgeimg(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);
bool findedgeimg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);


QStringList EdgeOfMaskImgPlugin::menulist() const
{
    return QStringList()
	<< tr("Label edge of a mask image using the original label values")
	<< tr("Label edge of a mask image using intensity 255")
	<< tr("about this plugin")
	;
}

void EdgeOfMaskImgPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("Label edge of a mask image using the original label values"))
		findedgeimg(callback, parent, 0);
	else if (menu_name == tr("Label edge of a mask image using intensity 255"))
		findedgeimg(callback, parent, 1);
	else
		visiocyte_msg("Label the edge of a mask image (label field image)'s edge using either the original label values or white (255). Developed by Hanchuan Peng, 2010.");
}




QStringList EdgeOfMaskImgPlugin::funclist() const
{
	return QStringList()
        <<tr("find_edge")
		<<tr("help");
}


bool EdgeOfMaskImgPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     if (func_name == tr("find_edge"))
	{
        return findedgeimg(callback, input, output);
	}
	else if(func_name == tr("help"))
	{
        cout<<"Usage : visiocyte -x masking -f find_edge -i <inimg_file> -o <outimg_file> -p <method_code>"<<endl;
		cout<<endl;
		cout<<"method_code  0: Label edge of a mask image using the original label values, 1: Label edge of a mask image using intensity 255"<<endl;
		cout<<endl;
        cout<<"e.g. visiocyte -x masking -f find_edge -i input.raw -o output.raw -p 0"<<endl;
		cout<<endl;
		return true;
	}
}

bool findedgeimg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	cout<<"Welcome to Label edge of a mask image"<<endl;
	if (output.size() != 1) return false;

	int method_code = 0;
     if (input.size()>=2)
     {
          vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
          if(paras.size() >= 1) method_code = atoi(paras.at(0));
	}

	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
	cout<<"method_code = "<<method_code<<endl;
	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;

     Image4DSimple *image = callback.loadImage(inimg_file);
     if (!image || !image->valid())
     {
          cerr<<"load image "<<inimg_file<<" error!"<<endl;
          return false;
     }

     VISIOCYTELONG szx=image->getXDim(),
             szy=image->getYDim(),
             szz=image->getZDim(),
             szc=image->getCDim();
     VISIOCYTELONG N = image->getTotalUnitNumber();

     //create the output buffer
	unsigned char *outputData = 0;
	try
	{
		outputData = new unsigned char [N];
		for (VISIOCYTELONG tmpi=0;tmpi<N;++tmpi) outputData[tmpi] = 0; //preset to be all 0
	}
	catch (...)
	{
		visiocyte_msg("Fail to allocate memory.");
		if (outputData) {delete []outputData; outputData=0;}
		return false;
	}

    Image4DSimple outputImage;
	outputImage.setData((unsigned char*)outputData, szx, szy, szz, szc, VISIOCYTE_UINT8);
	Image4DProxy<Image4DSimple> outputIProxy(&outputImage);

	//now do computation
    {
		bool bset255 = (method_code==0) ? false : true;

        Image4DProxy<Image4DSimple> p(image);
		Image4DProxy_foreach(p, ix, iy, iz, ic)
		{
			double v = p.value_at(ix, iy, iz, ic);
			VISIOCYTELONG cx, cy, cz;

			bool bb=false;
			for (cz = iz-1; cz<iz+2; ++cz)
			{
				for (cy = iy-1; cy<iy+2; ++cy)
				{
					for (cx = ix-1; cx<ix+2; ++cx)
					{
						if (!p.is_inner(cx, cy, cz, ic))
							continue;
						if (v!=p.value_at(cx, cy, cz, ic))
						{
							*outputIProxy.at(ix, iy, iz, ic) = (bset255) ? 255 : v;
							bb = true;
							break;
						}
					}
					if (bb)	break;
				}
				if (bb) break;
			}

			//note that all value had been preset as 0, thus no need to set as the background color in case not an edge point
		}
    }

     //how about save float??
    callback.saveImage(&outputImage, outimg_file);

    if (image) {delete image; image=0;}

     return true;
}






void findedgeimg(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code)
{
	visiocytehandle curwin = callback.currentImageWindow();
	if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}

	if (method_code!=0 && method_code!=1)
	{
		visiocyte_msg("Invalid internal method code. You should never see this message. Report this bug to the developer");
		return;
	}

	int start_t = clock(); // record time point

	Image4DSimple* image = callback.getImage(curwin);
	QString m_InputFileName = callback.getImageName(curwin);

    if (!image || !image->valid())
	{
		visiocyte_msg("The image pointer is invalid. Ensure your data is valid and try again!");
		return;
	}

	unsigned char* data1d = image->getRawData();

    VISIOCYTELONG N = image->getTotalUnitNumber();
	VISIOCYTELONG szx = image->getXDim(), szy = image->getYDim(), szz = image->getZDim(), szc = image->getCDim();

	//create the output buffer
	unsigned char *outputData = 0;
	try
	{
		outputData = new unsigned char [N];
		for (VISIOCYTELONG tmpi=0;tmpi<N;++tmpi) outputData[tmpi] = 0; //preset to be all 0
	}
	catch (...)
	{
		visiocyte_msg("Fail to allocate memory.");
		if (outputData) {delete []outputData; outputData=0;}
		return;
	}
	Image4DSimple outputImage;
	outputImage.setData((unsigned char*)outputData, szx, szy, szz, szc, VISIOCYTE_UINT8);
	Image4DProxy<Image4DSimple> outputIProxy(&outputImage);

	//now do computation
    {
		bool bset255 = (method_code==0) ? false : true;

		Image4DProxy<Image4DSimple> p(image);
		Image4DProxy_foreach(p, ix, iy, iz, ic)
		{
			double v = p.value_at(ix, iy, iz, ic);
			VISIOCYTELONG cx, cy, cz;

			bool bb=false;
			for (cz = iz-1; cz<iz+2; ++cz)
			{
				for (cy = iy-1; cy<iy+2; ++cy)
				{
					for (cx = ix-1; cx<ix+2; ++cx)
					{
						if (!p.is_inner(cx, cy, cz, ic))
							continue;
						if (v!=p.value_at(cx, cy, cz, ic))
						{
							*outputIProxy.at(ix, iy, iz, ic) = (bset255) ? 255 : v;
							bb = true;
							break;
						}
					}
					if (bb)	break;
				}
				if (bb) break;
			}

			//note that all value had been preset as 0, thus no need to set as the background color in case not an edge point
		}
    }


	int end_t = clock();
	printf("time eclapse %d s for edge detecting.\n", (end_t-start_t)/1000000);

	visiocytehandle newwin = callback.newImageWindow();
	callback.setImage(newwin, &outputImage);
	callback.setImageName(newwin, QString("edge map of the image"));
	callback.updateImageWindow(newwin);

	return;
}

