//by Hanchuan Peng
// 090814

// Adapted and upgraded to VISIOCYTEPluginInterface2_1 by Jianlong Zhou, 2012-04-05
// add dofunc() by Jianlong Zhou, 2012-04-16


#include <QtGui>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "rotateimg90.h"
#include "stackutil.h"

using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(rotateimg90, RotateImg90Plugin)


void processImage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, unsigned int rotateflag);
bool processImage(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, unsigned int rotateflag);

void rotateimage(unsigned char* data1d, VISIOCYTELONG *in_sz, ImagePixelType pixeltype, unsigned int rotateflag, unsigned char* &outimg);

const QString title = QObject::tr("Rotate image plugin");

QStringList RotateImg90Plugin::menulist() const
{
     return QStringList() << tr("Left 90 degrees in XY plane")
                          << tr("Right 90 degrees in XY plane")
                          << tr("180 degrees in XY plane")
                          << tr("about this plugin")
          ;
}


void RotateImg90Plugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     //choosing method
	unsigned int rotateflag = 0;

	if(menu_name == tr("Left 90 degrees in XY plane"))
     {
		rotateflag  = 1;
          processImage(callback,parent, rotateflag);
     }
	else if(menu_name == tr("Right 90 degrees in XY plane"))
	{
          rotateflag  = 2;
          processImage(callback,parent, rotateflag);
     }

	else if(menu_name == tr("180 degrees in XY plane"))
	{
          rotateflag  = 3;
          processImage(callback,parent, rotateflag);
     }
     else if (menu_name == tr("about this plugin"))
	{
		QMessageBox::information(parent, "Version info",
                QString("XY Plane Image 90-degree Rotation %1 (2009-Aug-14): this demo is developed by Hanchuan Peng to show VISIOCYTE plugin capability.")
                .arg(getPluginVersion()));
          return;
	}

}




QStringList RotateImg90Plugin::funclist() const
{
	return QStringList()
		<<tr("left90")
          <<tr("right90")
		<<tr("xy180")
          <<tr("help");
}


bool RotateImg90Plugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     if (func_name == tr("left90"))
	{
        return processImage(callback, input, output, 1);
	}
     else if (func_name == tr("right90"))
	{
        return processImage(callback, input, output, 2);
	}
     else if (func_name == tr("xy180"))
	{
        return processImage(callback, input, output, 3);
	}
	else if(func_name == tr("help"))
	{
		cout<<"Usage : visiocyte -x rotate -f <func> -i <inimg_file> -o <outimg_file> "<<endl;
          cout<<"func    rotate function type: 'left90' for rotate left 90 degrees in XY plane," << endl;
          cout<<"                              'right90' for rotate right 90 degrees in XY plane," << endl;
          cout<<"                              'xy180' for rotate 180 degrees in XY plane" << endl;
          cout<<endl;
          cout<<"e.g. visiocyte -x rotate -f left90 -i input.raw -o output.raw"<<endl;
		cout<<endl;

		return true;
	}
}


bool processImage(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, unsigned int rotateflag)
{
     cout<<"Welcome to Rotate image"<<endl;
	if (input.size() != 1 || output.size() != 1) return false;

	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);

     cout<<"rotateflag = "<<rotateflag<<endl;

	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;

	unsigned char * data1d = 0;
    VISIOCYTELONG in_sz[4];

	int datatype;
    if(!simple_loadimage_wrapper(callback, inimg_file, data1d, in_sz, datatype))
     {
          cerr<<"load image "<<inimg_file<<" error!"<<endl;
          return false;
     }

     unsigned char* outimg = 0;
     VISIOCYTELONG szx=in_sz[0], szy=in_sz[1], szz=in_sz[2], szc=in_sz[3];

     switch (datatype)
     {
          case 1: rotateimage(data1d, in_sz, VISIOCYTE_UINT8, rotateflag, outimg); break;
          case 2: rotateimage(data1d, in_sz, VISIOCYTE_UINT16, rotateflag, outimg); break;
          case 4: rotateimage(data1d, in_sz, VISIOCYTE_FLOAT32, rotateflag, outimg); break;
          default:
               visiocyte_msg("Invalid datatype.");
               if (data1d) {delete []data1d; data1d=0;}
               return false;
     }

     // save image
     switch(rotateflag)
     {
          case 1:
          case 2:
               in_sz[0]=szy; in_sz[1]=szx;
               simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)outimg, in_sz, datatype);
               break;

          case 3:
               in_sz[0]=szx; in_sz[1]=szy;
               simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)outimg, in_sz, datatype);
               break;
          default:
               break;
     }

     if (outimg) {delete []outimg; outimg =0;}
     if (data1d) {delete []data1d; data1d=0;}

     return true;
}





void processImage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, unsigned int rotateflag)
{
    visiocytehandle curwin = callback.currentImageWindow();
	if (!curwin)
	{
        QMessageBox::information(0, "", "You don't have any image open in the main window.");
		return;
	}

    Image4DSimple* image = callback.getImage(curwin);

	if (!image)
	{
		QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
		return;
	}

	unsigned char* data1d = image->getRawData();


	VISIOCYTELONG szx = image->getXDim(), szy = image->getYDim(), szz = image->getZDim(), szc = image->getCDim();
	// VISIOCYTELONG szchan = image->getTotalUnitNumberPerChannel(), szplane = image->getTotalUnitNumberPerPlane();
	// VISIOCYTELONG N = image->getTotalBytes();
	// VISIOCYTELONG i,j,k,c;

	if (!data1d || szx<=0 || szy<=0 || szz<=0 || szc<=0)
	{
		throw("Your data to the plugin is invalid. Check the program.");
		return;
	}

     ImagePixelType pixeltype = image->getDatatype();

     VISIOCYTELONG in_sz[4];
     in_sz[0]=szx; in_sz[1]=szy;  in_sz[2]=szz;  in_sz[3]=szc;

     unsigned char* outimg=0;
     rotateimage(data1d, in_sz, pixeltype, rotateflag, outimg);

     // image->setData(outimg, szy, szx, szz, szc, image->getDatatype());

     // display
     Image4DSimple * new4DImage = new Image4DSimple();
     switch(rotateflag)
     {
          case 1:
               new4DImage->setData((unsigned char*)outimg, szy, szx, szz, szc, image->getDatatype());
               break;
          case 2:
               new4DImage->setData((unsigned char*)outimg, szy, szx, szz, szc, image->getDatatype());
               break;
          case 3:
               new4DImage->setData((unsigned char*)outimg, szx, szy, szz, szc, image->getDatatype());
               break;
          default:
               break;
     }

     visiocytehandle newwin = callback.newImageWindow();
     callback.setImage(newwin, new4DImage);
     callback.setImageName(newwin, title);
     callback.updateImageWindow(newwin);

}


void rotateimage(unsigned char* data1d, VISIOCYTELONG *in_sz, ImagePixelType pixeltype, unsigned int rotateflag, unsigned char* &outimg)
{
     if (!data1d || in_sz[0]<=0 || in_sz[1]<=0 || in_sz[2]<=0 || in_sz[3]<=0 || outimg)
	{
		throw("Your data to the plugin is invalid. Check the program.");
		return;
	}

	VISIOCYTELONG szx = in_sz[0], szy = in_sz[1], szz = in_sz[2], szc = in_sz[3];
	VISIOCYTELONG szchan = szx*szy*szz, szplane = szx*szy;
     VISIOCYTELONG N, un;
     switch(pixeltype)
     {
          case VISIOCYTE_UINT8:
               un=1;
               break;
          case VISIOCYTE_UINT16:
               un=2;
               break;
          case VISIOCYTE_FLOAT32:
               un=4;
               break;
          default:
               visiocyte_msg("Invalid data type.", 0);
               return;
     }

     N = un*szx*szy*szz*szc;

     VISIOCYTELONG i,j,k,c;

     if (rotateflag == 1)
    {
		unsigned char *nm = new unsigned char [N];
		switch (pixeltype)
		{
			case VISIOCYTE_UINT8:
				{
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
						{
							VISIOCYTELONG offset = c*szchan + k*szplane;
							for (j=0;j<szy;j++)
								for (i=0;i<szx;i++)
									nm[offset + (szx-1-i)*szy + (j)] = data1d[offset + j*szx + i];
						}
				}
				break;
			case VISIOCYTE_UINT16:
				{
					unsigned short int * nm_uint16 = (unsigned short int *)nm;
					unsigned short int * old_uint16 = (unsigned short int *)data1d;
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
						{
							VISIOCYTELONG offset = c*szchan + k*szplane;
							for (j=0;j<szy;j++)
								for (i=0;i<szx;i++)
									nm_uint16[offset + (szx-1-i)*szy + (j)] = old_uint16[offset + j*szx + i];
						}
				}
				break;
			case VISIOCYTE_FLOAT32:
				{
					float * nm_float32 = (float *)nm;
					float * old_float32 = (float *)data1d;
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
						{
							VISIOCYTELONG offset = c*szchan + k*szplane;
							for (j=0;j<szy;j++)
								for (i=0;i<szx;i++)
									nm_float32[offset + (szx-1-i)*szy + (j)] = old_float32[offset + j*szx + i];
						}
				}
				break;
			default:
				return;
		}
          outimg = nm;
		//image->setData(nm, szy, szx, szz, szc, image->getDatatype()); //setData() will free the original memory automatically
    }
    else if (rotateflag == 2) // "Right 90 degrees in XY plane"
	{
		unsigned char *nm = new unsigned char [N];
		switch (pixeltype)
		{
			case VISIOCYTE_UINT8:
				{
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
						{
							VISIOCYTELONG offset = c*szchan + k*szplane;
							for (j=0;j<szy;j++)
								for (i=0;i<szx;i++)
									nm[offset + (i)*szy + (szy-1-j)] = data1d[offset + j*szx + i];
						}
				}
				break;
			case VISIOCYTE_UINT16:
				{
					unsigned short int * nm_uint16 = (unsigned short int *)nm;
					unsigned short int * old_uint16 = (unsigned short int *)data1d;
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
						{
							VISIOCYTELONG offset = c*szchan + k*szplane;
							for (j=0;j<szy;j++)
								for (i=0;i<szx;i++)
									nm_uint16[offset + (i)*szy + (szy-1-j)] = old_uint16[offset + j*szx + i];
						}
				}
				break;
			case VISIOCYTE_FLOAT32:
				{
					float * nm_float32 = (float *)nm;
					float * old_float32 = (float *)data1d;
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
						{
							VISIOCYTELONG offset = c*szchan + k*szplane;
							for (j=0;j<szy;j++)
								for (i=0;i<szx;i++)
									nm_float32[offset + (i)*szy + (szy-1-j)] = old_float32[offset + j*szx + i];
						}
				}
				break;
			default:
				return;
		}
          outimg = nm;
		//image->setData(nm, szy, szx, szz, szc, image->getDatatype()); //setData() will free the original memory automatically
	}
    else if (rotateflag == 3) //"180 degrees in XY plane"
    {
         unsigned char *nm = new unsigned char [N];
		switch (pixeltype)
		{
			case VISIOCYTE_UINT8:
				{
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
							for (j=0;j<szy/2;j++)
								for (i=0;i<szx;i++)
								{
									// unsigned char tmp = data1d[c*szchan + k*szplane + j*szx + i];
									// data1d[c*szchan + k*szplane + j*szx + i] = data1d[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)];
									// data1d[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)] = tmp;

                                             unsigned char tmp = data1d[c*szchan + k*szplane + j*szx + i];
									nm[c*szchan + k*szplane + j*szx + i] = data1d[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)];
									nm[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)] = tmp;
								}
				}
				break;
			case VISIOCYTE_UINT16:
				{
                         unsigned short int * nm_uint16 = (unsigned short int *)nm;
					unsigned short int * old_uint16 = (unsigned short int *)data1d;
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
							for (j=0;j<szy/2;j++)
								for (i=0;i<szx;i++)
								{
									// unsigned short int tmp = old_uint16[c*szchan + k*szplane + j*szx + i];
									// old_uint16[c*szchan + k*szplane + j*szx + i] = old_uint16[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)];
									// old_uint16[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)] = tmp;

                                             unsigned short int tmp = old_uint16[c*szchan + k*szplane + j*szx + i];
									nm_uint16[c*szchan + k*szplane + j*szx + i] = old_uint16[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)];
									nm_uint16[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)] = tmp;
								}
				}
				break;
			case VISIOCYTE_FLOAT32:
				{
                         float * nm_float32 = (float *)nm;
					float * old_float32 = (float *)data1d;
					for (c=0;c<szc;c++)
						for (k=0;k<szz;k++)
							for (j=0;j<szy/2;j++)
								for (i=0;i<szx;i++)
								{
									// float tmp = old_float32[c*szchan + k*szplane + j*szx + i];
									// old_float32[c*szchan + k*szplane + j*szx + i] = old_float32[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)];
									// old_float32[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)] = tmp;

                                             float tmp = old_float32[c*szchan + k*szplane + j*szx + i];
									nm_float32[c*szchan + k*szplane + j*szx + i] = old_float32[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)];
									nm_float32[c*szchan + k*szplane + (szy-1-j)*szx + (szx-1-i)] = tmp;
								}
				}
				break;
			default:
				return;
		}
          outimg = nm;
    }
	else
		return;

}
