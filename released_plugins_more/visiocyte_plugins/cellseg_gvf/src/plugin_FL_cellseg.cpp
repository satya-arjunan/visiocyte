/* plugin_FL_cellseg.cpp
 * a plugin version of the original VISIOCYTE cellseg module function based on Fuhui's watershed code
 * by Hanchuan Peng, 2011-02-23
 */


#include "visiocyte_message.h"
#include "volimg_proc.h"

#include "dialog_watershed_para.h"

#include "FL_cellSegmentation3D.cpp"

#include "plugin_FL_cellseg.h"

#define USHORTINT16 unsigned short int

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(plugin_FL_cellseg, FLCellSegPlugin);

void FL_cellseg(VISIOCYTEPluginCallback2 &callback, QWidget *parent);

//plugin funcs
const QString title = "Segment image objects";
QStringList FLCellSegPlugin::menulist() const
{
    return QStringList() 
        << tr("Segment all image objects (adaptive thresholding followed by watershed)")
		<< tr("about")
	;
}

void FLCellSegPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("Segment all image objects (adaptive thresholding followed by watershed)"))
    {
    	FL_cellseg(callback, parent); 
    }
	else if (menu_name == tr("about"))
	{
        visiocyte_msg(QString("This plugin uses Fuhui Long's cell segmentation program to detect and label 3D image objects. (version %1)").arg(getPluginVersion()));
	}
}

void FL_cellseg(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	visiocytehandle curwin = callback.currentImageWindow();
	if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}
	
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

	VISIOCYTELONG sz0 = subject->getXDim();
    VISIOCYTELONG sz1 = subject->getYDim();
    VISIOCYTELONG sz2 = subject->getZDim();
	VISIOCYTELONG sz3 = subject->getCDim();
	
	Image4DProxy<Image4DSimple> pSub(subject);
	
	
	//----------------------------------------------------------------------------------------------------------------------------------
	VISIOCYTELONG channelsz = sz0*sz1*sz2;

	float *pLabel = 0;
	unsigned char *pData = 0;

	
	//get the segmentation parameters
	segParameter segpara;
	dialog_watershed_para *p_mydlg=0;
	if (!p_mydlg) p_mydlg = new dialog_watershed_para(&segpara, subject);
	int res = p_mydlg->exec();
	if (res!=QDialog::Accepted)
		return;
	else
		p_mydlg->fetchData(&segpara);
	if (p_mydlg) {delete p_mydlg; p_mydlg=0;}
	

	// now allocate memory and do computation
	int start_t = clock();
	
	Vol3DSimple <unsigned char> * tmp_inimg = 0;
	Vol3DSimple <USHORTINT16> * tmp_outimg = 0;
	
	try
	{
		tmp_inimg = new Vol3DSimple <unsigned char> (sz0, sz1, sz2);
		tmp_outimg = new Vol3DSimple <USHORTINT16> (sz0, sz1, sz2);
	}
	catch (...)
	{
		visiocyte_msg("Unable to allocate memory for processing. Do nothing.");
		if (tmp_inimg) {delete tmp_inimg; tmp_inimg=0;}
		if (tmp_outimg) {delete tmp_outimg; tmp_outimg=0;}
		return;
	}
	
	//do computation
	memcpy((void *)tmp_inimg->getData1dHandle(), (void *)subject->getRawDataAtChannel(segpara.channelNo), sz0*sz1*sz2);
	
	bool b_res = FL_cellseg(tmp_inimg, tmp_outimg, segpara);

	if (tmp_inimg) {delete tmp_inimg; tmp_inimg=0;} //free the space immediately for better use of memory
	if (!b_res)
	{
		visiocyte_msg("Fail to do the cell segmentation using FL_cellseg().\n");
	}
	else
	{
		VISIOCYTELONG new_sz0 = tmp_outimg->sz0();
		VISIOCYTELONG new_sz1 = tmp_outimg->sz1();
		VISIOCYTELONG new_sz2 = tmp_outimg->sz2();
		VISIOCYTELONG new_sz3 = 1;
		VISIOCYTELONG tunits = new_sz0*new_sz1*new_sz2*new_sz3;
		USHORTINT16 * outvol1d = new USHORTINT16 [tunits];
		
		USHORTINT16 * tmpImg_d1d = (USHORTINT16 *)(tmp_outimg->getData1dHandle());
		
		memcpy((void *)outvol1d, (void *)tmp_outimg->getData1dHandle(), tunits*sizeof(USHORTINT16));
		if (tmp_outimg) {delete tmp_outimg; tmp_outimg=0;} //free the space immediately for better use of memory
		
		Image4DSimple p4DImage;
		p4DImage.setData((unsigned char*)outvol1d, sz0, sz1, sz2, 1, VISIOCYTE_UINT16);
		
		visiocytehandle newwin = callback.newImageWindow();
		callback.setImage(newwin, &p4DImage);
		callback.setImageName(newwin, QString("Segmented Image"));
		callback.updateImageWindow(newwin);
	}
		

	//----------------------------------------------------------------------------------------------------------------------------------

	int end_t = clock();
	printf("time eclapse %d s for labeling objects!\n", (end_t-start_t)/1000000);
}

