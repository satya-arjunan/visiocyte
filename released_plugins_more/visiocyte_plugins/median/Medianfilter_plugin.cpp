/* Medianfilter_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * Revised from a previous minmaxfiltering plugin
 * 2013-06-10 : by Zhi Zhou
 */


//last change: by PHC, 2013-07-08, adjust some styles and correct a small bug.

#include "visiocyte_message.h"
#include <vector>
#include "Medianfilter_plugin.h"
#include <QtGui>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "../plugin_loader/visiocyte_plugin_loader.h"
#include <boost/lexical_cast.hpp>


#include "stackutil.h"
#define INF 1E9

using namespace std;
Q_EXPORT_PLUGIN2(medianfilter, MedianFilterPlugin);

void median_fixed_domenu(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool median_fixed_dofunc(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

void median_adaptive_domenu(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool median_adaptive_dofunc(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

template <class T> void median_filter(T* data1d,
                                      VISIOCYTELONG *in_sz,
                                      unsigned int Wx,
                                      unsigned int Wy,
                                      unsigned int Wz,
                                      unsigned int c,
                                      T* &outimg);

template <class T> void adp_median_filter(T* data1d,
                                          VISIOCYTELONG *in_sz,
                                          unsigned int c,
                                          T* &outimg,T* gsdtdata1d );



QStringList MedianFilterPlugin::menulist() const
{
    return QStringList()
            <<tr("Fixed Window")
           <<tr("Adaptive Window")
             ;
}


void MedianFilterPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)

{
    if (menu_name == tr("Fixed Window"))
    {
        median_fixed_domenu(callback,parent);
    }
    else if (menu_name == tr("Adaptive Window"))
    {
        median_adaptive_domenu(callback,parent);
    }
}


QStringList MedianFilterPlugin::funclist() const
{
    return QStringList()
            <<tr("fixed_window")
           <<tr("adaptive_window")
             <<tr("help");
}


bool MedianFilterPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{

    if (func_name == tr("fixed_window"))
    {
        return median_fixed_dofunc(callback, input, output);
    }
    else if(func_name == tr("adaptive_window"))
    {
        return median_adaptive_dofunc(callback, input, output);
    }
    else if (func_name == tr("help"))
    {
        cout<<"Usage : visiocyte -x medianfilter -f fixed_window -i <inimg_file> -o <outimg_file> -p <wx> <wy> <wz> <ch>"<<endl;
        cout<<endl;
        cout<<"wx          filter window radius size (pixel #) in x direction, window size is 2*wx+1, default 1"<<endl;
        cout<<"wy          filter window radius size (pixel #) in y direction, window size is 2*wy+1, default 1"<<endl;
        cout<<"wz          filter window radius size (pixel #) in z direction, window size is 2*wz+1, default 1"<<endl;
        cout<<"ch          the input channel value, default 1 and start from 1, default 1"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"Usage : visiocyte -x medianfilter -f adaptive_window -i <inimg_file> -o <outimg_file> -p <ch> <th_idx> <th>"<<endl;
        cout<<endl;
        cout<<"ch	   the input channel value, default 1 and start from 1, default 1"<<endl;
	cout<<"th_idx 	   threshold method index, 0: mean, 1: usr defined, default 0"<<endl;
	cout<<"th	   user defined threshold value (when th_idx is 1), default 0"<<endl;
        cout<<endl;
        cout<<endl;
        return true;
    }
}

bool median_fixed_dofunc(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
    cout<<"Welcome to Median filter with fixed window"<<endl;
    if (output.size() != 1) return false;
    unsigned int Wx=1, Wy=1, Wz=1, ch=1;
    if (input.size()>=2)
    {

        vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
        cout<<paras.size()<<endl;
        if(paras.size() >= 1) Wx = atoi(paras.at(0));
        if(paras.size() >= 2) Wy = atoi(paras.at(1));
        if(paras.size() >= 3) Wz = atoi(paras.at(2));
        if(paras.size() >= 4) ch = atoi(paras.at(3));
    }

    char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
    char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);

    cout<<"Wx = "<<Wx<<endl;
    cout<<"Wy = "<<Wy<<endl;
    cout<<"Wz = "<<Wz<<endl;
    cout<<"ch = "<<ch<<endl;
    cout<<"inimg_file = "<<inimg_file<<endl;
    cout<<"outimg_file = "<<outimg_file<<endl;

    unsigned char * data1d = 0,  * outimg1d = 0;
    VISIOCYTELONG in_sz[4];

    unsigned int c = ch;//-1;

    int datatype;
    if(!simple_loadimage_wrapper(callback, inimg_file, data1d, in_sz, datatype))
    {
        cerr<<"load image "<<inimg_file<<" error!"<<endl;
        return false;
    }

    //input
    void* outimg = 0;

    switch (datatype)
    {
    case 1: median_filter(data1d, in_sz, Wx, Wy, Wz, c, (unsigned char*&)outimg); break;
    case 2: median_filter((unsigned short int*)data1d, in_sz, Wx, Wy, Wz, c, (unsigned short int*&)outimg); break;
    case 4: median_filter((float *)data1d, in_sz, Wx, Wy, Wz, c, (float*&)outimg); break;
    default:
        visiocyte_msg("Invalid datatype.");
        if (data1d) {delete []data1d; data1d=0;}
        //if (in_sz) {delete []in_sz; in_sz=0;}
        return false;
    }

    // save image
    in_sz[3]=1;
    simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)outimg, in_sz, datatype);

    if(outimg) {delete []outimg; outimg =0;}
    if (data1d) {delete []data1d; data1d=0;}

    return true;
}


void median_fixed_domenu(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
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

    unsigned char* data1d = p4DImage->getRawData();
    //VISIOCYTELONG totalpxls = p4DImage->getTotalBytes();
    VISIOCYTELONG pagesz = p4DImage->getTotalUnitNumberPerChannel();

    VISIOCYTELONG N = p4DImage->getXDim();
    VISIOCYTELONG M = p4DImage->getYDim();
    VISIOCYTELONG P = p4DImage->getZDim();
    VISIOCYTELONG sc = p4DImage->getCDim();

    //define datatype here

    //input
    bool ok1, ok2, ok3, ok4;
    unsigned int Wx=1, Wy=1, Wz=1, c=1;

    Wx = QInputDialog::getInteger(parent, "Window X ",
                                  "Enter radius (window size is 2*radius+1):",
                                  1, 1, N, 1, &ok1);

    if(ok1)
    {
        Wy = QInputDialog::getInteger(parent, "Window Y",
                                      "Enter radius (window size is 2*radius+1):",
                                      1, 1, M, 1, &ok2);
    }
    else
        return;

    if(ok2)
    {
        Wz = QInputDialog::getInteger(parent, "Window Z",
                                      "Enter radius (window size is 2*radius+1):",
                                      1, 1, P, 1, &ok3);
    }
    else
        return;

    if(sc==1)
    {
        c=1;
        ok4=true;
    }
    else
    {
        if(ok3)
        {
            c = QInputDialog::getInteger(parent, "Channel",
                                         "Enter channel NO:",
                                         1, 1, sc, 1, &ok4);
        }
        else
            return;
    }


    // filter
    VISIOCYTELONG in_sz[4];
    in_sz[0] = N; in_sz[1] = M; in_sz[2] = P; in_sz[3] = sc;

    ImagePixelType pixeltype = p4DImage->getDatatype();
    void* outimg = 0;
    switch (pixeltype)
    {
    case VISIOCYTE_UINT8: median_filter(data1d, in_sz, Wx, Wy, Wz, c,(unsigned char* &)outimg); break;
    case VISIOCYTE_UINT16: median_filter((unsigned short int *)data1d, in_sz, Wx, Wy, Wz, c, (unsigned short int* &)outimg); break;
    case VISIOCYTE_FLOAT32: median_filter((float *)data1d, in_sz, Wx, Wy, Wz, c, (float* &)outimg);break;
    default: visiocyte_msg("Invalid data type. Do nothing."); return;
    }

    // display
    Image4DSimple * new4DImage = new Image4DSimple();
    new4DImage->setData((unsigned char *)outimg, N, M, P, 1, pixeltype);
    visiocytehandle newwin = callback.newImageWindow();
    callback.setImage(newwin, new4DImage);
    callback.setImageName(newwin, "Median filter result");
    callback.updateImageWindow(newwin);

    return;
}



template <class T> void median_filter(T* data1d,
                                      VISIOCYTELONG *in_sz,
                                      unsigned int Wx,
                                      unsigned int Wy,
                                      unsigned int Wz,
                                      unsigned int c,
                                      T* &outimg)
{

    VISIOCYTELONG N = in_sz[0];
    VISIOCYTELONG M = in_sz[1];
    VISIOCYTELONG P = in_sz[2];
    VISIOCYTELONG sc = in_sz[3];
    VISIOCYTELONG pagesz = N*M*P;

    T *arr,tmp;
    int ii,jj;
    int size = (2*Wx+1)*(2*Wy+1)*(2*Wz+1);
    arr = new T[size];

    //filtering
    VISIOCYTELONG offsetc = (c-1)*pagesz;

    //declare temporary pointer
    T *pImage = new T [pagesz];
    if (!pImage)
    {
        printf("Fail to allocate memory.\n");
        return;
    }
    else
    {
        for(VISIOCYTELONG i=0; i<pagesz; i++)
            pImage[i] = 0;
    }

    //Median Filtering
    for(VISIOCYTELONG iz = 0; iz < P; iz++)
    {
         printf("\r median filter : %d %% completed ", ((iz + 1)*100) / P);fflush(stdout);
        VISIOCYTELONG offsetk = iz*M*N;
        for(VISIOCYTELONG iy = 0; iy < M; iy++)
        {
            VISIOCYTELONG offsetj = iy*N;
            for(VISIOCYTELONG ix = 0; ix < N; ix++)
            {

                VISIOCYTELONG xb = ix-Wx; if(xb<0) xb = 0;
                VISIOCYTELONG xe = ix+Wx; if(xe>=N-1) xe = N-1;
                VISIOCYTELONG yb = iy-Wy; if(yb<0) yb = 0;
                VISIOCYTELONG ye = iy+Wy; if(ye>=M-1) ye = M-1;
                VISIOCYTELONG zb = iz-Wz; if(zb<0) zb = 0;
                VISIOCYTELONG ze = iz+Wz; if(ze>=P-1) ze = P-1;
                ii = 0;

                for(VISIOCYTELONG k=zb; k<=ze; k++)
                {
                    VISIOCYTELONG offsetkl = k*M*N;
                    for(VISIOCYTELONG j=yb; j<=ye; j++)
                    {
                        VISIOCYTELONG offsetjl = j*N;
                        for(VISIOCYTELONG i=xb; i<=xe; i++)
                        {
                            T dataval = data1d[ offsetc + offsetkl + offsetjl + i];
                            arr[ii] = dataval;
                            if (ii>0)
                            {
                                jj = ii;
                                while(jj > 0 && arr[jj-1]>arr[jj])
                                {
                                    tmp = arr[jj];
                                    arr[jj] = arr[jj-1];
                                    arr[jj-1] = tmp;
                                    jj--;
                                }
                            }
                            ii++;
                        }
                    }
                }


                //set value
                VISIOCYTELONG index_pim = offsetk + offsetj + ix;
                pImage[index_pim] = arr[int(0.5*ii)+1];
            }
        }
    }
printf("\n");

    outimg = pImage;
    delete [] arr;

}

void median_adaptive_domenu(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
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

    unsigned char* data1d = p4DImage->getRawData();
    //VISIOCYTELONG totalpxls = p4DImage->getTotalBytes();
    VISIOCYTELONG pagesz = p4DImage->getTotalUnitNumberPerChannel();

    VISIOCYTELONG N = p4DImage->getXDim();
    VISIOCYTELONG M = p4DImage->getYDim();
    VISIOCYTELONG P = p4DImage->getZDim();
    VISIOCYTELONG sc = p4DImage->getCDim();
    
     //add input dialog
	
	AdaptiveMedianDialog dialog(callback, parent);
	if (!dialog.image)
		return;
	
	if (dialog.exec()!=QDialog::Accepted)
		return;
	
	dialog.update();
	
	Image4DSimple* subject = dialog.image;
	if (!subject)
		return;
	ROIList pRoiList = dialog.pRoiList;
	
	int c = dialog.ch+1;
	int th_idx = dialog.th_idx;
    double th = dialog.thresh;
	
	if(th_idx ==0)
	{
        VISIOCYTELONG offsetc = (c-1)*pagesz;
		for(VISIOCYTELONG iz = 0; iz < P; iz++)
	   	{
            double PixelSum = 0;
            VISIOCYTELONG offsetk = iz*M*N;
			for(VISIOCYTELONG iy = 0; iy < M; iy++)
			{
			    VISIOCYTELONG offsetj = iy*N;
			    for(VISIOCYTELONG ix = 0; ix < N; ix++)
			    {

				VISIOCYTELONG PixelVaule = data1d[offsetc + offsetk + offsetj + ix];
                PixelSum = PixelSum + PixelVaule;
				
			    }
			}
                th = th + PixelSum/(M*N*P);
        }
	


	}

    // filter
    VISIOCYTELONG in_sz[4];
    in_sz[0] = N; in_sz[1] = M; in_sz[2] = P; in_sz[3] = sc;

    ImagePixelType pixeltype = p4DImage->getDatatype();
    simple_saveimage_wrapper(callback, "temp.visiocyteraw", (unsigned char *)data1d, in_sz, pixeltype);
    //invoke gsdt function
    VISIOCYTEPluginArgItem arg;
    VISIOCYTEPluginArgList input;
    VISIOCYTEPluginArgList output;
    

    arg.type = "random";std::vector<char*> args1;
   // std:: string inputName(callback.getImageName(curwin).toStdString());char* inputName2 =  new char[inputName.length() + 1]; strcpy(inputName2, inputName.c_str());
    args1.push_back("temp.visiocyteraw"); arg.p = (void *) & args1; input<< arg;
    arg.type = "random";std::vector<char*> args;
    char channel = '0' + (c-1); 
    string threshold = boost::lexical_cast<string>(th); char* threshold2 =  new char[threshold.length() + 1]; strcpy(threshold2, threshold.c_str());
    args.push_back(threshold2);args.push_back("1");args.push_back(&channel);args.push_back("1"); arg.p = (void *) & args; input << arg;
    arg.type = "random";std::vector<char*> args2;args2.push_back("gsdtImage.visiocyteraw"); arg.p = (void *) & args2; output<< arg;
    	
    QString full_plugin_name = "gsdt";
    QString func_name = "gsdt";

    callback.callPluginFunc(full_plugin_name,func_name, input,output);

    //system("visiocyte -x gsdt -f gsdt -i /home/zhiz/Desktop/visiocyte/Images/ex_Repo_hb9_eve.tif -o /home/zhiz/Desktop/visiocyte/Images/gsdt_ex_Repo_hb9_eve.tif -p 0 1 0 1.0");

    unsigned char * gsdtdata1d = 0;
    int datatype;
    VISIOCYTELONG in_zz[4];

    char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
    if(!simple_loadimage_wrapper(callback, outimg_file, gsdtdata1d, in_zz, datatype))
    {
        visiocyte_msg("Fail to load image");
        return;
    }
    remove("gsdtImage.visiocyteraw");
    remove("temp.visiocyteraw");

    void* outimg = 0;
    switch (pixeltype)
    {
    case VISIOCYTE_UINT8: adp_median_filter(data1d, in_sz, c,(unsigned char* &)outimg, gsdtdata1d); break;
    case VISIOCYTE_UINT16: adp_median_filter((unsigned short int *)data1d, in_sz, c, (unsigned short int* &)outimg,(unsigned short int *)gsdtdata1d); break;
    case VISIOCYTE_FLOAT32: adp_median_filter((float *)data1d, in_sz, c, (float* &)outimg,(float *)gsdtdata1d);break;
    default: visiocyte_msg("Invalid data type. Do nothing."); return;
    }

    // display
    Image4DSimple * new4DImage = new Image4DSimple();
    new4DImage->setData((unsigned char *)outimg, N, M, P, 1, pixeltype);
    visiocytehandle newwin = callback.newImageWindow();
    callback.setImage(newwin, new4DImage);
    callback.setImageName(newwin, "Adaptive Median filter result");
    callback.updateImageWindow(newwin);

    return;
}

bool median_adaptive_dofunc(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
    cout<<"Welcome to Median filter with adaptive window"<<endl;
    if (output.size() != 1) return false;
    unsigned int ch = 1,th_idx = 0;
    double th = 0;
    if (input.size()>=2)
    {

        vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
        cout<<paras.size()<<endl;
        if(paras.size() >= 1) ch = atoi(paras.at(0));
        if(paras.size() >= 2) th_idx = atoi(paras.at(1));
	if(paras.size() >= 3) th = atoi(paras.at(2));
    }

    char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
    char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);

    cout<<"ch = "<<ch<<endl;
    cout<<"th_idx = "<<th_idx<<endl;
    cout<<"th = "<<th<<endl;	
    cout<<"inimg_file = "<<inimg_file<<endl;
    cout<<"outimg_file = "<<outimg_file<<endl;

    unsigned char * data1d = 0,  * outimg1d = 0;
    VISIOCYTELONG in_sz[4];

    unsigned int c = ch;//-1;

    int datatype;
    if(!simple_loadimage_wrapper(callback, inimg_file, data1d, in_sz, datatype))
    {
        cerr<<"load image "<<inimg_file<<" error!"<<endl;
        return false;
    }

    
    if(th_idx ==0)
    {
        VISIOCYTELONG N = in_sz[0];
        VISIOCYTELONG M = in_sz[1];
        VISIOCYTELONG P = in_sz[2];
        VISIOCYTELONG pagesz = N*M*P;
        VISIOCYTELONG offsetc = (c-1)*pagesz;
        for(VISIOCYTELONG iz = 0; iz < P; iz++)
        {
            double PixelSum = 0;
            VISIOCYTELONG offsetk = iz*M*N;
            for(VISIOCYTELONG iy = 0; iy < M; iy++)
            {
                VISIOCYTELONG offsetj = iy*N;
                for(VISIOCYTELONG ix = 0; ix < N; ix++)
                {

                VISIOCYTELONG PixelVaule = data1d[offsetc + offsetk + offsetj + ix];
                PixelSum = PixelSum + PixelVaule;

                }
            }
             th = th + PixelSum/(M*N*P);
        }


		
    }    
	
    //invoke gsdt function
    VISIOCYTEPluginArgItem arg;
    VISIOCYTEPluginArgList input2;
    VISIOCYTEPluginArgList output2;

    arg.type = "random";std::vector<char*> args1;
    args1.push_back(inimg_file); arg.p = (void *) & args1; input2<< arg;
    arg.type = "random";std::vector<char*> args;
    char channel = '0' + (c-1);
    string threshold = boost::lexical_cast<string>(th); char* threshold2 =  new char[threshold.length() + 1]; strcpy(threshold2, threshold.c_str());	
    args.push_back(threshold2);args.push_back("1");args.push_back(&channel);args.push_back("1"); arg.p = (void *) & args; input2 << arg;
    arg.type = "random";std::vector<char*> args2;
    args2.push_back("gsdtImage.tiff"); arg.p = (void *) & args2; output2<< arg;

    QString full_plugin_name = "gsdt"; //partial name will work
    QString func_name = "gsdt";

    callback.callPluginFunc(full_plugin_name,func_name, input2,output2);

    unsigned char * gsdtdata1d = 0;
    int datatype2;
    VISIOCYTELONG in_zz[4];

    char * outimg_file2 = ((vector<char*> *)(output2.at(0).p))->at(0);
    if (!simple_loadimage_wrapper(callback, outimg_file2, gsdtdata1d, in_zz, datatype2))
    {
        visiocyte_msg("Fail to load image");
        return false;
    }

    remove("gsdtImage.tiff");
    //input
    void* outimg = 0;

    switch (datatype)
    {
    case 1: adp_median_filter(data1d, in_sz, c,(unsigned char* &)outimg, gsdtdata1d); break;
    case 2: adp_median_filter((unsigned short int *)data1d, in_sz, c, (unsigned short int* &)outimg,(unsigned short int *)gsdtdata1d); break;
    case 4: adp_median_filter((float *)data1d, in_sz, c, (float* &)outimg,(float *)gsdtdata1d);break;
    default:
        visiocyte_msg("Invalid datatype.");
        if (data1d) {delete []data1d; data1d=0;}
        //if (in_sz) {delete []in_sz; in_sz=0;}
        return false;
    }

    // save image
    in_sz[3]=1;
    simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)outimg, in_sz, datatype);

    if(outimg) {delete []outimg; outimg =0;}
    if (data1d) {delete []data1d; data1d=0;}
    //if (in_sz) {delete []in_sz; in_sz=0;}

    return true;
}



template <class T> void adp_median_filter(T* data1d,
                                          VISIOCYTELONG *in_sz,
                                          unsigned int c,
                                          T* &outimg,
                                          T* gsdtdatald)
{

    VISIOCYTELONG N = in_sz[0];
    VISIOCYTELONG M = in_sz[1];
    VISIOCYTELONG P = in_sz[2];
    VISIOCYTELONG sc = in_sz[3];
    VISIOCYTELONG pagesz = N*M*P;

    int Wx,Wy,Wz;

    //filtering
    VISIOCYTELONG offsetc = (c-1)*pagesz;

    //declare temporary pointer
    T *pImage = new T [pagesz];
    if (!pImage)
    {
        printf("Fail to allocate memory.\n");
        return;
    }
    else
    {
        for(VISIOCYTELONG i=0; i<pagesz; i++)
            pImage[i] = 0;
    }

    //Median Filtering
    for(VISIOCYTELONG iz = 0; iz < P; iz++)
    {
        printf("\r median filter : %d %% completed ", ((iz + 1)*100) / P);fflush(stdout);
        VISIOCYTELONG offsetk = iz*M*N;
        for(VISIOCYTELONG iy = 0; iy < M; iy++)
        {
            VISIOCYTELONG offsetj = iy*N;
            for(VISIOCYTELONG ix = 0; ix < N; ix++)
            {

                T PixelValue = data1d[offsetc+offsetk + offsetj + ix];
                T GsdtValue = gsdtdatald[offsetk + offsetj + ix];
                Wx = (int)((std::log(double(PixelValue))/std::log(double(2)))/GsdtValue + 0.5);
                //printf("%d %d\n",PixelValue,Wx);

                if ((Wx<=0)||(PixelValue==0))
                {
                    VISIOCYTELONG index_pim = offsetk + offsetj + ix;
                    pImage[index_pim] = PixelValue;
                }
                else
                {
                    Wy = Wx;
                    Wz = Wx;

                    VISIOCYTELONG xb = ix-Wx; if(xb<0) xb = 0;
                    VISIOCYTELONG xe = ix+Wx; if(xe>=N-1) xe = N-1;
                    VISIOCYTELONG yb = iy-Wy; if(yb<0) yb = 0;
                    VISIOCYTELONG ye = iy+Wy; if(ye>=M-1) ye = M-1;
                    VISIOCYTELONG zb = iz-Wz; if(zb<0) zb = 0;
                    VISIOCYTELONG ze = iz+Wz; if(ze>=P-1) ze = P-1;

                    T *arr,tmp;
                    int ii,jj;
                    int size = (2*Wx+1)*(2*Wy+1)*(2*Wz+1);
                    arr = new T[size];
                    //printf("%d %d\n",PixelValue,size);

                    ii = 0;

                    for(VISIOCYTELONG k=zb; k<=ze; k++)
                    {
                        VISIOCYTELONG offsetkl = k*M*N;
                        for(VISIOCYTELONG j=yb; j<=ye; j++)
                        {
                            VISIOCYTELONG offsetjl = j*N;
                            for(VISIOCYTELONG i=xb; i<=xe; i++)
                            {
                                T dataval = data1d[ offsetc + offsetkl + offsetjl + i];
                                arr[ii] = dataval;
                                if (ii>0)
                                {
                                    jj = ii;
                                    while(jj > 0 && arr[jj-1]>arr[jj])
                                    {
                                        tmp = arr[jj];
                                        arr[jj] = arr[jj-1];
                                        arr[jj-1] = tmp;
                                        jj--;
                                    }
                                }
                                ii++;
                            }
                        }
                    }

                    //for (ii = 0;ii<size;ii++)
                    //	printf("%d ",arr[ii]);
                    //	printf("\n");
                    //set value
                    VISIOCYTELONG index_pim = offsetk + offsetj + ix;
                    pImage[index_pim] = arr[int(0.5*size)+1];
                    delete [] arr;
                }
            }
        }
    }

    outimg = pImage;
    return;
}

