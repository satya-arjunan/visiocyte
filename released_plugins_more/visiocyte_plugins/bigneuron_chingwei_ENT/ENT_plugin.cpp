/* ENT_plugin.cpp
 * The plugin is called ensemble neuron tracer.
 * Developers: Ching-Wei Wang, Hilmil Pradana, Cheng-Ta Huang 
 * Institution: National Taiwan University of Science and Technology
 * Website: http://www-o.ntust.edu.tw/~cweiwang/
 * Email: cweiwang@mail.ntust.edu.tw
 * Release date: 2016-1-8
 */
 
#include "visiocyte_message.h"
#include <vector>
#include <math.h>
#include "basic_surf_objs.h"

#include "ENT_plugin.h"

#include "vn_imgpreprocess.h"
#include "fastmarching_dt.h"
#include "fastmarching_tree.h"
#include "hierarchy_prune.h"
#include "marker_radius.h"



Q_EXPORT_PLUGIN2(APP2_ported, APP2_ported);

using namespace std;

struct input_PARA
{
    QString inimg_file;
    VISIOCYTELONG channel;
};

struct saveData
{
	int x;
	int y;
	int z;
	int parent;
	int child;
	int blockX;
	int blockY;
};

void reconstruction_func(VISIOCYTEPluginCallback2 &callback, QWidget *parent, input_PARA &PARA, bool bmenu);
const vector<MyMarker*> APP2(unsigned char* data1d, VISIOCYTELONG N, VISIOCYTELONG M, VISIOCYTELONG P, VISIOCYTELONG sc);
unsigned char*** Convert1D3D (unsigned char* inimg1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z);
unsigned char* Convert3D1D (unsigned char*** inimg1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z);
unsigned char*** Erotion (unsigned char*** data1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z);
unsigned char*** Dilation (unsigned char*** data1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z);
vector<MyMarker*> Combining(vector<MyMarker*> main, vector<MyMarker*> adding);
unsigned char *BVersion(unsigned char *inimg1d, VISIOCYTELONG size);
unsigned char* NVersion (unsigned char* inimg1d, VISIOCYTELONG size);
unsigned char* SVersion (unsigned char* inimg1d, VISIOCYTELONG size);
double Learning(unsigned char* img1D, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z, VISIOCYTELONG blockX, VISIOCYTELONG blockY);
void Partition(unsigned char* data1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z, int &numLocationBlockX, int &numLocationBlockY, int &blockX, int &blockY);
unsigned char* cubeLocation(unsigned char* inimg1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z, int numLocationBlockX, int numLocationBlockY);
vector<MyMarker*> BaseTracer(unsigned char* img1D, VISIOCYTELONG X, VISIOCYTELONG Y, VISIOCYTELONG Z, VISIOCYTELONG sc);

unsigned char *data1d_1ch;
Image4DSimple *p4dImageNew = 0;
 
QStringList APP2_ported::menulist() const
{
	return QStringList() 
		<<tr("tracing_menu")
		<<tr("about");
}

QStringList APP2_ported::funclist() const
{
	return QStringList()
		<<tr("tracing_func")
		<<tr("help");
}

void APP2_ported::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("tracing_menu"))
	{
        bool bmenu = true;
        input_PARA PARA;
        reconstruction_func(callback,parent,PARA,bmenu);

	}
	else
	{
		visiocyte_msg(tr("The plugin is called ensemble neuron tracer."
			"Developed by Ching-Wei Wang(cweiwang@mail.ntust.edu.tw) email: cweiwang@mail.ntust.edu.tw, Hilmil Pradana, Cheng-Ta Huang, National Taiwan University of Science and Technology, 2016-1-8"));
	}
}

bool APP2_ported::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("tracing_func"))
	{
        bool bmenu = false;
        input_PARA PARA;

        vector<char*> * pinfiles = (input.size() >= 1) ? (vector<char*> *) input[0].p : 0;
        vector<char*> * pparas = (input.size() >= 2) ? (vector<char*> *) input[1].p : 0;
        vector<char*> infiles = (pinfiles != 0) ? * pinfiles : vector<char*>();
        vector<char*> paras = (pparas != 0) ? * pparas : vector<char*>();

        if(infiles.empty())
        {
            fprintf (stderr, "Need input image. \n");
            return false;
        }
        else
            PARA.inimg_file = infiles[0];
        int k=0;
        PARA.channel = (paras.size() >= k+1) ? atoi(paras[k]) : 1;  k++;
        reconstruction_func(callback,parent,PARA,bmenu);
	}
    else if (func_name == tr("help"))
    {
        ////HERE IS WHERE THE DEVELOPERS SHOULD UPDATE THE USAGE OF THE PLUGIN
		printf("**** Usage of APP2_ported tracing **** \n");
		printf("visiocyte -x APP2_ported -f tracing_func -i <inimg_file> -p <channel> <other parameters>\n");
        printf("inimg_file       The input image\n");
        printf("channel          Data channel for tracing. Start from 1 (default 1).\n");

        printf("outswc_file      Will be named automatically based on the input image file name, so you don't have to specify it.\n\n");

	}
	else return false;

	return true;
}

void reconstruction_func(VISIOCYTEPluginCallback2 &callback, QWidget *parent, input_PARA &PARA, bool bmenu)
{
    unsigned char* data1d = 0;
    VISIOCYTELONG N,M,P,sc,c;
    VISIOCYTELONG in_sz[4];
    if(bmenu)
    {
        visiocytehandle curwin = callback.currentImageWindow();
        if (!curwin)
        {
            QMessageBox::information(0, "", "You don't have any image open in the main window.");
            return;
        }

        Image4DSimple* p4DImage = callback.getImage(curwin);

        if (!p4DImage)
        {
            QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
            return;
        }

        if(p4DImage->getDatatype()!=VISIOCYTE_UINT8)
        {
            QMessageBox::information(0, "", "Please convert the image to be UINT8 and try again!");
            return;
        }
        data1d = p4DImage->getRawData();
        N = p4DImage->getXDim();
        M = p4DImage->getYDim();
        P = p4DImage->getZDim();
        sc = p4DImage->getCDim();

        bool ok1;

        if(sc==1)
        {
            c=1;
            ok1=true;
        }
        else
        {
            c = QInputDialog::getInteger(parent, "Channel",
                                             "Enter channel NO:",
                                             1, 1, sc, 1, &ok1);
        }

        if(!ok1)
            return;

        in_sz[0] = N;
        in_sz[1] = M;
        in_sz[2] = P;
        in_sz[3] = sc;


        PARA.inimg_file = p4DImage->getFileName();
    }
    else
    {
        int datatype = 0;
        if (!simple_loadimage_wrapper(callback,PARA.inimg_file.toStdString().c_str(), data1d, in_sz, datatype))
        {
            fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",PARA.inimg_file.toStdString().c_str());
            return;
        }
        if(PARA.channel < 1 || PARA.channel > in_sz[3])
        {
            fprintf (stderr, "Invalid channel number. \n");
            return;
        }

        if(datatype !=1)
        {
            fprintf (stderr, "Please convert the image to be UINT8 and try again!\n");
            return;
        }
        N = in_sz[0];
        M = in_sz[1];
        P = in_sz[2];
        sc = in_sz[3];
        c = PARA.channel;
    }

    //main neuron reconstruction code

    //// THIS IS WHERE THE DEVELOPERS SHOULD ADD THEIR OWN NEURON TRACING CODE
	/*****************************************************************************************/
	/** inisialization **/
//	vector<MyMarker*> Tracer1, Tracer2, Tracer3, Result;
	unsigned char* img1D1 = new unsigned char[(in_sz[0]*in_sz[1]*in_sz[2])];
	unsigned char* img1D2 = new unsigned char[(in_sz[0]*in_sz[1]*in_sz[2])];
    unsigned char* img1D3 = new unsigned char[(in_sz[0]*in_sz[1]*in_sz[2])];
	
/*	for (VISIOCYTELONG i=0;i<(in_sz[0]*in_sz[1]*in_sz[2]);i++)
	{
		img1D1[i] = data1d[i];
		img1D2[i] = data1d[i];
		img1D3[i] = data1d[i];
	}*/

    memcpy(img1D1, data1d, (in_sz[0]*in_sz[1]*in_sz[2]));
	memcpy(img1D2, data1d, (in_sz[0]*in_sz[1]*in_sz[2]));
    memcpy(img1D3, data1d, (in_sz[0]*in_sz[1]*in_sz[2]));

/*	unsigned char*** img3D;
	img3D = new unsigned char**[in_sz[2]];
	for(int i = 0; i < in_sz[2]; ++i)
	{
		img3D[i] = new unsigned char*[in_sz[1]];
		for(int j = 0; j < in_sz[1]; ++j) img3D[i][j] = new unsigned char[in_sz[0]];
	}*/
	double val[3], maximumD, idx;
    int topDown, leftRight, blockX, blockY, maximumI;

	/** end of inisialization **/
	/*****************************************************************************************/
	
	
	/*****************************************************************************************/
	/** function **/
/*	img1D = Contrast(data1d, in_sz[0]*in_sz[1]*in_sz[2], PARA, 50);
	
	Image4DSimple* p4DImageNew1 = 0;
	p4DImageNew1 = new Image4DSimple;
	visiocytehandle newwin1 = callback.newImageWindow();
	p4DImageNew1->setData(img1D, in_sz[0], in_sz[1], in_sz[2], 1, VISIOCYTE_UINT8);
	callback.setImage(newwin1, p4DImageNew1);
    callback.setImageName(newwin1, QObject::tr("Image Partition"));*/

	/*for (VISIOCYTELONG i=0;i<(in_sz[0]*in_sz[1]*in_sz[2]);i++)
	{
		if (data1d[i]>0) img1D[i] = 255;
		else img1D[i] = 0;
    }*/
    Partition(data1d, in_sz[0], in_sz[1], in_sz[2], topDown, leftRight, blockX, blockY);

    img1D1 = NVersion(img1D1, in_sz[0]*in_sz[1]*in_sz[2]);
	val[0] = Learning(img1D1, in_sz[0], in_sz[1], in_sz[2], leftRight, topDown);
	
	img1D2 = SVersion(img1D2, in_sz[0]*in_sz[1]*in_sz[2]);
	val[1] = Learning(img1D2, in_sz[0], in_sz[1], in_sz[2], leftRight, topDown);
	
	img1D3 = BVersion(img1D3, in_sz[0]*in_sz[1]*in_sz[2]);
	val[2] = Learning(img1D3, in_sz[0], in_sz[1], in_sz[2], leftRight, topDown);
	
	if (val[0] == 0 && val[1] == 0 && val[2] == 0)
	{
		// very clean
		vector<MyMarker*> Result[3];
		
		bool maxNode[3];
		maxNode[0] = true;
		maxNode[1] = true;
		maxNode[2] = true;
		int resultSize[3];
		
		Result[0] = BaseTracer(img1D1, in_sz[0], in_sz[1], in_sz[2], sc);
		Result[1] = BaseTracer(img1D2, in_sz[0], in_sz[1], in_sz[2], sc);
		Result[2] = BaseTracer(img1D3, in_sz[0], in_sz[1], in_sz[2], sc);
		
		resultSize[0] = Result[0].size();
		resultSize[1] = Result[1].size();
		resultSize[2] = Result[2].size();
		
		if (resultSize[0]>11000) resultSize[0] = 0;
		if (resultSize[1]>11000) resultSize[1] = 0;
		if (resultSize[2]>11000) resultSize[2] = 0;
		
		maximumI = max(resultSize[0], resultSize[1]);
		maximumI = max(maximumI, resultSize[2]);
		
		for (int i=0;i<3;i++)
		{
			if (maximumI == Result[i].size())
			{
                QString swc_name = PARA.inimg_file + "_ENT.swc";
				saveSWC_file(swc_name.toStdString(), Result[i]);
			}
		}
	}
	else
	{
		// noise
		vector<MyMarker*> Result1, Result2;
		
		maximumD = max(val[0], val[1]);
		maximumD = max(maximumD, val[2]);
		for (int i=0;i<3;i++)
		{
			if (val[i]==maximumD) idx = i;
		}
		if (idx == 0)
		{
			Result1 = BaseTracer(img1D2, in_sz[0], in_sz[1], in_sz[2], sc);
			Result2 = BaseTracer(img1D3, in_sz[0], in_sz[1], in_sz[2], sc);
		}
		else if (idx == 1)
		{
			Result1 = BaseTracer(img1D1, in_sz[0], in_sz[1], in_sz[2], sc);
			Result2 = BaseTracer(img1D3, in_sz[0], in_sz[1], in_sz[2], sc);
		}
		else
		{
			Result1 = BaseTracer(img1D1, in_sz[0], in_sz[1], in_sz[2], sc);
			Result2 = BaseTracer(img1D2, in_sz[0], in_sz[1], in_sz[2], sc);
		}
		
		if (Result1.size()>11000)
		{
            QString swc_name = PARA.inimg_file + "_ENT.swc";
			saveSWC_file(swc_name.toStdString(), Result2);
		}
		else if (Result2.size()>11000)
		{
            QString swc_name = PARA.inimg_file + "_ENT.swc";
			saveSWC_file(swc_name.toStdString(), Result1);
		}
		else
		{
			maximumI = max(Result1.size(), Result2.size());
			if (maximumI == Result1.size())
			{
                QString swc_name = PARA.inimg_file + "_ENT.swc";
				saveSWC_file(swc_name.toStdString(), Result1);
			}
			else
			{
                QString swc_name = PARA.inimg_file + "_ENT.swc";
				saveSWC_file(swc_name.toStdString(), Result2);
			}
		}
	}
	
/*	QString swc_name1 = PARA.inimg_file + "NVersionNew.swc";
	saveSWC_file(swc_name1.toStdString(), Result1);
	QString swc_name2 = PARA.inimg_file + "SVersionNew.swc";
	saveSWC_file(swc_name2.toStdString(), Result2);
	QString swc_name3 = PARA.inimg_file + "BVersionNew.swc";
	saveSWC_file(swc_name3.toStdString(), Result3);

	cout << value1 << " " << value2 << " " << value3 << endl << endl;*/
	
	/** end of function **/
	/*****************************************************************************************/

	/*****************************************************************************************/
/*	Image4DSimple* p4DImageNew1 = 0;
	p4DImageNew1 = new Image4DSimple;
	visiocytehandle newwin1 = callback.newImageWindow();
	p4DImageNew1->setData(img1D1, in_sz[0], in_sz[1], in_sz[2], 1, VISIOCYTE_UINT8);
	callback.setImage(newwin1, p4DImageNew1);
	callback.setImageName(newwin1, QObject::tr("NVersionNew"));

	Image4DSimple* p4DImageNew2 = 0;
	p4DImageNew2 = new Image4DSimple;
	visiocytehandle newwin2 = callback.newImageWindow();
	p4DImageNew2->setData(img1D2, in_sz[0], in_sz[1], in_sz[2], 1, VISIOCYTE_UINT8);
	callback.setImage(newwin2, p4DImageNew2);
	callback.setImageName(newwin2, QObject::tr("SVersionNew"));
	
	Image4DSimple* p4DImageNew3 = 0;
	p4DImageNew3 = new Image4DSimple;
	visiocytehandle newwin3 = callback.newImageWindow();
	p4DImageNew3->setData(img1D3, in_sz[0], in_sz[1], in_sz[2], 1, VISIOCYTE_UINT8);
	callback.setImage(newwin3, p4DImageNew3);
	callback.setImageName(newwin3, QObject::tr("BVersionNew"));*/
	/*****************************************************************************************/
    return;
}

vector<MyMarker*> BaseTracer(unsigned char* img1D, VISIOCYTELONG X, VISIOCYTELONG Y, VISIOCYTELONG Z, VISIOCYTELONG sc)
{
	/*****************************************************************************************/
	/** inisialization **/
	VISIOCYTELONG in_sz[3];
	in_sz[0] = X;
	in_sz[1] = Y;
	in_sz[2] = Z;
	
	vector<MyMarker*> Tracer1, Tracer2, Tracer3, Result;
	unsigned char*** img3D;
	img3D = new unsigned char**[in_sz[2]];
	for(int i = 0; i < in_sz[2]; ++i)
	{
		img3D[i] = new unsigned char*[in_sz[1]];
		for(int j = 0; j < in_sz[1]; ++j) img3D[i][j] = new unsigned char[in_sz[0]];
	}
	/** end of inisialization **/
	/*****************************************************************************************/
	
	Tracer1 = APP2(img1D, in_sz[0], in_sz[1], in_sz[2], sc);
	img3D = Convert1D3D (img1D, in_sz[0], in_sz[1], in_sz[2]);
	delete[] img1D;
	
	img3D = Erotion (img3D, in_sz[0], in_sz[1], in_sz[2]);
	img1D = Convert3D1D (img3D, in_sz[0], in_sz[1], in_sz[2]);
	Tracer2 = APP2(img1D, in_sz[0], in_sz[1], in_sz[2], sc);
	delete[] img1D;

	img3D = Dilation (img3D, in_sz[0], in_sz[1], in_sz[2]);
	img1D = Convert3D1D (img3D, in_sz[0], in_sz[1], in_sz[2]);
	Tracer3 = APP2(img1D, in_sz[0], in_sz[1], in_sz[2], sc);
	delete[] img1D;
	
	Result = Combining(Tracer2, Tracer1);
	Result = Combining(Result, Tracer3);
	
	Tracer1.clear();
	Tracer2.clear();
	Tracer3.clear();
	
	cout << endl << endl << "End of Tracer." << endl << endl;
	
	return Result;
}

unsigned char* cubeLocation(unsigned char* inimg1d, VISIOCYTELONG X, VISIOCYTELONG Y, VISIOCYTELONG Z, int numLocationBlockX, int numLocationBlockY)
{
	/*********************************************************************************/
	/****************************/
	VISIOCYTELONG sz[3];
	sz[0] = ((int)(X/8)) * 8;
	sz[1] = ((int)(Y/8)) * 8;
	sz[2] = Z;
	
	unsigned char* imgPartition;
	VISIOCYTELONG sizeImage = (sz[0]*sz[1]*sz[2])/64;

	imgPartition = new unsigned char[sizeImage];
	/****************************/
	int countImgPartion = 0;
	int count = 0;
	int xx = sz[0]/8;
	int yy = sz[1]/8;
	/*********************************************************************************/
	
	// separate 1D data to 64 seperate 1D data
	/****************************/
	for (VISIOCYTELONG k=0;k<sz[2];k++)
	{
		for (VISIOCYTELONG i=0;i<sz[0];i++)
		{
			for (VISIOCYTELONG j=0;j<sz[1];j++)
			{
				if (i>=(numLocationBlockX*xx) && i<((numLocationBlockX+1)*xx))
				{
					if (j>=(numLocationBlockY*yy) && j<((numLocationBlockY+1)*yy))
					{
						imgPartition[countImgPartion] = inimg1d[count];
						countImgPartion = countImgPartion + 1;
						
					}
				}
				count++;
			}
		}
	}
	return imgPartition;
}

double Learning(unsigned char* img1D, VISIOCYTELONG X, VISIOCYTELONG Y, VISIOCYTELONG Z, VISIOCYTELONG blockX, VISIOCYTELONG blockY)
{
	VISIOCYTELONG sz[3];
	sz[0] = ((int)(X/8)) * 8;
	sz[1] = ((int)(Y/8)) * 8;
	sz[2] = Z;
	
	unsigned char* imgPartition;
	VISIOCYTELONG sizeImage = (sz[0]*sz[1]*sz[2])/64;
	imgPartition = new unsigned char[sizeImage];
	imgPartition = cubeLocation(img1D, X, Y, Z, blockX, blockY);
	
	VISIOCYTELONG numOfPixel = 0;
	double probability;
	for (VISIOCYTELONG i=0;i<sizeImage;i++)
	{
		if (imgPartition[i]!=0) numOfPixel++;
	}
	delete[] imgPartition;
	probability = ((double)numOfPixel)/((double)sizeImage);
	return probability;
}

void Partition(unsigned char* data1d, VISIOCYTELONG X, VISIOCYTELONG Y, VISIOCYTELONG Z, int &numLocationBlockX, int &numLocationBlockY, int &blockX, int &blockY)
{
	/*********************************************************************************/
	/****************************/
	VISIOCYTELONG sz[3];
	sz[0] = ((int)(X/8)) * 8;
	sz[1] = ((int)(Y/8)) * 8;
	sz[2] = Z;

    /****************************/
	unsigned char*** img3D;
	img3D = new unsigned char**[sz[2]];
    for(int x = 0; x < sz[2]; ++x)
    {
        img3D[x] = new unsigned char*[sz[1]];
        for(int y = 0; y < sz[1]; ++y) img3D[x][y] = new unsigned char[sz[0]];
    }
	/****************************/
	
	/****************************/
	unsigned char ** img2D = new unsigned char*[sz[1]];
	for(int i = 0; i < sz[1]; ++i)
    img2D[i] = new unsigned char[sz[0]];
	/****************************/
	
	/****************************/
    unsigned char* imgPartition;
    VISIOCYTELONG sizeImage = (sz[0]*sz[1]*sz[2])/64;
    imgPartition = new unsigned char[sizeImage];
	/****************************/

	unsigned char maxRow;
	unsigned char maxValue = 0;
	int maximumRow;
	int maximumBlock = 0;
	int count = 0, countPartition = 0;
    int numOfPixel = 0;
	/*********************************************************************************/

	// convert 1D data to 3D data
    /****************************/
    for (int k=0;k<sz[2];k++)
    {
        for (int j=0;j<sz[1];j++)
        {
            for (int i=0;i<sz[0];i++)
            {
                img3D[k][j][i] = data1d[i+(j*X)+(k*X*Y)];
            }
        }
    }
	/****************************/
	
	// convert 3D data to 2D data
	/****************************/
    for (int i=0;i<sz[1];i++)
    {
        for (int j=0;j<sz[0];j++)
        {
           //img2D[i][j] = img3D[0][i][j];
            for (int k=1;k<sz[2];k++)
            {
                img2D[i][j] = max(img3D[0][i][j], img3D[k][i][j]);
            }
        }
    }
	
    delete[] img3D;
	/****************************/
	
	// maximum intensity in image 2D
	/****************************/
	
	for (int i=0;i<sz[1];i++)
	{
		maxRow = img2D[i][0];
		for (int j=1;j<sz[0];j++)
		{
			maxRow = max(maxRow, img2D[i][j]);
		}
		maxValue = max(maxValue, maxRow);
	}
	/****************************/
	
	// search block which has maxValue
	/****************************/
	for (int i=0;i<sz[1];i++)
	{
		for (int j=1;j<sz[0];j++)
		{
			if (img2D[i][j]<maxValue) img2D[i][j] = 0;
			else img2D[i][j] = 1;
		}
    }
	
	int blockCount[8][8];
	int coordinateX, coordinateY;
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		{
			blockCount[i][j] = 0;
		}
	}
	
	for (int i=0;i<sz[1];i++)
	{
		for (int j=0;j<sz[0];j++)
		{
			if (img2D[i][j]==1)
			{
				coordinateX = (i*8)/sz[0];
				coordinateY = (j*8)/sz[1];
				blockCount[coordinateX][coordinateY] = blockCount[coordinateX][coordinateY]+1;
			}
		}
	}

	for (int i=0;i<8;i++)
	{
		maximumRow = blockCount[i][0];
		for (int j=1;j<8;j++)
		{
			maximumRow = max(maximumRow, blockCount[i][j]);
		}
		maximumBlock = max(maximumBlock, maximumRow);
	}
	
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		{
			if (blockCount[i][j] == maximumBlock)
			{
				numLocationBlockX = i;
				numLocationBlockY = j;
			}
		}
	}
	
	if (numLocationBlockX>=0 && numLocationBlockX<=3)
	{
		if (numLocationBlockY >=0 && numLocationBlockY <=3)
		{
			// pojok kanan bawah
			blockX = 7;
			blockY = 7;
		}
		else
		{
			// pojok kiri bawah
			blockX = 7;
			blockY = 0;
		}
	}
	else
	{
		if (numLocationBlockY >=0 && numLocationBlockY <=3)
		{
			// pojok kanan atas
			blockX = 0;
			blockY = 7;
		}
		else
		{
			// pojok kiri atas
			blockX = 0;
			blockY = 0;
		}
    }
    /***********************/
}

unsigned char*** Convert1D3D (unsigned char* inimg1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z)
{
	unsigned char*** img3D;
	img3D = new unsigned char**[z];
	for(int i = 0; i < z; ++i)
	{
		img3D[i] = new unsigned char*[y];
		for(int j = 0; j < y; ++j) img3D[i][j] = new unsigned char[x];
	}
	
	for (int k=0;k<z;k++)
	{
		for (int j=0;j<y;j++)
		{
			for (int i=0;i<x;i++)
			{
				img3D[k][j][i] = inimg1d[i+(j*x)+(k*x*y)];
			}
		}
	}
	return img3D;
}

unsigned char* Convert3D1D (unsigned char*** inimg1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z)
{
	unsigned char* img1D = new unsigned char[x*y*z];
	VISIOCYTELONG count = 0;
	for (int k=0;k<z;k++)
	{
		for (int j=0;j<y;j++)
		{
			for (int i=0;i<x;i++)
			{
				img1D[count] = inimg1d[k][j][i];
				count++;
			}
		}
	}
	return img1D;
}

unsigned char*** Erotion (unsigned char*** data1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z)
{
	unsigned char*** img3D;
	img3D = new unsigned char**[z];
	for(int i = 0; i < z; ++i)
	{
		img3D[i] = new unsigned char*[y];
		for(int j = 0; j < y; ++j) img3D[i][j] = new unsigned char[x];
	}
	
	for (int k=0;k<z;k++)
	{
		for (int j=0;j<y;j++)
		{
			for (int i=0;i<x;i++)
			{
				img3D[k][j][i] = 0;
			}
		}
	}
	
	for (int k=1;k<z-1;k++)
	{
		for (int j=1;j<y-1;j++)
		{
			for (int i=1;i<x-1;i++)
			{
				if (data1d[k][j][i]==255)
				{
					img3D[k][j][i] = 255;
					img3D[k][j][i+1] = 255;
					img3D[k][j][i-1] = 255;
					
					img3D[k][j-1][i] = 255;
					img3D[k][j+1][i] = 255;
					
					img3D[k+1][j][i] = 255;
					img3D[k-1][j][i] = 255;
				}
			}
		}
	}
	return img3D;
}

unsigned char*** Dilation (unsigned char*** data1d, VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z)
{
	unsigned char*** img3D;
	img3D = new unsigned char**[z];
	for(int i = 0; i < z; ++i)
	{
		img3D[i] = new unsigned char*[y];
		for(int j = 0; j < y; ++j) img3D[i][j] = new unsigned char[x];
	}
	
	for (int k=0;k<z;k++)
	{
		for (int j=0;j<y;j++)
		{
			for (int i=0;i<x;i++)
			{
				img3D[k][j][i] = 0;
			}
		}
	}
	
	for (int k=1;k<z-1;k++)
	{
		for (int j=1;j<y-1;j++)
		{
			for (int i=1;i<x-1;i++)
			{
				if (data1d[k][j][i]==255 && data1d[k][j][i+1]==255 && data1d[k][j][i-1]==255 && data1d[k][j-1][i]==255 && data1d[k][j+1][i]==255 && data1d[k+1][j][i]==255 && data1d[k-1][j][i]==255)
				{
					img3D[k][j][i] = 255;
				}
			}
		}
	}
	return img3D;
}

const vector<MyMarker*> APP2(unsigned char* data1d, VISIOCYTELONG N, VISIOCYTELONG M, VISIOCYTELONG P, VISIOCYTELONG sc)
{
	VISIOCYTELONG in_sz[4], c;
	in_sz[0] = N;
	in_sz[1] = M;
	in_sz[2] = P;
	in_sz[3] = sc;
	c = 1;
	vector<MyMarker*> outswc;
	VISIOCYTELONG pagesz = N*M*P;
	delete data1d_1ch;
    try {data1d_1ch = new unsigned char [pagesz];}
    catch(...)
	{
		visiocyte_msg("cannot allocate memory for data1d_1ch."); 
		return outswc;
	}
	
    for(VISIOCYTELONG i = 0; i < pagesz; i++)
        data1d_1ch[i] = data1d[i+(c-1)*pagesz];

	if (p4dImageNew)
	{
		delete p4dImageNew;
		p4dImageNew=0;
	}
	//delete p4dImageNew;
    p4dImageNew = new Image4DSimple;
    if(!p4dImageNew->createImage(N,M,P,1, VISIOCYTE_UINT8))
	{
		return outswc;
	}

    memcpy(p4dImageNew->getRawData(), data1d_1ch, pagesz);

    unsigned char * indata1d;
	indata1d = p4dImageNew->getRawDataAtChannel(0);

    in_sz[3] = 1;
    double dfactor_xy = 1, dfactor_z = 1;

    if (in_sz[0]<=256 && in_sz[1]<=256 && in_sz[2]<=256)
    {
        dfactor_z = dfactor_xy = 1;
    }
    else if (in_sz[0] >= 2*in_sz[2] || in_sz[1] >= 2*in_sz[2])
    {
        if (in_sz[2]<=256)
        {
            double MM = in_sz[0];
            if (MM<in_sz[1]) MM=in_sz[1];
            dfactor_xy = MM / 256.0;
            dfactor_z = 1;
        }
        else
        {
            double MM = in_sz[0];
            if (MM<in_sz[1]) MM=in_sz[1];
            if (MM<in_sz[2]) MM=in_sz[2];
            dfactor_xy = dfactor_z = MM / 256.0;
        }
    }
    else
    {
        double MM = in_sz[0];
        if (MM<in_sz[1]) MM=in_sz[1];
        if (MM<in_sz[2]) MM=in_sz[2];
        dfactor_xy = dfactor_z = MM / 256.0;
    }

    printf("dfactor_xy=%5.3f\n", dfactor_xy);
    printf("dfactor_z=%5.3f\n", dfactor_z);

    if (dfactor_z>1 || dfactor_xy>1)
    {
        visiocyte_msg("enter ds code", 0);

        VISIOCYTELONG out_sz[4];
        unsigned char * outimg=0;
		delete[] outimg;
        if (!downsampling_img_xyz(indata1d, in_sz, dfactor_xy, dfactor_z, outimg, out_sz))
            return outswc;
        p4dImageNew->setData(outimg, out_sz[0], out_sz[1], out_sz[2], out_sz[3], VISIOCYTE_UINT8);

        indata1d = p4dImageNew->getRawDataAtChannel(0);
        in_sz[0] = p4dImageNew->getXDim();
        in_sz[1] = p4dImageNew->getYDim();
        in_sz[2] = p4dImageNew->getZDim();
        in_sz[3] = p4dImageNew->getCDim();
    }

    vector<MyMarker *> outtree;

    cout<<"Start detecting cellbody"<<endl;

    float * phi = 0;
    vector<MyMarker> inmarkers;

    fastmarching_dt_XY(indata1d, phi, in_sz[0], in_sz[1], in_sz[2],2, 10);

    VISIOCYTELONG sz0 = in_sz[0];
    VISIOCYTELONG sz1 = in_sz[1];
    VISIOCYTELONG sz2 = in_sz[2];
    VISIOCYTELONG sz01 = sz0 * sz1;
    VISIOCYTELONG tol_sz = sz01 * sz2;

    VISIOCYTELONG max_loc = 0;
    double max_val = phi[0];
    for(VISIOCYTELONG i = 0; i < tol_sz; i++)
    {
        if(phi[i] > max_val)
        {
            max_val = phi[i];
            max_loc = i;
        }
    }
	
	MyMarker max_marker(max_loc % sz0, max_loc % sz01 / sz0, max_loc / sz01);
	inmarkers.push_back(max_marker);

    cout<<"======================================="<<endl;
    cout<<"Construct the neuron tree"<<endl;

    visiocyte_msg("8bit", 0);

    fastmarching_tree(inmarkers[0], indata1d, outtree, in_sz[0], in_sz[1], in_sz[2], 2, 10, false);
    cout<<"======================================="<<endl;

    //save a copy of the ini tree
    cout<<"Save the initial unprunned tree"<<endl;
    vector<MyMarker*> & inswc = outtree;

    if (1)
    {
        VISIOCYTELONG tmpi;

        vector<MyMarker*> tmpswc;
        for (tmpi=0; tmpi<inswc.size(); tmpi++)
        {
            MyMarker * curp = new MyMarker(*(inswc[tmpi]));
            tmpswc.push_back(curp);

            if (dfactor_xy>1) inswc[tmpi]->x *= dfactor_xy;
            inswc[tmpi]->x += (0);
            if (dfactor_xy>1) inswc[tmpi]->x += dfactor_xy/2;

            if (dfactor_xy>1) inswc[tmpi]->y *= dfactor_xy;
            inswc[tmpi]->y += (0);
            if (dfactor_xy>1) inswc[tmpi]->y += dfactor_xy/2;

            if (dfactor_z>1) inswc[tmpi]->z *= dfactor_z;
            inswc[tmpi]->z += (0);
            if (dfactor_z>1)  inswc[tmpi]->z += dfactor_z/2;
        }

        //saveSWC_file(QString(PARA.inimg_file).append("_ini.swc").toStdString(), inswc);

        for (tmpi=0; tmpi<inswc.size(); tmpi++)
        {
            inswc[tmpi]->x = tmpswc[tmpi]->x;
            inswc[tmpi]->y = tmpswc[tmpi]->y;
            inswc[tmpi]->z = tmpswc[tmpi]->z;
        }

        for(tmpi = 0; tmpi < tmpswc.size(); tmpi++)
            delete tmpswc[tmpi];
        tmpswc.clear();
    }

    cout<<"Pruning neuron tree"<<endl;

    visiocyte_msg("start to use happ.\n", 0);
    happ(inswc, outswc, indata1d, in_sz[0], in_sz[1], in_sz[2],10, 5, 0.3333);

    if (p4dImageNew) {delete p4dImageNew; p4dImageNew=0;} //free buffe

    inmarkers[0].x *= dfactor_xy;
    inmarkers[0].y *= dfactor_xy;
    inmarkers[0].z *= dfactor_z;


    for(VISIOCYTELONG i = 0; i < outswc.size(); i++)
    {
        if (dfactor_xy>1) outswc[i]->x *= dfactor_xy;
        outswc[i]->x += 0;
        if (dfactor_xy>1) outswc[i]->x += dfactor_xy/2;

        if (dfactor_xy>1) outswc[i]->y *= dfactor_xy;
        outswc[i]->y += 0;
        if (dfactor_xy>1) outswc[i]->y += dfactor_xy/2;

        if (dfactor_z>1) outswc[i]->z *= dfactor_z;
        outswc[i]->z += 0;
        if (dfactor_z>1)  outswc[i]->z += dfactor_z/2;

        outswc[i]->radius *= dfactor_xy; //use xy for now
    }

    //re-estimate the radius using the original image
    double real_thres = 40;

    VISIOCYTELONG szOriginalData[4] = {N,M,P, 1};

    int method_radius_est = 2;
    for(VISIOCYTELONG i = 0; i < outswc.size(); i++)
    {
        //printf(" node %ld of %ld.\n", i, outswc.size());
        outswc[i]->radius = markerRadius(data1d_1ch, szOriginalData, *(outswc[i]), real_thres, method_radius_est);
    }

	return outswc;
}

struct location {
	int x;
	int y;
	int z;
};

struct maps {
	location main;
	location adding;
};

vector<MyMarker*> Combining(vector<MyMarker*> main, vector<MyMarker*> adding)
{
	vector<maps> dataLocation;
	vector<MyMarker*> result;
	vector<MyMarker*> root;
	result = main;
	maps insert;
	MyMarker* addingCheck;
	MyMarker* check;
	location ins1;
	location ins2;
	VISIOCYTELONG size = result.size();
	VISIOCYTELONG idx;
	double minimum;
	double temp;
	double Threshold = 2;
	int x, y, z;
	bool flag;

	for(VISIOCYTELONG i=0;i<adding.size();i++)
	{
		minimum = 1000.0;
		for(VISIOCYTELONG j=0;j<main.size();j++)
		{
			temp = sqrt(double(((main[j]->x - adding[i]->x)*(main[j]->x - adding[i]->x))+((main[j]->y - adding[i]->y)*(main[j]->y - adding[i]->y))+((main[j]->z - adding[i]->z)*(main[j]->z - adding[i]->z))));
			if (temp<minimum)
			{
				minimum = temp;
				x = main[j]->x;
				y = main[j]->y;
				z = main[j]->z;
			}
		}
		if (minimum<=Threshold)
		{
			ins1.x = x;
			ins1.y = y;
			ins1.z = z;
			
			ins2.x = adding[i]->x;
			ins2.y = adding[i]->y;
			ins2.z = adding[i]->z;
			insert.main = ins1;
			insert.adding = ins2;
			dataLocation.push_back(insert);
		}
		else
		{
			// gak ada di map
			ins1.x = x;
			ins1.y = y;
			ins1.z = z;
			
			ins2.x = adding[i]->x;
			ins2.y = adding[i]->y;
			ins2.z = adding[i]->z;
			insert.main = ins1;
			insert.adding = ins2;
			dataLocation.push_back(insert);
			result.push_back(adding[i]);
		}
	}
	int count = 0;
	for (VISIOCYTELONG i=size;i<result.size(); i++)
	{
		idx = 0;
		flag = true;
		for(VISIOCYTELONG j=0;j<result.size();j++)
		{
			if (result[i]->parent == result[j])
			{
				flag = false;
			}
		}
		if (flag)
		{
			minimum = 1000.0;
			for(VISIOCYTELONG j=0;j<result.size();j++)
			{
				if (i!=j)
				{
					temp = sqrt(double(((result[i]->x-result[j]->x)*(result[i]->x-result[j]->x)) + ((result[i]->y-result[j]->y)*(result[i]->y-result[j]->y)) + ((result[i]->z-result[j]->z)*(result[i]->z-result[j]->z))));
					if (temp<minimum)
					{
						minimum = temp;
						idx = j;
					}
				}
			}
			result[i]->parent = result[idx];
		}
	}
	
	return result;
}

unsigned char *BVersion(unsigned char *img, VISIOCYTELONG size)
{
/*	unsigned char* img;
	img = new unsigned char[size];
	
	for (VISIOCYTELONG i=0;i<size;i++)
	{
		img[i] = image[i];
	}*/
	
	int contrast_threshold = 15;
	int maxV = img[0];
	int minV = img[0];
	int local_contrast;
	int mid_gray;
	
	for (VISIOCYTELONG i=1;i<size;i++)
	{
		maxV = int(max(double(maxV), double(img[i])));
		minV = int(min(double(minV), double(img[i])));
	}
	local_contrast = maxV-minV;
	mid_gray = int((maxV+minV)/2);
	
	for (VISIOCYTELONG i=1;i<size;i++)
	{
		if (local_contrast<contrast_threshold)
		{
			if (mid_gray>=128)
			{
				img[i] = 255;
			}
			else
			{
				img[i] = 0;
			}
		}
		else
		{
			if (img[i]>=mid_gray)
			{
				img[i] = 255;
			}
			else
			{
				img[i] = 0; 
			}
		}
	}
	
	return img;
}

unsigned char* NVersion (unsigned char* img, VISIOCYTELONG size) //Niblack
{
	/*unsigned char* img;
	img = new unsigned char[size];
	
	for (VISIOCYTELONG i=0;i<size;i++)
	{
		img[i] = image[i];
    }*/

    int distribution[256], pixelMin, pixelMax, temp2;
	float  histNormalized[256]; 
	float nonzero = 0.1;
	float w = 0;
	float u = 0;
	float uT = 0;
	float  work1, work2;		// working variables
	double work3 = 0.0;
	int binary_threshold = 0;
	float imagemean = 0.0;

	for (int i=0;i<256;i++)
	{
		distribution[i] = 0;
		histNormalized[i] = 0;
	}

	for (VISIOCYTELONG i=0;i<size;i++)
    {
        distribution[img[i]] = distribution[img[i]] + 1;
		imagemean += img[i];
		if (img[i] >0 )
			nonzero = nonzero +1 ;
    }
	// Create normalised histogram values
	// (size=image width * image height)
	for (int i=1; i<256; i++) 
		histNormalized[i] = distribution[i]/(float)nonzero;
	// Calculate total mean level
	for (int i=1; i<256; i++) 
		uT+=((i+1)*histNormalized[i]);
	for (int i=1; i<256; i++) {
		w+=histNormalized[i];
		u+=((i+1)*histNormalized[i]);
		work1 = (uT * w - u);
		work2 = (work1 * work1) / ( w * (1.0f-w) );
		if (work2>work3) work3=work2;
	}
	// Convert the final value to an integer
	

	imagemean = imagemean/(float)nonzero;
	float sum_deviation=0.0, deviation = 0.0;
	
	for(VISIOCYTELONG i=0;i<size;i++)
		if (img[i]!=0) sum_deviation+=(img[i]-imagemean)*(img[i]-imagemean);
	
	
	deviation = sqrt(sum_deviation/(float)nonzero);
	
	float k = 0.2, c = 0;
	binary_threshold =  imagemean + k * deviation - c ;
	
	for (int r = 0; r < size; r++)
    {
            if (img[r] > binary_threshold) 
				img[r] = 255;
			else
				img[r] = 0;
    }
	
	return img;
}

unsigned char* SVersion (unsigned char* img, VISIOCYTELONG size) //Sauvola
{
/*	unsigned char* img;
	img = new unsigned char[size];
	
	for (VISIOCYTELONG i=0;i<size;i++)
	{
		img[i] = image[i];
	}*/
	
	int distribution[256], pixelMin, pixelMax, temp2;
	float  histNormalized[256]; 
	float nonzero = 0.1;
	float w = 0;
	float u = 0;
	float uT = 0;
	float  work1, work2;		// working variables
	double work3 = 0.0;
	int binary_threshold = 0;
	float imagemean = 0.0;
	
	for (int i=0;i<256;i++)
	{
		distribution[i] = 0;
		histNormalized[i] = 0;
	}
	
	for (VISIOCYTELONG i=0;i<size;i++)
	{
		distribution[img[i]] = distribution[img[i]] + 1;
		imagemean += img[i];
		if (img[i] >0 )
			nonzero = nonzero +1 ;
	}
	// Create normalised histogram values
	// (size=image width * image height)
	for (int i=1; i<256; i++) 
		histNormalized[i] = distribution[i]/(float)nonzero;
	// Calculate total mean level
	for (int i=1; i<256; i++) 
		uT+=((i+1)*histNormalized[i]);
	for (int i=1; i<256; i++) {
		w+=histNormalized[i];
		u+=((i+1)*histNormalized[i]);
		work1 = (uT * w - u);
		work2 = (work1 * work1) / ( w * (1.0f-w) );
		if (work2>work3) work3=work2;
	}
	// Convert the final value to an integer
	
	
	imagemean = imagemean/(float)nonzero;
	float sum_deviation=0.0, deviation = 0.0;
	
	for(VISIOCYTELONG i=0;i<size;i++)
		if (img[i]!=0) sum_deviation+=(img[i]-imagemean)*(img[i]-imagemean);
	deviation = sqrt(sum_deviation/(float)nonzero);
	
	float k = 0.5, r = 128;
	binary_threshold =  imagemean * ( 1 + k * ( deviation / r - 1 ) ) ;
	
	for (int r = 0; r < size; r++)
    {
            if (img[r] > binary_threshold) 
				img[r] = 255;
			else
				img[r] = 0;
    }
	
	return img;
}
