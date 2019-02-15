/*
 * Copyright (c)2006-2010  Hanchuan Peng (Janelia Farm, Howard Hughes Medical Institute).
 * All rights reserved.
 */


/************
                                            ********* LICENSE NOTICE ************

This folder contains all source codes for the VISIOCYTE project, which is subject to the following conditions if you want to use it.

You will ***have to agree*** the following terms, *before* downloading/using/running/editing/changing any portion of codes in this package.

1. This package is free for non-profit research, but needs a special license for any commercial purpose. Please contact Hanchuan Peng for details.

2. You agree to appropriately cite this work in your related studies and publications.

Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) “VISIOCYTE enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,” Nature Biotechnology, Vol. 28, No. 4, pp. 348-353, DOI: 10.1038/nbt.1612. ( http://penglab.janelia.org/papersall/docpdf/2010_NBT_VISIOCYTE.pdf )

Peng, H, Ruan, Z., Atasoy, D., and Sternson, S. (2010) “Automatic reconstruction of 3D neuron structures using a graph-augmented deformable model,” Bioinformatics, Vol. 26, pp. i38-i46, 2010. ( http://penglab.janelia.org/papersall/docpdf/2010_Bioinfo_GD_ISMB2010.pdf )

3. This software is provided by the copyright holders (Hanchuan Peng), Howard Hughes Medical Institute, Janelia Farm Research Campus, and contributors "as is" and any express or implied warranties, including, but not limited to, any implied warranties of merchantability, non-infringement, or fitness for a particular purpose are disclaimed. In no event shall the copyright owner, Howard Hughes Medical Institute, Janelia Farm Research Campus, or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; reasonable royalties; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

4. Neither the name of the Howard Hughes Medical Institute, Janelia Farm Research Campus, nor Hanchuan Peng, may be used to endorse or promote products derived from this software without specific prior written permission.

*************/




/*****************************************************************************************************
*
* VISIOCYTE's plug-in interface of QObject
*
* Copyright: Hanchuan Peng (Howard Hughes Medical Institute, Janelia Farm Research Campus).
* The License Information and User Agreement should be seen at http://penglab.janelia.org/proj/visiocyte .
*
* 2009-Aug-21
* 2010-06-01
* 2010-06-02: add setPluginOutputAndDisplayUsingGlobalSetting()
* 2010-06-03: add two others function to extract and assemble the results of different channels
* 2010-08-03: add pushObjectIn3DWindow, pushImageIn3DWindow, pushTimepointIn3DWindow, open/close(ROI)3DWindow
* 2010-08-04: "com.janelia.visiocyte.VISIOCYTEPluginInterface/1.1"
* 2012-04: add a few more functions to allow direct access of 3D surface objects
* 2012-04-12: extend the updateImageWindow() function to allow it also update the min max value is asked to do so 
* ********************************************************************************************************
*/

#ifndef _VISIOCYTE_INTERFACE_H_
#define _VISIOCYTE_INTERFACE_H_

#include <QtCore>

class VISIOCYTEPluginInterface2_1;
class VISIOCYTEPluginInterface2;
class VISIOCYTEPluginInterface;
class VISIOCYTESingleImageInterface2_1;
class VISIOCYTESingleImageInterface;
QT_BEGIN_NAMESPACE
	Q_DECLARE_INTERFACE(VISIOCYTEPluginInterface2_1, "com.janelia.visiocyte.VISIOCYTEPluginInterface/2.1");
	Q_DECLARE_INTERFACE(VISIOCYTEPluginInterface2, "com.janelia.visiocyte.VISIOCYTEPluginInterface/2.0");
	Q_DECLARE_INTERFACE(VISIOCYTEPluginInterface, "com.janelia.visiocyte.VISIOCYTEPluginInterface/1.1");
	Q_DECLARE_INTERFACE(VISIOCYTESingleImageInterface2_1, "com.janelia.visiocyte.VISIOCYTESingleImageInterface/2.1");
	Q_DECLARE_INTERFACE(VISIOCYTESingleImageInterface, "com.janelia.visiocyte.VISIOCYTESingleImageInterface/1.0");
QT_END_NAMESPACE

//==========================================================================================

#include "basic_4dimage.h"
#include "basic_surf_objs.h"
#include "basic_landmark.h"
#include "basic_view3d.h"
#include "basic_triview.h"
#include "visiocyte_global_preference.h"
#include "visiocyte_message.h"

struct VISIOCYTEPluginArgItem
{
	QString type;	void * p;
};
typedef QList<VISIOCYTEPluginArgItem> VISIOCYTEPluginArgList;

typedef QList<QPolygon>        ROIList;
typedef QList<LocationSimple>  LandmarkList;
//typedef QList<ImageMarker>     MarkerList;
//typedef QList<LabelSurf>       LabelSurfList;
//typedef QList<NeuronSWC>       SWCList;
//typedef QList<CellAPO>         APOList;

typedef QList<visiocytehandle>       visiocytehandleList;

// this is the export interface of VISIOCYTE functions to plugin
class VISIOCYTEPluginCallback
{
public:
	virtual ~VISIOCYTEPluginCallback() {}
	virtual bool callPluginFunc(const QString & plugin_name, const QString & func_name,
			const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output) = 0;
    //virtual bool callPluginMenu(const QString & plugin_name, const QString & menu_name) = 0; //a bug caused in v2.823?

	virtual visiocytehandleList getImageWindowList() const = 0;
	virtual visiocytehandle currentImageWindow() = 0;
	virtual visiocytehandle curHiddenSelectedWindow() = 0; //by PHC, 20101009. curHiddenSelectedWindow may not be the *currentImageWindow* if the selection is done from a 3d viewer
	virtual visiocytehandle newImageWindow(QString name="new_image") = 0;
	virtual void updateImageWindow(visiocytehandle image_window, bool b_forceUpdateChannelMinMaxValues=false) = 0; //by PHC, 20120412

	virtual QString getImageName(visiocytehandle image_window) const = 0;
	virtual void setImageName(visiocytehandle image_window, QString name) = 0;

	virtual Image4DSimple * getImage(visiocytehandle image_window) = 0;
	virtual bool setImage(visiocytehandle image_window, Image4DSimple * image) = 0;

	//a special debug function. don't use this if you don't know how to use. by PHC, 100731
	virtual bool setImageTest(visiocytehandle image_window, Image4DSimple * image, unsigned char *a) = 0;

	virtual LandmarkList  getLandmark(visiocytehandle image_window) = 0;
	virtual bool setLandmark(visiocytehandle image_window, LandmarkList & landmark_list) = 0;

	virtual ROIList getROI(visiocytehandle image_window) = 0;
	virtual bool setROI(visiocytehandle image_window, ROIList & roi_list) = 0;

	virtual NeuronTree getSWC(visiocytehandle image_window) = 0;
	virtual bool setSWC(visiocytehandle image_window, NeuronTree & nt) = 0;

    virtual Image4DSimple * loadImage(char *filename) = 0;  //2013-08-09. two more functions for simplied calls to use Visiocyte's image loading and saving functions without linking to additional libs
    virtual Image4DSimple * loadImage(char *filename, VISIOCYTELONG zsliceno) = 0;  //2013-11-02. load one single slice from an image, useful when the image is large
    virtual bool saveImage(Image4DSimple * img, char *filename) = 0;

    virtual VISIOCYTE_GlobalSetting getGlobalSetting() = 0;
	virtual bool setGlobalSetting( VISIOCYTE_GlobalSetting & gs ) = 0;

// 1.1 functions
	virtual void open3DWindow(visiocytehandle image_window) = 0;
	virtual void close3DWindow(visiocytehandle image_window) = 0;
	virtual void openROI3DWindow(visiocytehandle image_window) = 0;
	virtual void closeROI3DWindow(visiocytehandle image_window) = 0;

	virtual void pushObjectIn3DWindow(visiocytehandle image_window) = 0;
	virtual void pushImageIn3DWindow(visiocytehandle image_window) = 0;
	virtual int pushTimepointIn3DWindow(visiocytehandle image_window, int timepoint) = 0;

	virtual bool screenShot3DWindow(visiocytehandle image_window, QString BMPfilename) = 0;
	virtual bool screenShotROI3DWindow(visiocytehandle image_window, QString BMPfilename) = 0;

};

//this is the major VISIOCYTE plugin interface, and will be enhanced continuously
class VISIOCYTEPluginInterface
{
public:
	virtual ~VISIOCYTEPluginInterface() {}

	virtual QStringList menulist() const = 0;
	virtual void domenu(const QString & menu_name, VISIOCYTEPluginCallback & visiocyte, QWidget * parent) = 0;

	virtual QStringList funclist() const = 0;
	virtual void dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,
												QWidget * parent) = 0;
};

//==========================================================================================

class TriviewControl; //added by PHC, 2010-Dec-08
class View3DControl;
class MainWindow;
class VisiocyteR_MainWindow;

struct DataLists_in_3dviewer
{
    QStringList swc_file_list;
    QStringList pointcloud_file_list;
    QString surface_file;
    QString labelfield_file;
    QString imgfile;
};


class VISIOCYTEPluginCallback2 : public VISIOCYTEPluginCallback
{
public:
	virtual ~VISIOCYTEPluginCallback2() {}

	virtual View3DControl * getView3DControl(visiocytehandle image_window) = 0;
	virtual View3DControl * getLocalView3DControl(visiocytehandle image_window) = 0;
	virtual TriviewControl * getTriviewControl(visiocytehandle image_window) = 0;
  
    //added PHC 20120406. add a main window handle, to allow access everything in Visiocyte
    
    virtual MainWindow * getVisiocyteMainWindow() = 0; 
    virtual QList <VisiocyteR_MainWindow *> getListAll3DViewers() = 0; 
    virtual VisiocyteR_MainWindow * find3DViewerByName(QString fileName) = 0; //the name can be partially matched    
    
    //added PHC 20120406 to allow uses to access the surface data objects in a 3D viewer, but based on a tri-view
	virtual QList <NeuronTree> * getHandleNeuronTrees_3DGlobalViewer(visiocytehandle image_window) = 0;
	virtual QList <CellAPO>    * getHandleAPOCellList_3DGlobalViewer(visiocytehandle image_window) = 0;        
    virtual QList <LabelSurf> getListLabelSurf_3DGlobalViewer(visiocytehandle image_window) = 0;
    virtual bool setListLabelSurf_3DGlobalViewer(visiocytehandle image_window, QList <LabelSurf> listLabelSurfinput) = 0;

    //added PHC 20120406 to allow uses to access the surface data objects in a 3D viewer
	virtual QList <NeuronTree> * getHandleNeuronTrees_Any3DViewer(VisiocyteR_MainWindow *w) = 0;
	virtual QList <CellAPO>    * getHandleAPOCellList_Any3DViewer(VisiocyteR_MainWindow *w) = 0;        
    virtual QList <LabelSurf> getListLabelSurf_Any3DViewer(VisiocyteR_MainWindow *w) = 0;
    virtual bool setListLabelSurf_Any3DViewer(VisiocyteR_MainWindow *w, QList <LabelSurf> listLabelSurfinput) = 0;    

    //added PHC 20130904 allow a plugin program to refresh and rescan all plugins //commented as it does not work. by PHC 20130904
    //virtual void refreshMainMenuPluginList() = 0;

    virtual View3DControl * getView3DControl_Any3DViewer(VisiocyteR_MainWindow *w) = 0;  //Dec. 02, 2013 by Zhi Zhou
    virtual void screenShot_Any3DViewer(VisiocyteR_MainWindow *w,QString filename) = 0; //Dec. 02, 2013 by Zhi Zhou

    virtual void update_3DViewer(VisiocyteR_MainWindow *w) = 0;//Dec, 9, 2013. PHC

    virtual void update_NeuronBoundingBox(VisiocyteR_MainWindow *w) = 0; //Oct. 08, 2014 by Hanbo Chen

    virtual LandmarkList * getHandleLandmarkList_Any3DViewer(VisiocyteR_MainWindow *w) = 0; //Oct. 16, 2014 by Hanbo Chen
    virtual bool setHandleLandmarkList_Any3DViewer(VisiocyteR_MainWindow *w, LandmarkList & landmark_list) = 0; //Aug. 2, 2017 by Zhi Zhou

    virtual DataLists_in_3dviewer fetch_3dviewer_datafilelist(QString name3dviewer) = 0; //20140122 a conveniece function to access the record of data in a 3D viewer

    // extend the plugin interface by allowing open a 3D surface file , PHC Feb 10, 2015
    virtual VisiocyteR_MainWindow * open3DViewerForSingleSurfaceFile(QString fileName) = 0;
    virtual VisiocyteR_MainWindow * open3DViewerForLinkerFile(QString fileName) = 0;
    virtual VisiocyteR_MainWindow * createEmpty3DViewer() = 0; //create a 3D viewer with no content
    virtual void setWindowDataTitle(VisiocyteR_MainWindow *w, QString title) = 0 ;
    virtual QString getWindowDataTitle(VisiocyteR_MainWindow *w)=0; //BRL add 2017.02.22
    virtual void moveWindow(VisiocyteR_MainWindow *w, int x, int y) = 0 ;
    virtual void resizeWindow(VisiocyteR_MainWindow *w, int x, int y) = 0 ;
    virtual void setHideDisplayControlButton(VisiocyteR_MainWindow *w)=0;
//    virtual void setResizeEvent(VisiocyteR_MainWindow *w, int x, int y)=0;

    //added TeraFly interface, functions are provided by Alessadnro Bria, the wrapper is provided by Zhi Zhou Aug. 23, 2017
    virtual NeuronTree getSWCTeraFly() = 0;
    virtual bool setSWCTeraFly(NeuronTree & nt) = 0;
    virtual LandmarkList  getLandmarkTeraFly() = 0;
    virtual bool setLandmarkTeraFly(LandmarkList & landmark_list) = 0;
    virtual QString getPathTeraFly()=0;
    virtual const Image4DSimple * getImageTeraFly() = 0;
    virtual QString versionTeraFly() = 0;

    virtual bool getDimTeraFly(const std::string & path, VISIOCYTELONG * & sz) = 0;
    virtual unsigned char* getSubVolumeTeraFly(const std::string & path, size_t x0, size_t x1, size_t y0, size_t y1, size_t z0, size_t z1) = 0;
    virtual void releaseOpenedVolumesTeraFly() = 0;

    virtual bool setImageTeraFly(size_t x, size_t y, size_t z) = 0;

	virtual void redrawEditInfo(int editInputNum) = 0;


#ifdef __ALLOW_VR_FUNCS__
    virtual void openVRWindow(VisiocyteR_MainWindow *w, bool bOnlineMode = false) = 0;
    virtual void openVRWindowV2(visiocytehandle image_window, bool bOnlineMode = false) = 0;
#endif


};

class VISIOCYTEPluginInterface2
{
public:
	virtual ~VISIOCYTEPluginInterface2() {}

	virtual QStringList menulist() const = 0;
	virtual void domenu(const QString & menu_name, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent) = 0;

	virtual QStringList funclist() const = 0;
	virtual bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,
												VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent) = 0;
};

//=============================================================================
// CMB Nov 23, 2010
// enhance plugin interface to support plugin version numbers
// to help VISIOCYTE automatically update itself.
// VISIOCYTEPluginInterface2_1 is just like VISIOCYTEPluginInterface2, plus the getPluginVersion() method.

class VISIOCYTEPluginInterface2_1 : public VISIOCYTEPluginInterface2
{
public:
        // As a particular plugin is improved,
        // this version number should monotonically increase
        virtual float getPluginVersion() const = 0;
};


//==========================================================================================

//a function for considering the global setting to extract the IDs of the channels for plugin processing
inline QList<VISIOCYTELONG> getChannelListForProcessingFromGlobalSetting( VISIOCYTELONG nc, VISIOCYTEPluginCallback & callback ) //nc is the # of channels in an image
{
	QList<VISIOCYTELONG> chlist;
	if ( nc<=0 )
	{
		visiocyte_msg(QString("Invalid # channels parameter to getChannelListForProcessingFromGlobalSetting()."));
		return chlist;
	}

	//get the list of channels for processing

	int chano_preference = callback.getGlobalSetting().iChannel_for_plugin;
	if ( chano_preference >= nc )
	{
		visiocyte_msg(QString("The global setting uses a channel id that is bigger than the # of channels of this image. Apply to processing to the last channel of this image."));
		chano_preference = nc-1;
	}

	if (chano_preference < 0)
	{
		for (VISIOCYTELONG i=0;i<nc;i++)
			chlist << i;
	}
	else {
		chlist << chano_preference;
	}

	return chlist;
}

//a function for considering the global setting to extract the DATA of the channels for plugin processing
inline QList<VISIOCYTE_Image3DBasic> getChannelDataForProcessingFromGlobalSetting( Image4DSimple * p, VISIOCYTEPluginCallback & callback)
{
	QList<VISIOCYTE_Image3DBasic> dlist;
	if ( !p || !p->valid() )
	{
		visiocyte_msg(QString("Invalid inputs to getChannelDataForProcessingFromGlobalSetting(). Don't output the plugin results.\n"));
		return dlist;
	}

	//get the list of channels for processing
	QList<VISIOCYTELONG> chlist = getChannelListForProcessingFromGlobalSetting( p->getCDim(), callback );
	for (VISIOCYTELONG i=0; i<chlist.size(); i++)
	{
		VISIOCYTE_Image3DBasic v;
		v.setData(p, chlist.at(i));
		dlist << v;
	}
	return dlist;
}


//a function for considering the global setting for data conversion
template <class T> bool setPluginOutputAndDisplayUsingGlobalSetting(T * pluginoutputimg1d, VISIOCYTELONG sz0, VISIOCYTELONG sz1, VISIOCYTELONG sz2, VISIOCYTELONG sz3, VISIOCYTEPluginCallback & callback, const char* imageName=0)
{
	if (!pluginoutputimg1d || sz0<=0 || sz1<=0 || sz2<=0 || sz3<=0 )
	{
		visiocyte_msg(QString("Invalid inputs to setPluginOutputAndDisplayUsingGlobalSetting(). Don't output the plugin results.\n"));
		return false;
	}

	VISIOCYTELONG totalunits = sz0*sz1*sz2*sz3;
	if (totalunits<=0)
	{
		visiocyte_msg(QString("Overflow of the *long* data type. Don't output the plugin results.\n"));
		return false;
	}

	VISIOCYTE_GlobalSetting gs = callback.getGlobalSetting();

	unsigned char * output1d = 0;
	VISIOCYTELONG i;

	if ( gs.b_plugin_outputImgRescale ) //rescale to [0, 255]
	{
		T mm = pluginoutputimg1d[0], MM = pluginoutputimg1d[0];
		for (i=0; i<totalunits; ++i)
		{
			if (pluginoutputimg1d[i]<mm)
				mm = pluginoutputimg1d[i];
			else if (pluginoutputimg1d[i]>MM)
				MM = pluginoutputimg1d[i];
		}

		if ( gs.b_plugin_outputImgConvert2UINT8 || mm==MM ) //if mm=MM, then no need to use float even the origianl data is float
		{
			output1d = new unsigned char [totalunits];
			if (mm==MM)
				for (i=0; i<totalunits; ++i)
					output1d[i] = 0;
			else
			{
				double w = 255.0/(double(MM)-double(mm));
				for (i=0; i<totalunits; ++i)
					output1d[i] = (unsigned char) ((double)pluginoutputimg1d[i] * w);
			}
		}
		else //not convert to unsigned char, but to float
		{
			float * output1d_float = new float [totalunits];
			output1d = (unsigned char *)output1d_float;

			double w = 255.0/(double(MM)-double(mm)); //MM must not equal mm now
			for (i=0; i<totalunits; ++i)
				output1d_float[i] = (float) (((double)pluginoutputimg1d[i]-double(mm)) * w);
		}
	}
	else //not rescale to [0, 255]
	{
		if ( gs.b_plugin_outputImgConvert2UINT8 )
		{
			output1d = new unsigned char [totalunits];
			for (i=0; i<totalunits; ++i)
				output1d[i] = (unsigned char)(pluginoutputimg1d[i]);
		}
		else //not convert to unsigned char, but to float
		{
			float * output1d_float = new float [totalunits];
			output1d = (unsigned char *)output1d_float;

			for (i=0; i<totalunits; ++i)
				output1d_float[i] = (float)(pluginoutputimg1d[i]);
		}
	}

	//now set up the output window and data

	Image4DSimple p4DImage;
	if ( gs.b_plugin_outputImgConvert2UINT8 )
	{
		p4DImage.setData(output1d, sz0, sz1, sz2, sz3, VISIOCYTE_UINT8);
	}
	else
	{
		p4DImage.setData(output1d, sz0, sz1, sz2, sz3, VISIOCYTE_FLOAT32);
	}

	visiocytehandle mywin = ( gs.b_plugin_dispResInNewWindow ) ? callback.newImageWindow() : callback.currentImageWindow();

	callback.setImage(mywin, &p4DImage);
        if(imageName != 0)
          {
            callback.setImageName(mywin, QString(imageName));
          }
        else
          {
            callback.setImageName(mywin, QString("plugin_output_image"));
          }
	callback.updateImageWindow(mywin);

	return true;
}

//a function for assembling output of filtered channels of images
inline bool assembleProcessedChannels2Image4DClass(QList<VISIOCYTE_Image3DBasic> & pd, VISIOCYTEPluginCallback & cb)
{
	VISIOCYTELONG i;
	for (i=0; i<pd.size(); i++)
	{
		if (!pd[i].data1d || pd[i].sz0<=0 || pd[i].sz1<=0 || pd[i].sz2<=0 || (pd[i].datatype!=VISIOCYTE_UINT8 && pd[i].datatype!=VISIOCYTE_UINT16 && pd[i].datatype!=VISIOCYTE_FLOAT32) )
		{
			visiocyte_msg(QString("The %1 channel of the input to assembleProcessedChannels2Image4DClass() is invalid. Don't output the plugin results.\n").arg(i));
			return false;
		}
		if (i>=1)
		{
			if (pd[i].sz0!=pd[i-1].sz0 || pd[i].sz1!=pd[i-1].sz1 || pd[i].sz2!=pd[i-1].sz2 || pd[i].datatype!=pd[i-1].datatype)
			{
				visiocyte_msg(QString("The %1 and %2 channels of the input have different sizes in assembleProcessedChannels2Image4DClass() is invalid. Don't output the plugin results.\n").arg(i).arg(i-1));
				return false;
			}
		}
	}
	VISIOCYTELONG mysz0 = pd[0].sz0, mysz1 = pd[0].sz1, mysz2 = pd[0].sz2, mysz3 = pd.size();
	ImagePixelType curdatatype = pd[0].datatype;
	VISIOCYTELONG nunitbytes = 1; if (curdatatype==VISIOCYTE_UINT16) nunitbytes=2; else if (curdatatype==VISIOCYTE_FLOAT32) nunitbytes=4;
	VISIOCYTELONG nchanbytes = mysz0*mysz1*mysz2*nunitbytes;
	unsigned char *pout = 0;

	try {
		pout = new unsigned char [nchanbytes * mysz3];
	}
	catch (...)
	{
		visiocyte_msg(QString("Fail to allocate a buffer memory for output of the plugin in assembleProcessedChannels2Image4DClass()."));
		return false;
	}

	//now copy data
	for (i=0;i<mysz3;i++)
	{
		unsigned char *psrc = pd[i].data1d;
		unsigned char *pdst = pout + nchanbytes * i;
		for (VISIOCYTELONG j=0; j<nchanbytes; j++)
			pdst[j] = psrc[j];
	}

	//now set VISIOCYTE display
	switch (curdatatype)
	{
		case VISIOCYTE_UINT8:   return setPluginOutputAndDisplayUsingGlobalSetting(pout, mysz0, mysz1, mysz2, mysz3, cb); break;
		case VISIOCYTE_UINT16:  return setPluginOutputAndDisplayUsingGlobalSetting((unsigned short int *)pout, mysz0, mysz1, mysz2, mysz3, cb); break;
		case VISIOCYTE_FLOAT32: return setPluginOutputAndDisplayUsingGlobalSetting((float *)pout, mysz0, mysz1, mysz2, mysz3, cb); break;
		default: return false;
	}

	if (pout) {delete []pout; pout=0;} //100815 per Aurelien & Luis suggestion.

	return false;
}


// obsolete interface for manipulating only one image at a time.
class VISIOCYTESingleImageInterface
{
public:
    virtual ~VISIOCYTESingleImageInterface() {}

    virtual QStringList menulist() const = 0;
    virtual void processImage(const QString & menu_name, Image4DSimple * image, QWidget * parent) = 0;
};

// And version aware interface for auto updating
class VISIOCYTESingleImageInterface2_1 : public VISIOCYTESingleImageInterface
{
public:
    // As a particular plugin is improved,
    // this version number should monotonically increase
    virtual float getPluginVersion() const = 0;
};


//some additional simple inline functions for image IO
inline bool simple_loadimage_wrapper(VISIOCYTEPluginCallback & cb, const char * filename, unsigned char * & pdata, VISIOCYTELONG sz[4], int & datatype)
{
    if (!filename || !sz)
        return false;

    Image4DSimple *inimg = 0;
    inimg = cb.loadImage((char *)filename);
    if (!inimg || !inimg->valid())
        return false;

    if (pdata) {delete []pdata; pdata=0;}

    VISIOCYTELONG totalbytes = inimg->getTotalBytes();
    try
    {
        pdata = new unsigned char [totalbytes];
        if (!pdata)
            goto Label_error_simple_loadimage_wrapper;

        memcpy(pdata, inimg->getRawData(), totalbytes);
        datatype = inimg->getUnitBytes(); //1,2,or 4
        sz[0] = inimg->getXDim();
        sz[1] = inimg->getYDim();
        sz[2] = inimg->getZDim();
        sz[3] = inimg->getCDim();
        if (inimg) {delete inimg; inimg=0;} //delete "outimg" pointer, added by Z.ZHOU 06122014

    }
    catch (...)
    {
        goto Label_error_simple_loadimage_wrapper;
    }

    return true;

Label_error_simple_loadimage_wrapper:
    if (inimg) {delete inimg; inimg=0;}
    return false;
}

inline bool simple_loadimage_wrapper_singleslice(VISIOCYTEPluginCallback & cb, const char * filename, unsigned char * & pdata, VISIOCYTELONG sz[4], int & datatype, VISIOCYTELONG zsliceno)
{
    if (!filename || !sz)
        return false;

    Image4DSimple *inimg = 0;
    if (zsliceno>=0)
        inimg = cb.loadImage((char *)filename, zsliceno);
    else
        inimg = cb.loadImage((char *)filename);
    if (!inimg || !inimg->valid())
        return false;

    if (pdata) {delete []pdata; pdata=0;}

    VISIOCYTELONG totalbytes = inimg->getTotalBytes();
    try
    {
        pdata = new unsigned char [totalbytes];
        if (!pdata)
            goto Label_error_simple_loadimage_wrapper;

        memcpy(pdata, inimg->getRawData(), totalbytes);
        datatype = inimg->getUnitBytes(); //1,2,or 4
        sz[0] = inimg->getXDim();
        sz[1] = inimg->getYDim();
        sz[2] = inimg->getZDim();
        sz[3] = inimg->getCDim();
        if (inimg) {delete inimg; inimg=0;}//delete "outimg" pointer, added by Z.ZHOU 06122014

    }
    catch (...)
    {
        goto Label_error_simple_loadimage_wrapper;
    }

    return true;

Label_error_simple_loadimage_wrapper:
    if (inimg) {delete inimg; inimg=0;}
    return false;
}


inline bool simple_saveimage_wrapper(VISIOCYTEPluginCallback & cb, const char * filename, unsigned char * pdata, VISIOCYTELONG sz[4], int datatype)
{
    if (!filename || !sz || !pdata)
    {
        visiocyte_msg("some of the parameters for simple_saveimage_wrapper() are not valid.", 0);
        return false;
    }

    ImagePixelType dt;
    if (datatype==1)
        dt = VISIOCYTE_UINT8;
    else if (datatype==2)
        dt = VISIOCYTE_UINT16;
    else if (datatype==4)
        dt = VISIOCYTE_FLOAT32;
    else
    {
        visiocyte_msg(QString("the specified save data type in simple_saveimage_wrapper() is not valid, dt=[%1].").arg(datatype), 0);
        return false;
    }

    Image4DSimple * outimg = new Image4DSimple;
    if (outimg)
        outimg->setData(pdata, sz[0], sz[1], sz[2], sz[3], dt);
    else
    {
        visiocyte_msg("Fail to new Image4DSimple for outimg.");
        return false;
    }

    return cb.saveImage(outimg, (char *)filename);
    //in this case no need to delete "outimg" pointer as it is just a container and will not use too much memory
}



#endif /* _VISIOCYTE_INTERFACE_H_ */

