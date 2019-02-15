/* plugin_watershed.cpp
 * watershed plugin based on Fuhui Long's watershed code
 * by Hanchuan Peng, 2011-02-21
 */


#include "FL_watershed_vs.h"
#include "FL_bwlabel2D3D.h"
#include "visiocyte_message.h"
#include "volimg_proc.h"

#include "plugin_watershed.h"

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(plugin_watershed, WatershedVSPlugin);

void watershed_vs(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);

//plugin funcs
const QString title = "Watershed segment image objects";
QStringList WatershedVSPlugin::menulist() const
{
    return QStringList() 
		<< tr("3D")
		<< tr("2D (for all individual Z-sections)")
		<< tr("about")
	;
}

void WatershedVSPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("3D"))
    {
    	watershed_vs(callback, parent, 3); //3 - 3D
    }
	else if (menu_name == tr("2D (for all individual Z-sections)"))
	{
    	watershed_vs(callback, parent, 2); //2 - 2D
	}
	else if (menu_name == tr("about"))
	{
    	visiocyte_msg(QString("This plugin uses watershed segmentation to label all image objects. Developed by Hanchuan Peng, by wrapping Fuhui Long's original code. (version %1)").arg(getPluginVersion())); 
	}
}

void watershed_vs(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code)
{
	visiocytehandle curwin = callback.currentImageWindow();
	if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}
	
	if (method_code!=3 && method_code!=2)
	{
		visiocyte_msg("Invalid watershed method code. You should never see this message. Report this bug to the developer");
		return;
	}
		
	LabelImgObjectParaDialog dialog(callback, parent);
	if (!dialog.image)
		return;
	if (dialog.exec()!=QDialog::Accepted)
		return;
	
	VISIOCYTELONG ch = dialog.ch;
	VISIOCYTELONG th_idx = dialog.th_idx;
	double tt = dialog.thresh;
	VISIOCYTELONG volsz_thres = (dialog.b_filtersmallobjs) ? dialog.volsz : 0; //the threshold to filter out small objects

	int start_t = clock(); // record time 
	
	Image4DSimple* subject = callback.getImage(curwin);
	QString m_InputFileName = callback.getImageName(curwin);
	
	if (!subject)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}
	if (subject->getDatatype()!=VISIOCYTE_UINT8)
	{
		QMessageBox::information(0, title, QObject::tr("This demo program only supports 8-bit data. Your current image data type is not supported."));
		return;
	}

	if (th_idx==0 || th_idx==1)
	{
		double mm, vv;
		mean_and_std(subject->getRawDataAtChannel(ch), subject->getTotalUnitNumberPerChannel(), mm, vv);
		tt = (th_idx == 0) ? mm : mm+vv;
		visiocyte_msg(QString("in fast image object labeling: ch=%1 mean=%2 std=%2").arg(ch).arg(mm).arg(vv), 0);
	}	
	
	Image4DProxy<Image4DSimple> pSub(subject);
	
	VISIOCYTELONG sz0 = subject->getXDim();
    VISIOCYTELONG sz1 = subject->getYDim();
    VISIOCYTELONG sz2 = subject->getZDim();
	VISIOCYTELONG sz3 = subject->getCDim();
	
	//----------------------------------------------------------------------------------------------------------------------------------
	VISIOCYTELONG channelsz = sz0*sz1*sz2;

	float *pLabel = 0;
	unsigned char *pData = 0;
	
	try
	{
		//pLabel = new float [channelsz];
		pData = new unsigned char [channelsz]; 
	}
	catch (...)
	{
		visiocyte_msg("Fail to allocate memory in Watershed segmentation.");
		if (pLabel) {delete []pLabel; pLabel=0;}
		if (pData) {delete []pData; pData=0;}
		return;
	}
	
    VISIOCYTELONG i;
	unsigned char * pSubtmp = pSub.begin();
    for(i = 0; i < channelsz;  i++)
	{
		pData[i] = (pSubtmp[i]<=tt) ? 0 : 1;
	}
	
	// dist transform
	VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
	VISIOCYTELONG nobjs=0;
	if (method_code==3)
	{
		VISIOCYTELONG nh_code=26; //6,18,or 26
		watershed_vs(pData, pLabel, sz_data, 3, nh_code);
	}
	else if (method_code==2)
	{
		VISIOCYTELONG nh_code=8; //4 or 8
		watershed_vs(pData, pLabel, sz_data, 2, nh_code);
	}
	else
	{
		visiocyte_msg("Invalid BWLabelN method code. You should never see this message. Report this bug to the developer");
		return;
	}
	
	if (pData) {delete []pData; pData=0;}
    if (!pLabel)
    {
        visiocyte_msg("The label field of the image objects is not valid. Quit.");
        return;
    }
		
    //seems nobjs has not been set, thus the filtering code should has a problem
    for (i=1, nobjs = pLabel[0]; i<channelsz; i++)
        if (VISIOCYTELONG(pLabel[i])>nobjs) {nobjs = pLabel[i];}

    if (nobjs<=0)
    {
        visiocyte_msg("No valid image object was detected using the p[arameters you specified. Quit.");
        if (pLabel) {delete []pLabel; pLabel=0;}
        return;
    }
    else
    {
        visiocyte_msg(QString("Watershed labeling finds at most %1 objects").arg(nobjs));
    }

    if (volsz_thres>0  && nobjs>0) //filter out small objects
	{
		try {
			float * hh = new float [nobjs];
			float * mapval = new float [nobjs];
			
			VISIOCYTELONG j;
			for (j=0;j<nobjs;j++) {hh[j]=0; mapval[j]=j;} //of course, 0 also map to 0!
			for (j=0;j<channelsz;j++)
			{
				//pLabel[j]--; //it seems Fuhui's data is 1-based, so subtract 1. Is this correct?
				hh[VISIOCYTELONG(pLabel[j])]++;
			}
			VISIOCYTELONG k=0;
            for (j=1;j<nobjs;j++) //start from 1 as the value is the background!
			{
				if (hh[j]<volsz_thres)
				{
					mapval[j]=0; //if less than a thres, then map to 0, which is background
				}
				else
				{
					printf("Obj [%ld] = [%ld]\n", VISIOCYTELONG(j), VISIOCYTELONG(hh[j]));
					k++;
					mapval[j] = k; //otherwise map to a continous label-value
                }
			}
			for (j=0;j<channelsz;j++)
				pLabel[j] = mapval[VISIOCYTELONG(pLabel[j])];

			if (hh) {delete []hh; hh=0;}
			if (mapval) {delete []mapval; mapval=0;}
		}
		catch (...) {
			visiocyte_msg("Unable to allocate memory to filter small objects. Thus skip it.");
		}
	}
	
	//----------------------------------------------------------------------------------------------------------------------------------

	int end_t = clock();
	printf("time eclapse %d s for labeling objects!\n", (end_t-start_t)/1000000);
	
	Image4DSimple p4DImage;
	p4DImage.setData((unsigned char*)pLabel, sz0, sz1, sz2, 1, VISIOCYTE_FLOAT32);
	
	visiocytehandle newwin = callback.newImageWindow();
	callback.setImage(newwin, &p4DImage);
	callback.setImageName(newwin, QString("Object-Labeled Image"));
	callback.updateImageWindow(newwin);
}
