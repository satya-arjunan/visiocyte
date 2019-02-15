/* dt.cpp
 * distance transform of an image stack
 * revised from Fuhui Long's dt versions
 * 2010-04-16: by Hanchuan Peng
 */

// Adapted and upgraded to add dofunc() by Jianlong Zhou, 2012-04-11


#include <iostream>
#include "dt.h"
#include "FL_bwdist.h"
#include "visiocyte_message.h"

#include "stackutil.h"
#include "my_surf_objs.h"

using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(dt, DtPlugin);

void dtimg(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);
bool dtimg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

//plugin funcs
const QString title = "Distance transform";
QStringList DtPlugin::menulist() const
{
    return QStringList()
		<< tr("3D")
		<< tr("2D (for all individual Z-sections)")
          << tr("About this plugin");
}

void DtPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("3D"))
    {
    	dtimg(callback, parent, 1); //1 - is the 3D DT
    }
	else if (menu_name == tr("2D (for all individual Z-sections)"))
	{
    	dtimg(callback, parent, 2); //2 - is the within-XY plane DT
	}
    else if (menu_name == tr("About this plugin"))
	{
		QMessageBox::information(parent, "Version info",
               QString("Distance Transform Plugin %1 (2010-04-16). (Hanchuan Peng Lab, Janelia Research Farm Campus, HHMI)"
                       ).arg(getPluginVersion()));
	}
}




QStringList DtPlugin::funclist() const
{
	return QStringList()
		<<tr("dt")
		<<tr("help");
}


bool DtPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     if (func_name == tr("dt"))
	{
          return dtimg(callback, input, output);
	}
	else if(func_name == tr("help"))
	{
        cout<<"Usage : visiocyte -x dll_name -f dt -i <inimg_file> -o <outimg_file> -p <method> <channel> <first> <rescale> <marker_file> "<<endl;
		cout<<endl;
		cout<<"method       1: 3D, 2: 2D (for all individual Z-sections), default 1 "<<endl;
		cout<<"channel      channel number to do dt (first channel is 1), default 1"<<endl;
		cout<<"first        1: use the 1st marker's intensity to define the 'img background'"<<endl;
          cout<<"             0: use intensity 0 to indicate background), default 0"<<endl;
		cout<<"rescale      Rescale output image intensity to [0,255], 1: rescale, 0: not rescale, default 0"<<endl;
          cout<<"marker_file  marker file name if 'first' is set to 1" <<endl;
		cout<<endl;
        cout<<"e.g. visiocyte -x dll_name (need replacement) -f dt -i input.raw -o output.raw -p 1 1 0 0 mymarker.marker"<<endl;
		cout<<endl;
		return true;
	}

}

bool dtimg(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
     cout<<"Welcome to Distance transform"<<endl;
     if(input.size()<1 || output.size() != 1) return false;

	unsigned int method_code=1, channel=1, b_use_1stmarker=0, b_rescale=0;
     char * marker_file = 0;
     if (input.size()>=2)
     {
          vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
          if(paras.size() >= 1) method_code = atoi(paras.at(0));
          if(paras.size() >= 2) channel = atoi(paras.at(1));
          if(paras.size() >= 3) b_use_1stmarker = atoi(paras.at(2));
          if(paras.size() >= 4) b_rescale = atoi(paras.at(3));
          if(b_use_1stmarker)
          {
               // marker file name
               marker_file = paras.at(4);
          }
	}

     if (method_code!=1 && method_code!=2)
     {
         visiocyte_msg("Invalid DT method code. It must be either 1 or 2.");
         return false;
     }


	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
	cout<<"method_code = "<<method_code<<endl;
     cout<<"channel = "<<channel<<endl;
	cout<<"b_use_1stmarker = "<<b_use_1stmarker<<endl;
     cout<<"b_rescale = "<<b_rescale<<endl;
     if (marker_file) cout<<"marker_file = "<<marker_file<<endl;
    cout<<"inimg_file = ["<<inimg_file<<"]"<<endl;
    cout<<"outimg_file = ["<<outimg_file<<"]"<<endl;

     unsigned int ch = channel-1; // for channel starting from 0.

     Image4DSimple *subject = callback.loadImage(inimg_file);
     if(!subject || !subject->valid())
     {
          visiocyte_msg("Fail to load the input image.");
          if (subject) {delete subject; subject=0;}
          return false;
     }
     if(subject->getDatatype() != VISIOCYTE_UINT8)
     {
          printf("\nError: The program only supports UINT8 datatype.\n");
          if (subject) {delete subject; subject=0;}
          return false;
     }

     Image4DProxy<Image4DSimple> pSub(subject);
     // do dt

     VISIOCYTELONG sz0 = subject->getXDim();
     VISIOCYTELONG sz1 = subject->getYDim();
     VISIOCYTELONG sz2 = subject->getZDim();
     VISIOCYTELONG sz3 = subject->getCDim();
	VISIOCYTELONG pagesz_sub = sz0*sz1*sz2;

     // read marker file
     vector<MyMarker*> list_landmark_sub;
	VISIOCYTELONG markerx, markery, markerz;
     if (b_use_1stmarker)
     {
          if( !readMarker_file(marker_file, list_landmark_sub) )
          {
               return false;
          }
          else
          {
               if(list_landmark_sub.size()<1)
               {
                    b_use_1stmarker = false;
               }
               else
               {
                    markerx = VISIOCYTELONG(list_landmark_sub.at(0)->x)-1; if (markerx<0 || markerx>=sz0) b_use_1stmarker = false;
                    markery = VISIOCYTELONG(list_landmark_sub.at(0)->y)-1; if (markery<0 || markery>=sz1) b_use_1stmarker = false;
                    markerz = VISIOCYTELONG(list_landmark_sub.at(0)->z)-1; if (markerz<0 || markerz>=sz2) b_use_1stmarker = false;
               }
          }
     }
	unsigned char tt = (b_use_1stmarker) ? *(pSub.at(markerx, markery, markerz, ch)) : 0;

	//----------------------------------------------------------------------------------------------------------------------------------
	VISIOCYTELONG channelsz = sz0*sz1*sz2;

	VISIOCYTELONG *pDist = 0;
	VISIOCYTELONG *pLabel = 0;
	unsigned char *pData = 0;

	try
	{
		pDist = new VISIOCYTELONG [channelsz];
		pLabel = new VISIOCYTELONG [channelsz];
		pData = new unsigned char [channelsz];
	}
	catch (...)
	{
		visiocyte_msg("Fail to allocate memory in Distance Transform.");
		if (pDist) {delete []pDist; pDist=0;}
		if (pLabel) {delete []pLabel; pLabel=0;}
		if (pData) {delete []pData; pData=0;}
		return false;
	}

	unsigned char * pSubtmp = pSub.begin();
	for(VISIOCYTELONG i = 0; i < channelsz;  i++)
	{
		pData[i] = (pSubtmp[i]==tt) ? 0 : 1;
	}

	// dist transform
	VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
	if (method_code==1)
		dt3d_binary(pData, pDist, pLabel, sz_data, 1);
    else // (method_code==2)
	{
		VISIOCYTELONG pagesz = sz0*sz1;
		for (VISIOCYTELONG kk=0;kk<sz2;kk++)
			dt2d_binary(pData+kk*pagesz, pDist+kk*pagesz, pLabel+kk*pagesz, sz_data, 1);
	}

	VISIOCYTELONG maxtest=0, mintest=INF;
	VISIOCYTELONG *p=pDist, *pend=pDist+channelsz;
	for(p=pDist; p<pend; ++p)
	{
		if(*p > maxtest) maxtest = *p;
		else if (*p < mintest) mintest = *p;
	}
	maxtest -= mintest;

	if (b_rescale)
	{
		if(maxtest)
		{
			for(VISIOCYTELONG i=0; i<channelsz; i++)
				pData[i] = (unsigned char)(255.0*(pDist[i]-mintest)/maxtest);
		}
		else
		{
			for(VISIOCYTELONG i=0; i<channelsz; i++)
				pData[i] = 0;
		}
	}
	else
	{
		for(VISIOCYTELONG i=0; i<channelsz; i++)
			pData[i] = (unsigned char)(pDist[i]);
	}

     // save image
    Image4DSimple outimg;
    outimg.setData(pData, sz0, sz1, sz2, 1, VISIOCYTE_UINT8);

     callback.saveImage(&outimg, outimg_file);

	if (pDist) {delete []pDist; pDist=0;}
	if (pLabel) {delete []pLabel; pLabel=0;}
    if (subject) {delete subject; subject=0;}

    //pData will be freed through outimg

    visiocyte_msg("Finish running and file saving.", 0);
     return true;
}





void dtimg(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code)
{
	visiocytehandle curwin = callback.currentImageWindow();
	if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}

	if (method_code!=1 && method_code!=2)
	{
		visiocyte_msg("Invalid DT method code. You should never see this message. Report this bug to the developer");
		return;
	}

	DtDialog dialog(callback, parent);
	if (dialog.exec()!=QDialog::Accepted)
		return;

	int ch = dialog.ch;

	int start_t = clock(); // record time point

	bool b_use_1stmarker = dialog.b_use_1stmarker;
	bool b_rescale = dialog.b_rescale;

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

	Image4DProxy<Image4DSimple> pSub(subject);

	VISIOCYTELONG sz0 = subject->getXDim();
    VISIOCYTELONG sz1 = subject->getYDim();
    VISIOCYTELONG sz2 = subject->getZDim();
	VISIOCYTELONG sz3 = subject->getCDim();
	VISIOCYTELONG pagesz_sub = sz0*sz1*sz2;

	//also get the landmark from the subject
	LandmarkList list_landmark_sub=callback.getLandmark(curwin);
	VISIOCYTELONG markerx, markery, markerz;
	if (b_use_1stmarker)
	{
		if(list_landmark_sub.size()<1)
		{
			b_use_1stmarker = false;
		}
		else
		{
			markerx = VISIOCYTELONG(list_landmark_sub.at(0).x)-1; if (markerx<0 || markerx>=sz0) b_use_1stmarker = false;
			markery = VISIOCYTELONG(list_landmark_sub.at(0).y)-1; if (markery<0 || markery>=sz1) b_use_1stmarker = false;
			markerz = VISIOCYTELONG(list_landmark_sub.at(0).z)-1; if (markerz<0 || markerz>=sz2) b_use_1stmarker = false;
		}
	}
	unsigned char tt = (b_use_1stmarker) ? *(pSub.at(markerx, markery, markerz, ch)) : 0;

	//----------------------------------------------------------------------------------------------------------------------------------
	VISIOCYTELONG channelsz = sz0*sz1*sz2;

	VISIOCYTELONG *pDist = 0;
	VISIOCYTELONG *pLabel = 0;
	unsigned char *pData = 0;

	try
	{
		pDist = new VISIOCYTELONG [channelsz];
		pLabel = new VISIOCYTELONG [channelsz];
		pData = new unsigned char [channelsz];
	}
	catch (...)
	{
		visiocyte_msg("Fail to allocate memory in Distance Transform.");
		if (pDist) {delete []pDist; pDist=0;}
		if (pLabel) {delete []pLabel; pLabel=0;}
		if (pData) {delete []pData; pData=0;}
		return;
	}

	unsigned char * pSubtmp = pSub.begin();
	for(VISIOCYTELONG i = 0; i < channelsz;  i++)
	{
		pData[i] = (pSubtmp[i]==tt) ? 0 : 1;
	}

	// dist transform
	VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
	if (method_code==1)
		dt3d_binary(pData, pDist, pLabel, sz_data, 1);
	else if (method_code==2)
	{
		VISIOCYTELONG pagesz = sz0*sz1;
		for (VISIOCYTELONG kk=0;kk<sz2;kk++)
			dt2d_binary(pData+kk*pagesz, pDist+kk*pagesz, pLabel+kk*pagesz, sz_data, 1);
	}
	else
	{
		visiocyte_msg("Invalid DT method code. You should never see this message. Report this bug to the developer");
		return;
	}

	VISIOCYTELONG maxtest=0, mintest=INF;
	VISIOCYTELONG *p=pDist, *pend=pDist+channelsz;
	for(p=pDist; p<pend; ++p)
	{
		if(*p > maxtest) maxtest = *p;
		else if (*p < mintest) mintest = *p;
	}
	maxtest -= mintest;

	if (b_rescale)
	{
		if(maxtest)
		{
			for(VISIOCYTELONG i=0; i<channelsz; i++)
				pData[i] = (unsigned char)(255.0*(pDist[i]-mintest)/maxtest);
		}
		else
		{
			for(VISIOCYTELONG i=0; i<channelsz; i++)
				pData[i] = 0;
		}
	}
	else
	{
		for(VISIOCYTELONG i=0; i<channelsz; i++)
			pData[i] = (unsigned char)(pDist[i]);
	}

	if (pDist) {delete []pDist; pDist=0;}
	if (pLabel) {delete []pLabel; pLabel=0;}

	//----------------------------------------------------------------------------------------------------------------------------------

	int end_t = clock();
	printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);

	Image4DSimple p4DImage;
	p4DImage.setData((unsigned char*)pData, sz0, sz1, sz2, 1, subject->getDatatype());

	visiocytehandle newwin;
	if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
		newwin = callback.currentImageWindow();
	else
		newwin = callback.newImageWindow();

	callback.setImage(newwin, &p4DImage);
	callback.setImageName(newwin, QString("distance transformed image"));
	callback.updateImageWindow(newwin);

//	setPluginOutputAndDisplayUsingGlobalSetting(pData, sz0, sz1, sz2, sz3, callback);
}


