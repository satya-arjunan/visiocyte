/* rescale_and_convert.cpp
 * 2012-04-11 Hanchuan Peng
 */


#include <QtGui>

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "visiocyte_message.h"
//#include "stackutil.h"

#include "rescale_and_convert.h"

using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(rescale, RescaleConvertPlugin)

void processImage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, const QString & menu_name);
bool processImage(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, bool b_convert2uint8);
template <class T> bool rc(T* data1d, VISIOCYTELONG *sz, VISIOCYTELONG c, double apercent); 
template <class T> bool scaleintensity(T *img, VISIOCYTELONG sz[4], VISIOCYTELONG channo, double lower_th, double higher_th, double target_min, double target_max);

const QString title = QObject::tr("Rescale and Convert Plugin");
QStringList RescaleConvertPlugin::menulist() const
{
    return QStringList() 
    << tr("Rescale all channels' intensity to [0,255] with 1% saturation") 
    << tr("Rescale only one channel")
    << tr("Rescale all channels' intensity by shifting number of bits")
    << tr("About");
}

void RescaleConvertPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("Rescale all channels' intensity to [0,255] with 1% saturation") || menu_name == tr("Rescale only one channel"))
	{
		processImage(callback,parent, menu_name);
	}
    else if  (menu_name == tr("Rescale all channels' intensity by shifting number of bits"))
    {
        visiocytehandle curwin = callback.currentImageWindow();
        if (!curwin)
        {
            visiocyte_msg("You don't have any image open in the main window.");
            return;
        }

        Image4DSimple* p4DImage = callback.getImage(curwin);
        if (!p4DImage)
        {
            visiocyte_msg("The image pointer is invalid. Ensure your data is valid and try again!");
            return;
        }

        bool ok;
        int bitnumber = QInputDialog::getInteger(parent, "Shifted Bit Number",
                                     "Enter shifted number:",
                                     3, 1, 8, 1, &ok);

        if (!ok)
            return;

        unsigned char* data1d = p4DImage->getRawData();
        VISIOCYTELONG in_sz[4];
        in_sz[0] = p4DImage->getXDim();
        in_sz[1] = p4DImage->getYDim();
        in_sz[2] = p4DImage->getZDim();
        in_sz[3] = p4DImage->getCDim();

        VISIOCYTELONG tunits =  in_sz[0]* in_sz[1]* in_sz[2]* in_sz[3];
        unsigned char * total1dData_8bit = 0;
        try
        {
            total1dData_8bit = new unsigned char [tunits];
        }
        catch (...)
        {
            visiocyte_msg("Fail to allocate memory in total1dData_8bit.\n");
            return;
        }
        for (VISIOCYTELONG i=0;i<tunits;i++)
        {
            total1dData_8bit[i] = data1d[i] >> bitnumber << bitnumber;
        }

        Image4DSimple * new4DImage = new Image4DSimple();
        new4DImage->setData((unsigned char *)total1dData_8bit, in_sz[0], in_sz[1], in_sz[2], in_sz[3], p4DImage->getDatatype());
        visiocytehandle newwin = callback.newImageWindow();
        callback.setImage(newwin, new4DImage);
        callback.setImageName(newwin, "Rescaled result");
        callback.updateImageWindow(newwin);
    }
    else if (menu_name == tr("About"))
    {
         visiocyte_msg("This plugin is developed by Hanchuan Peng as a simple example for rescaling an image (1% saturation) and convert to 8bit data.");
    }
}

void processImage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, const QString & menu_name)
{
    visiocytehandle curwin = callback.currentImageWindow();
	if (!curwin)
	{
        visiocyte_msg("You don't have any image open in the main window.");
		return;
	}
    
    Image4DSimple* p4DImage = callback.getImage(curwin);
	if (!p4DImage)
	{
		visiocyte_msg("The image pointer is invalid. Ensure your data is valid and try again!");
		return;
    }
    
    double apercent = 0.01;
    unsigned char* data1d = p4DImage->getRawData();
    
    VISIOCYTELONG sc = p4DImage->getCDim();
    
    //input
    bool b_res;
    bool ok1;
    VISIOCYTELONG c=-1;
    
    if (menu_name==QString("Rescale all channels' intensity to [0,255] with 1% saturation"))
        c = -1;
    else if (menu_name==QString("Rescale only one channel"))
    {
        if (sc>1) //only need to ask if more than one channel
        {
            c = QInputDialog::getInteger(parent, "Channel",
                                         "Enter channel # (0, 1... for the 1st, 2nd... channels; -1 for all channels):",
                                         -1, -1, sc-1, 1, &ok1);
            if (!ok1)
                return;
        }
    }
    
    VISIOCYTELONG in_sz[4];
    in_sz[0] = p4DImage->getXDim();
    in_sz[1] = p4DImage->getYDim();
    in_sz[2] = p4DImage->getZDim();
    in_sz[3] = p4DImage->getCDim();
    
    //
    VISIOCYTELONG cb, ce, k;
    if (c<0) {cb=0; ce=sc-1;}
    else {cb = ce = c;}
    
    for (k=cb; k<=ce; k++)
    {
        switch(p4DImage->getDatatype())
        {
            case VISIOCYTE_UINT8:
                b_res = rc(data1d, in_sz, k, apercent);
                break;
            case VISIOCYTE_UINT16:
                b_res = rc((unsigned short int *)data1d, in_sz, k, apercent);
                break;
            case VISIOCYTE_FLOAT32:
                b_res = rc((float *)data1d, in_sz, k, apercent);
                break;
            default: b_res = false; visiocyte_msg("Right now this plugin supports only UINT8/UINT16/FLOAT32 data. Do nothing."); return;
        }
    }
    
    //display. there is no need to set the data, as it directly operate on the original data
    bool b_updateMinMax = true;
    callback.updateImageWindow(curwin, b_updateMinMax);
}



QStringList RescaleConvertPlugin::funclist() const
{
	return QStringList()
    <<tr("rescale")
    <<tr("rescale_and_convert_to_8bit")
    <<tr("rescale_shift_bits")
    <<tr("help");
}

bool RescaleConvertPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (func_name == tr("rescale")) 
	{
        bool b_convert2uint8 = false;
        return processImage(callback, input, output, b_convert2uint8);
	}
    else if (func_name == tr("rescale_and_convert_to_8bit")) 
	{
        bool b_convert2uint8 = true;
        return processImage(callback, input, output, b_convert2uint8);
	}
    else if (func_name == tr("rescale_shift_bits"))
    {
        if(input.size() < 1 || output.size() != 1)
            return false;

        VISIOCYTELONG bitnumber = 3;
        if (input.size()>=2)
        {
            vector<char*> paras = *(vector<char*> *)(input.at(1).p);
            if(paras.size() >= 1) bitnumber = atoi(paras.at(0));
        }

        char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
        char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
        cout<<"number of shifted bits = "<<bitnumber<<endl;
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

        VISIOCYTELONG tunits =  in_sz[0]* in_sz[1]* in_sz[2]* in_sz[3];
        for (VISIOCYTELONG i=0;i<tunits;i++)
        {
            data1d[i] = data1d[i] >> bitnumber << bitnumber;
        }

        simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)data1d, in_sz, datatype);
        if (data1d) {delete []data1d; data1d=0;}
        return true;

    }
	else if(func_name == tr("help"))
	{
		cout<<"Usage 1: visiocyte -x rescale_and_convert -f rescale -i <inimg_file> -o <outimg_file> -p <channel>"<<endl;
        cout<<"Usage 2: visiocyte -x rescale_and_convert -f rescale_and_convert_to_8bit -i <inimg_file> -o <outimg_file> -p <channel>"<<endl;
        cout<<"Usage 2: visiocyte -x rescale_and_convert -f rescale_shift_bits -i <inimg_file> -o <outimg_file> -p <shifted_bits> "<<endl;
		cout<<"channel                  the input channel value, default -1 (all channels) and start from 0 (first channel)"<<endl;
        cout<<"shifted_bits             number of shited bits, default 3 and start from 1 to 8"<<endl;
		cout<<"This plugin is developed by Hanchuan Peng as a simple example for rescaling an image (1% saturation) and convert to 8bit data."<<endl;
		cout<<endl;
		return true;
	}
}

bool processImage(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, bool b_convert2uint8 = false)
{
	if(input.size() < 1 || output.size() != 1) 
        return false;

	VISIOCYTELONG c = -1;
    if (input.size()>=2)
    {
        vector<char*> paras = *(vector<char*> *)(input.at(1).p);
        if(paras.size() >= 1) c = atoi(paras.at(0));
    }
    if (c<0)
    {
        visiocyte_msg("You explicitly or implicitly specified a channel number < 0, which means do it for all channels. \n",0);
    }
        
	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
    cout<<"channel = "<<c<<endl;
	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;

    double apercent = 0.01;

    bool b_res=false;
    unsigned char * data1d = 0;
    VISIOCYTELONG in_sz[4];
    VISIOCYTELONG cb, ce, k;
    
    int datatype;
    if(!simple_loadimage_wrapper(callback, inimg_file, data1d, in_sz, datatype))
    {
        cerr<<"load image "<<inimg_file<<" error!"<<endl;
        return false;
    }
    
    if (c>=in_sz[3])
    {
        visiocyte_msg("The specified channel is not valid. Do nothing.", 0);
        goto Label_exit;
    }
    
    if (c<0) {cb=0; ce=in_sz[3]-1;}
    else {cb = ce = c;}
    
    for (k=cb; k<=ce; k++)
    {
        printf("\n... processing [%d] channel now...\n", k);
        switch(datatype)
        {
            case 1: b_res = rc(data1d, in_sz, k, apercent); break;
            case 2: b_res = rc((unsigned short int *)data1d, in_sz, k, apercent); break;
            case 4: b_res = rc((float *)data1d, in_sz, k, apercent); break;
            default: b_res = false; visiocyte_msg("Right now this plugin supports only UINT8/UINT16/FLOAT32 data. Do nothing."); goto Label_exit;
        }
        
        if (!b_res) goto Label_exit;
    }
    
    if (b_convert2uint8)
    {
        void *tmpimg = (void *)data1d;
        if (!(b_res = convert_data_to_8bit(tmpimg, in_sz, datatype)))
            goto Label_exit;
        data1d = (unsigned char *)tmpimg;
        b_res = simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)data1d, in_sz, 1);
    }
    else
        b_res = simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)data1d, in_sz, datatype);
    
    //printf("b_res=[%s]\n", (b_res)?"true":"false");

Label_exit:
     if (data1d) {delete []data1d; data1d=0;}
     return b_res;
}


template <class T> bool rc(T* data1d, VISIOCYTELONG *sz, VISIOCYTELONG c, double apercent)
//apercent is the percentage of signal that should be upper/lower saturation
{
    if (!data1d || !sz || c<0 || c>=sz[3])
    {
        printf("problem: c=[%ld] sz=[%p] szc=[%ld], data1d=[%p]\n",c, sz, sz[3], data1d);
        return false;
    }
    
    VISIOCYTELONG channelsz = sz[0]*sz[1]*sz[2];
    T * outimg = (data1d + c*channelsz);

	if (apercent<0 || apercent>=0.5)
	{
		visiocyte_msg(QString("Your percentage parameter [%1] is wrong. Must be bwteen 0 and 0.5.\n").arg(apercent));
		return false;
	}
    
    VISIOCYTELONG i;
    double minval, maxval;
    minval = maxval = double(outimg[0]);
    for (i=1; i<channelsz; i++)
    {
        if (minval > outimg[i]) minval = outimg[i];
        else if (maxval < outimg[i]) maxval = outimg[i];
    }

	if (sizeof(T) == 4) //VISIOCYTE_FLOAT32, or any other 32bit data; for float data , first rescale it to [0, 4095]
	{
        if (!scaleintensity(data1d, sz, c, minval, maxval, double(0), double(4095)))
            return false;
        else
        {
            minval = 0;
            maxval = 4095;
        }
	}
    
	{
		VISIOCYTELONG maxvv = ceil(maxval+1); //this should be safe now as the potential FLOAT32 data has been rescaled
        
		qDebug() << "ch =" << c << " maxvv=" << maxvv;
        
		double *hist = 0;
		try
		{
			hist = new double [maxvv];
		}
		catch (...)
		{
			qDebug() << "fail to allocate"; return false;
			visiocyte_msg("Fail to allocate memory.\n");
			return false;
		}
        
		for (i=0;i<maxvv;i++)
		{
			hist[i] = 0;
		}
        
		//find the histogram
        for (i=0;i<channelsz;i++)
            hist[VISIOCYTELONG(outimg[i])] += 1;
        
		qDebug() << "Histogram computed.";
        
		//compute the CDF
		for (i=1;i<maxvv;i++)
		{
			hist[i] += hist[i-1];
		}
        
		for (i=0;i<maxvv;i++)
		{
			hist[i] /= hist[maxvv-1];
		}
        
		//now search for the intensity thresholds
		double lowerth, upperth; lowerth = upperth = 0;
		for (i=0;i<maxvv-1;i++) //not the most efficient method, but the code should be readable
		{
			if (hist[i]<apercent && hist[i+1]>apercent)
				lowerth = i;
			if (hist[i]<1-apercent && hist[i+1]>1-apercent)
				upperth = i;
		}
        
		visiocyte_msg(QString("channel=%1 lower th=%2 upper th=%3").arg(c).arg(lowerth).arg(upperth), 0);
        
		//real rescale of intensity
		scaleintensity(data1d, sz, c, lowerth, upperth, double(0), double(255));
        
		//free space
		if (hist) {delete []hist; hist=0;}
	}
    
	return true;
}

template <class T> bool scaleintensity(T *img, VISIOCYTELONG sz[4], VISIOCYTELONG channo, double lower_th, double higher_th, double target_min, double target_max) 
//map the value linear from [lower_th, higher_th] to [target_min, target_max]
{
    if (!img || !sz || sz[0]<=0 || sz[1]<=0 || sz[2]<=0 || sz[3]<=0 || channo>=sz[3]) //channo < 0 is reserved to mean all channels
	{
		visiocyte_msg("Invalid parameters in scaleintensity();\n");
		return false;
	}
    
	double t;
	if (lower_th>higher_th) {t=lower_th; lower_th=higher_th; higher_th=t;}
	if (target_min>target_max) {t=target_min; target_min=target_max; target_max=t;}
    
	double rate = (higher_th==lower_th) ? 1 : (target_max-target_min)/(higher_th-lower_th); //if the two th vals equal, then later-on t-lower_th will be 0 anyway
    
	VISIOCYTELONG i,j,k,c;
    
	VISIOCYTELONG channelPageSize = sz[0]*sz[1]*sz[2];
	switch ( sizeof(T) ) 
	{
		case 1:
        case 2:
        case 4:
			for (c=0;c<sz[3];c++)
			{
				if (channo>=0 && c!=channo)
					continue;
                T *data = img + c*channelPageSize;
				for (i=0;i<channelPageSize;i++)
                {
                    t = data[i];
                    if (t>higher_th) t=higher_th;
                    else if (t<lower_th) t=lower_th;
                    data[i] = (T)((t - lower_th)*rate + target_min);
                }
			}
			break;
            
		default:
			visiocyte_msg("invalid datatype in scaleintensity();\n", 0);
			return false;
	}
    
	return true;
}


