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


/*
 * xformwidget.h
 *
 */

#ifndef XFORMWIDGET_H_
#define XFORMWIDGET_H_

#include "visiocyte_core.h"
#include "../basic_c_fun/basic_triview.h"
#include "../basic_c_fun/volimg_proc.h"
//#include "../3drenderer/visiocyter_common.h"
#include "../basic_c_fun/basic_view3d.h"

#include "mapview.h"

#include <QMdiSubWindow>
#include <QMenu>

class VisiocyteR_MainWindow;
class VisiocyteR_GLWidget;
class MainWindow;
class ChannelTabWidget;

#define DELETE_AND_ZERO(p)	{ if ((p)!=NULL) delete (p); (p) = NULL; }



struct iDrawExternalParameter
{
	My4DImage* image4d;
	XFormWidget *xwidget;
	VisiocyteR_MainWindow *window3D; //pointer to 3DView main window, by RZC 080921, 090503
	QList <VisiocyteR_MainWindow *> * p_list_3Dview_win; //by PHC, 081003. This is linked to a mainwindow, which will keep record of all generated VisiocyteR_MainWindows
	MainWindow *VISIOCYTEmainwindow; //the pointer to the VISIOCYTE main window, so that VisiocyteR can get the global setting preferences. add on 090503

	QStringList swc_file_list;
	QStringList pointcloud_file_list;
	QString surface_file;
	QString labelfield_file;

	//some external controls for the 3d viewer
	//float zthickness; //the default z-thickness when start the 3d viewer. 100626

	int b_local; // 0,1,2,3
	LocationSimple local_start, local_size;
	QPoint local_win_pos;
	QSize local_win_size;

	bool b_use_512x512x256;
	bool b_still_open;

     // used for saving resampled 512X512X256 data, ZJL

          // for data
          RGBA8 *total_rgbaBuf, *rgbaBuf;
          float sampleScale[5];
          VISIOCYTELONG bufSize[5]; //(x,y,z,c,t) 090731: add time dim
          bool b_limitedsize;
          bool isSimulatedData;
          int data_unitbytes;
          unsigned char* data4dp;
          unsigned char**** data4d_uint8;

          VISIOCYTELONG dim1, dim2, dim3, dim4, dim5;
          VISIOCYTELONG start1, start2, start3, start4, start5;
          VISIOCYTELONG size1, size2, size3, size4, size5;
          BoundingBox dataBox;
          BoundingBox dataViewProcBox;
          bool bSavedDataFor3DViewer;

          // for texture
          int imageX, imageY, imageZ, imageT;
          int safeX, safeY, safeZ;
          int realX, realY, realZ, realF;
          int fillX, fillY, fillZ, fillF;



     iDrawExternalParameter() {b_use_512x512x256=true; b_local=b_still_open=false;
          image4d=0; xwidget=0; window3D=0; p_list_3Dview_win=0; VISIOCYTEmainwindow=0;
          // for saving resampled
          total_rgbaBuf=0; rgbaBuf=0; b_limitedsize=true;isSimulatedData=false;
          sampleScale[0]=sampleScale[1]=sampleScale[2]=sampleScale[3]=sampleScale[4]=1;
          bufSize[0]=bufSize[1]=bufSize[2]=bufSize[3]=bufSize[4]=0;
          data4dp=0; data4d_uint8=0;
          dim1=dim2=dim3=dim4=dim5=0;
		start1=start2=start3=start4=start5=0;
		size1=size2=size3=size4=size5=0;
          bSavedDataFor3DViewer = true;
          /*zthickness=1;*/}
     ~iDrawExternalParameter() {if (xwidget==0 && image4d) delete image4d;
          DELETE_AND_ZERO(total_rgbaBuf);
          DELETE_AND_ZERO(rgbaBuf); DELETE_AND_ZERO(data4dp);
     }
};


#if defined(USE_Qt5_VS2015_Win7_81) || defined(USE_Qt5_VS2015_Win10_10_14393)
class XFormWidget : public QMdiSubWindow, public TriviewControl //class XFormWidget : public QMainWindow
#else
class XFormWidget : public QWidget, public TriviewControl //class XFormWidget : public QMainWindow
#endif
{
    Q_OBJECT;

public:
    XFormWidget(QWidget *parent);
    XFormWidget(QWidget *parent, Qt::WidgetAttribute f); //080814 add the second arugment. This is eventually not used. Just keep here for further use
    ~XFormWidget();

    void initialize();
    void connectColorGUI(); //110721 RZC
    void disconnectColorGUI(); //110721 RZC
    void setColorGUI(); //110721 RZC
    QWidget* createColorGUI(); //110719 RZC
	void createGUI();
	void connectEventSignals();
	void disconnectEventSignals();
	void updateDataRelatedGUI();
    bool setCTypeBasedOnImageData();
	void cleanData();
	bool loadData();
	bool importLeicaData();
	bool importGeneralImgSeries(const QStringList & mylist, TimePackType timepacktype);

    bool loadFile(QString filename);
    bool importGeneralImageFile(QString filename);
    bool importLeicaFile(QString filename);
	QString userFriendlyCurrentFile() {return (openFileNameLabel);}
	QString getOpenFileNameLabel() {return openFileNameLabel;}

	bool saveData();
	bool saveFile(QString filename);

	My4DImage * getImageData() {return imgData;} //080326

	bool newProcessedImage(QString filename, unsigned char *ndata1d, VISIOCYTELONG nsz0, VISIOCYTELONG nsz1, VISIOCYTELONG nsz2, VISIOCYTELONG nsz3, ImagePixelType ndatatype); //080408
	// for replace imageData and filename
	bool setImageData(unsigned char *ndata1d, VISIOCYTELONG nsz0, VISIOCYTELONG nsz1, VISIOCYTELONG nsz2, VISIOCYTELONG nsz3, ImagePixelType ndatatype); //090818 RZC
	bool setCurrentFileName(QString cfilename);


	//for communication of windows
	void setMainControlWindow(MainWindow * p) {p_mainWindow = p;}
	virtual MainWindow * getMainControlWindow() {return p_mainWindow;}

	void forceToChangeFocus(int x, int y, int z); //081210
	void changeFocusFromExternal(int x, int y, int z); //this should be called from external. When no cross-image communication is needed, should not use this.
    void changeFocusToExternal(int newx, int newy, int newz); // this is the function to call other image-view's changeFocusFromExternal() function

        void setWindowTitle_Prefix(const char *prefix);
        void setWindowTitle_Suffix(const char *sfix);

	VISIOCYTELONG estimateRoughAmountUsedMemory(); //080810

	bool getFlagImgValScaleDisplay(); //100814

	My4DImage * selectSubjectImage();
	My4DImage * selectImage();
	QList <BlendingImageInfo> selectBlendingImages();

	void setColorType(ImageDisplayColorType myctype) {Ctype = myctype;}
	ImageDisplayColorType getColorType() {return Ctype;}
    QRadioButton* colorMapRadioButton() {return colorMapDispType;} //110723 RZC

	iDrawExternalParameter mypara_3Dview;
	iDrawExternalParameter mypara_3Dlocalview;
	VISIOCYTE_atlas_viewerDialog *atlasViewerDlg;

	VISIOCYTELONG bbx0, bbx1, bby0, bby1, bbz0, bbz1; //by PHC. 100821. the current regional bbox. for curve based zoomin
	void setLocal3DViewerBBox(VISIOCYTELONG x0, VISIOCYTELONG x1, VISIOCYTELONG y0, VISIOCYTELONG y1, VISIOCYTELONG z0, VISIOCYTELONG z1)
	{
		bbx0 = x0;
		bbx1 = x1;
		bby0 = y0;
		bby1 = y1;
		bbz0 = z0;
		bbz1 = z1;
	}


	double disp_zoom; //081114
	bool b_use_dispzoom;

	bool bDispMarkerLabel;

    // for mapview ZJL
    void createMapviewControlWin();
    void updateMapviewControlWin(int level);
    void updateMapview();

    // mapview
    // QString hraw_prefix; // for mapview control
     Mapview_Paras mapview_paras; // for mapview control
     ImageMapView mapview; // mapview

protected:
	virtual void changeEvent(QEvent* e);    //110802 RZC
	virtual void hideEvent(QHideEvent * e); //110808 RZC

	void keyPressEvent ( QKeyEvent * e); //100815, PHC
	void closeEvent ( QCloseEvent * event );  //080814
	//void focusInEvent ( QFocusEvent * event ); //080829

	void loadH264Image( char const* filename );
	void loadHDF5( char const* filename );

	void * p_customStruct; //a convenient pointer to pass back and forth some useful parameter information for an engine

private:
	// communication of different images windows
	MainWindow * p_mainWindow;
	bool bSendSignalToExternal, bAcceptSignalFromExternal, bUsingMultithreadedImageIO;

	My4DImage *imgData;
    ImageDisplayColorType Ctype;
    ImageDisplayColorType Ctype_glass;

	QString openFileNameLabel;

	// Interface components
	bool bExistGUI;
    bool bLinkFocusViews;
    bool bDisplayFocusCross;

	QGroupBox *dataGroup, *viewGroup, *infoGroup;
	QGroupBox *mainGroup, *coordGroup, *scaleGroup, *typeGroup;

	QScrollBar *xSlider, *ySlider, *zSlider;
	QSpinBox *xValueSpinBox, *yValueSpinBox, *zValueSpinBox;
	QLabel *xSliderLabel, *ySliderLabel, *zSliderLabel;
	QCheckBox *linkFocusCheckBox;
	QCheckBox *displayFocusCrossCheckBox;

    QScrollBar *xScaleSlider, *yScaleSlider, *zScaleSlider;
	QLabel *xScaleSliderLabel, *yScaleSliderLabel, *zScaleSliderLabel;
	QCheckBox *lookingGlassCheckBox;
	QPushButton *zoomWholeViewButton;

    QRadioButton *colorRedType, *colorGreenType, *colorBlueType, *colorAllType;
    QRadioButton *colorRed2GrayType, *colorGreen2GrayType, *colorBlue2GrayType, *colorAll2GrayType;
    QRadioButton *colorMapDispType;
	QCheckBox *imgValScaleDisplayCheckBox;

	QCheckBox * cBox_bSendSignalToExternal, * cBox_bAcceptSignalFromExternal;

    QPushButton *landmarkCopyButton, *landmarkPasteButton, *landmarkSaveButton, *landmarkLoadButton; //080107
	QPushButton *landmarkManagerButton;
	//QCheckBox *landmarkLabelDispCheckBox;

    QPushButton *resetButton, *openFileNameButton, *whatsThisButton, *imgProcessButton, *imgVISIOCYTEButton; //, *imgVISIOCYTEROIButton;

	QHBoxLayout *allLayout;
	  QVBoxLayout *dataGroupLayout;
	    QGridLayout *xyzViewLayout;
	    QVBoxLayout *infoGroupLayout;
	  QVBoxLayout *mainGroupLayout;
	    QGridLayout *coordGroupLayout;
	    QGridLayout *scaleGroupLayout;
	    QGridLayout *typeGroupLayout;
	    QGridLayout *LandmarkGroupLayout;

	XFormView *xy_view; //change in Z
    XFormView *yz_view; //change in X
    XFormView *zx_view; //change in Y

    ChannelTabWidget *channelTabXView;//110722 RZC
    ChannelTabWidget *channelTabGlass;//110801 RZC

    // for Mapview. 20120309 ZJL
    QWidget *mvControlWin;
    QScrollBar *xSlider_mapv, *ySlider_mapv, *zSlider_mapv;
    QSpinBox *xValueSpinBox_mapv, *yValueSpinBox_mapv, *zValueSpinBox_mapv, *zoomSpinBox_mapv;
    QScrollBar *zoomSlider_mapv;

    // @ADDED by Alessandro on 2015-05-07 : hide/display controls and info.
    QPushButton *hideDisplayControlsButton;
    QPushButton *hideDisplayInfoButton;


	MyTextBrowser *focusPointFeatureWidget;

    QMenu menuTriviewZoom;
	void createMenuOfTriviewZoom();

    QMenu menu3DViewer;
	void createMenuOf3DViewer();

public slots:
//    void changeColorType(ImageDisplayColorType c);

//110722 RZC, for directly updating pixmap of 3view.  //110803 RZC, add bGlass
	void mixChannelColorPlaneX(const QPixmap& pxm, bool bGlass) {yz_view->setPixmap(pxm, bGlass);}
	void mixChannelColorPlaneY(const QPixmap& pxm, bool bGlass) {zx_view->setPixmap(pxm, bGlass);}
	void mixChannelColorPlaneZ(const QPixmap& pxm, bool bGlass) {xy_view->setPixmap(pxm, bGlass);}
//110802 RZC, for show/hide channelTabGlass
	void onActivated(QWidget* );
//110803 RZC, for sync multiple ChannelTabWidget
	void syncChannelTabWidgets(ChannelTabWidget* sender);

	void setColorRedType();
    void setColorGreenType();
    void setColorBlueType();
    void setColorAllType();
    void setColorRed2GrayType();
    void setColorGreen2GrayType();
    void setColorBlue2GrayType();
    void setColorAll2GrayType();
	void setColorMapDispType(ImageDisplayColorType Ctype=colorPseudoMaskColor, bool bGlass=false); //110725 RZC, add Ctype //110803 RZC, add bGlass
	void switchMaskColormap();

    void toggleLinkFocusCheckBox();
    void toggleDisplayFocusCrossCheckBox();
	void toggleImgValScaleDisplay();
	void toggleLookingGlassCheckBox();

	void doMenuOfTriviewZoom();
    void triview_zoomin();
    void triview_zoomout();
	void triview_zoom1();
	void triview_zoom2();
	void triview_zoomhalf();
	void triview_setzoom(double z, bool b_multiply); //b_multiply determine is z is the target zoom, or the target zoom should be product of the current zoom multply z

	void popupImageProcessingDialog();
	void popupImageProcessingDialog(QString item);
	void doImage3DView();
	void doImage3DLocalMarkerView();
	void doImage3DLocalRoiView();
	void doImage3DLocalBBoxView(); //do not have arguments so that can be used as the slot of a timer signal
    void doImage3DView(bool tmp_b_use_512x512x256, int b_local=0, VISIOCYTELONG bbx0=-1, VISIOCYTELONG bbx1=-1, VISIOCYTELONG bby0=-1, VISIOCYTELONG bby1=-1, VISIOCYTELONG bbz0=-1, VISIOCYTELONG bbz1=-1,
                       bool show=true); // @ADDED by Alessandro on 2015-09-29. Postpone show() if required.
	void doMenuOf3DViewer();
	void aboutInfo();

    void setOpenFileName();
    void reset();

	//this should be called from external. When no cross-image communication is needed, should not use this.
	void changeFocusXToExternal(int c); //will call changeFocusToExternal(int,int,int)
	void changeFocusYToExternal(int c); //will call changeFocusToExternal(int,int,int)
	void changeFocusZToExternal(int c); //will call changeFocusToExternal(int,int,int)

	void toggleCheckBox_bSendSignalToExternal();
	void toggleCheckBox_bAcceptSignalFromExternal();

	void copyLandmarkToPublicBuffer();
	void pasteLandmarkFromPublicBuffer();
	void saveLandmarkToFile();
	void loadLandmarkFromFile();
	void openLandmarkManager();

	void toggleLandmarkLabelDisp() {bDispMarkerLabel = !bDispMarkerLabel; this->updateViews();}

	void launchAtlasViewer(int curTab=0);

	void cascadeWindows();
	void updateViews(); //090615: a convenient function to call my4dimage updateViews()

	void updateTriview(); // call MainWindow updateTriview() Dec-21-2010 YuY

    // for mapview
    void changeXOffset_mapv(int x);
    void changeYOffset_mapv(int y);
    void changeZOffset_mapv(int z);
    void changeLevel_mapv(int level);

    // @ADDED by Alessandro on 2015-05-07 : hide/display controls and infos.
    void hideDisplayControls();
    void hideDisplayInfo();


signals:
	void colorChanged(int); //110722 RZC, connected to ChannelTable::updateXFormWidget(int)
	void colorChangedGlass(int); //110803 RZC, connected to ChannelTable::updateXFormWidget(int)

    void external_focusXChanged(int c);
    void external_focusYChanged(int c);
    void external_focusZChanged(int c);

	void external_validZSliceChanged(long z);


#define __used_by_visiocyte_interface__
public:    // in mainwindow_interface.cpp

	bool transferImageData(Image4DSimple *img, unsigned char *a);
	QList<QPolygon> get3ViewROI();
	bool set3ViewROI(QList<QPolygon> & roi_list);

	void finishEditingSWC();

	void open3DWindow();
	void openROI3DWindow();
	void close3DWindow();
	void closeROI3DWindow();
	void pushObjectIn3DWindow();
	void pushImageIn3DWindow();
	int pushTimepointIn3DWindow(int timepoint);

	bool screenShot3DWindow(QString filename);
	bool screenShotROI3DWindow(QString filename);

	VisiocyteR_GLWidget * getView3D();
	VisiocyteR_GLWidget * getLocalView3D();

	// a few interface functions for external plugin use. prototyped by PHC. 2010-Dec-10

	virtual void getFocusLocation(VISIOCYTELONG & cx, VISIOCYTELONG & cy, VISIOCYTELONG & cz) const
	{
		if (imgData)
		{
			cx = imgData->getFocusX()+1;
			cy = imgData->getFocusY()+1;
			cz = imgData->getFocusZ()+1;
		}
	}
	virtual void setFocusLocation(VISIOCYTELONG cx, VISIOCYTELONG cy, VISIOCYTELONG cz)
	{
		if (imgData)
		{
			forceToChangeFocus(cx, cy, cz);
			imgData->updateViews(); // update trivews
		}
	}

	virtual void setFocusLocation2Center()
	{
		if (imgData)
		{
			// reinit focus to center along x, y, z
			VISIOCYTELONG sx, sy, sz;

			sx = imgData->getXDim();
			sy = imgData->getYDim();
			sz = imgData->getZDim();

			forceToChangeFocus(sx/2, sy/2, sz/2);
			imgData->updateViews(); // update trivews
		}
	}

	virtual void updateMinMax(VISIOCYTELONG nFrame)
	{
		if (imgData)
		{
			//imgData->updateminmaxvalues();

			VISIOCYTELONG sx, sy, sz, sc;

			sx = imgData->getXDim();
			sy = imgData->getYDim();
			sz = imgData->getZDim();
			sc = imgData->getCDim();

			if(nFrame<0 || nFrame>=sz) // changed by YuY Feb 8, 2011
				return;

			VISIOCYTELONG offsets = nFrame*sx*sy;
			VISIOCYTELONG pagesz = sx*sy;

			switch (imgData->getDatatype())
			{
				case VISIOCYTE_UINT8:
					for(VISIOCYTELONG i=0;i<sc;i++)
					{
						unsigned char minvv,maxvv;
						VISIOCYTELONG tmppos_min, tmppos_max;
						unsigned char *datahead = (unsigned char *)(imgData->getRawDataAtChannel(i));

						minMaxInVector(datahead+offsets, pagesz, tmppos_min, minvv, tmppos_max, maxvv);

						if(imgData->p_vmax[i]<maxvv)
							imgData->p_vmax[i] = maxvv;
						if(imgData->p_vmin[i]>minvv)
							imgData->p_vmin[i] = minvv;
					}
					break;

				case VISIOCYTE_UINT16:
					for(VISIOCYTELONG i=0;i<sc;i++)
					{
						USHORTINT16 minvv,maxvv;
						VISIOCYTELONG tmppos_min, tmppos_max;
						USHORTINT16 *datahead = (USHORTINT16 *)(imgData->getRawDataAtChannel(i));

						minMaxInVector(datahead+offsets, pagesz, tmppos_min, minvv, tmppos_max, maxvv);

						if(imgData->p_vmax[i]<maxvv)
							imgData->p_vmax[i] = maxvv;
						if(imgData->p_vmin[i]>minvv)
							imgData->p_vmin[i] = minvv;
					}
					break;

				case VISIOCYTE_FLOAT32:
					for(VISIOCYTELONG i=0;i<sc;i++)
					{
						float minvv,maxvv;
						VISIOCYTELONG tmppos_min, tmppos_max;
						float *datahead = (float *)(imgData->getRawDataAtChannel(i));

						minMaxInVector(datahead+offsets, pagesz, tmppos_min, minvv, tmppos_max, maxvv);

						if(imgData->p_vmax[i]<maxvv)
							imgData->p_vmax[i] = maxvv;
						if(imgData->p_vmin[i]>minvv)
							imgData->p_vmin[i] = minvv;
					}
					break;

				default:
					visiocyte_msg("Invalid data type found in updateMinMax().");
					return;
			}
		}
	}

	virtual void getTriViewColorDispType(int & mytype)
	{
		mytype = (int) ( this->getColorType() );
	}
	virtual void setTriViewColorDispType(int mytype)
	{
		this->setColorType((ImageDisplayColorType)mytype);
	}

	virtual void * getCustomStructPointer() const
	{
		return (this->p_customStruct);
	}

	virtual void setCustomStructPointer(void *a)
	{
		this->p_customStruct = a;
	}

	virtual VISIOCYTELONG getValidZslice() const
	{
		if (imgData)
		{
			return imgData->getValidZSliceNum();
		}
                return 0;
	}

	virtual void setValidZslice(VISIOCYTELONG curslice)
	{
		if (imgData)
		{
			imgData->setValidZSliceNum(curslice);
		}
	}

	virtual VISIOCYTELONG getPreValidZslice() const
	{
		if (imgData)
		{
			return imgData->getPreValidZSliceNum();
		}
                return 0;
	}

	virtual void setPreValidZslice(VISIOCYTELONG preslice)
	{
		if (imgData)
		{
			imgData->setPreValidZSliceNum(preslice);
		}
	}

	virtual void trigger(VISIOCYTELONG curslice)
	{
		if (imgData && curslice>0)
		{
			qDebug()<<"triggering here ..."<<curslice;
			emit external_validZSliceChanged(curslice);
		}
	}

};


#endif /* XFORMWIDGET_H_ */
