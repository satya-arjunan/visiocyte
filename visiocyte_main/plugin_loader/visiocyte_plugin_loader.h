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




/**************************************************************************
*
* Visiocyte's plug-in loading support functions
*
* 2009-08-11, Zongcai Ruan
* Last change: 2010-10-08. Hanchuan Peng
* Last change: 2015-02-10. Hanchuan Peng
***************************************************************************
*/

#ifndef _VISIOCYTE_PLUGIN_LOADER_H_
#define _VISIOCYTE_PLUGIN_LOADER_H_
#include <qmenu.h>
#if defined(USE_Qt5_VS2015_Win7_81) || defined(USE_Qt5_VS2015_Win10_10_14393)
  #include <QtWidgets>
#else
  #include <QtGui>
#endif
// These two explicit includes make my IDE work better - CMB 08-Oct-2010
#include "../basic_c_fun/visiocyte_interface.h"


QString     visiocyte_getInterfaceName(QObject *plugin);
QStringList visiocyte_getInterfaceMenuList(QObject *plugin);
QStringList visiocyte_getInterfaceFuncList(QObject *plugin);

class MainWindow;
class Visiocyte_PluginLoader;

class Visiocyte_PluginLoader : public QObject, public VISIOCYTEPluginCallback2
{
    Q_OBJECT;

public:
    Visiocyte_PluginLoader(QMenu* menuPlugin, MainWindow* mainwindow);
    Visiocyte_PluginLoader(MainWindow* mainwindow); //by PHC, 101008. a convenience function for access plugin interface w/o a menu
    virtual ~Visiocyte_PluginLoader() {clear();}
    static QList<QDir> getPluginsDirList();

public slots:
	void rescanPlugins();
	void populateMenus(); //hook menu to visiocyte, called by rescanPlugins, MainWindow::updateProcessingMenu
	void aboutPlugins();
    void runPlugin();
	void runPlugin(QPluginLoader *loader, const QString & menuString); //by PHC, 101008

    void runRecentPlugin(); //added by Zhi Z, 20140721
    void clear_recentPlugins();

protected:
	bool runSingleImageInterface(QObject* plugin, const QString& command);
	bool runPluginInterface(QObject* plugin, const QString& command);
	bool runPluginInterface2(QObject* plugin, const QString& command);
	bool runPluginInterface2_1(QObject* plugin, const QString& command);

	void clear();
	void loadPlugins(); //load only once
	void addToMenu(QMenu *menu, QObject *plugin, const QStringList &texts, const char *member);
	void searchPluginDirs(QMenu* menu, const QDir& pluginsDir);
	void searchPluginFiles(QMenu* menu, const QDir& pluginsDir);
    void addrecentPlugins(QMenu* menu); //add by Zhi Z, 20140721
    void updated_recentPlugins();

    // QList<QDir> pluginsDirList;
    QStringList pluginFilenameList;
    QList<QPluginLoader*> pluginList;
    QMenu plugin_menu;

    QStringList recentpluginsList;
    QList<QVariant> recentpluginsIndex;
    QMenu *plugin_recent;//added by Zhi Z, 20140721
    QMenu *plugin_most;//added by Zhi Z, 20140721

    QMenu* visiocyte_menuPlugin;
    MainWindow* visiocyte_mainwindow;

public:
	QStringList getPluginNameList(); //by YuY 20110429

//VISIOCYTEPluginCallback
public:
	virtual bool callPluginFunc(const QString &plugin_name,
			const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output);
    //virtual bool callPluginMenu(const QString &plugin_name, const QString &func_name); //a bug caused in v2.823?

	virtual visiocytehandleList getImageWindowList() const;
	virtual visiocytehandle currentImageWindow();
	virtual visiocytehandle curHiddenSelectedWindow(); //by PHC 101009

	virtual visiocytehandle newImageWindow(QString name);
	virtual void updateImageWindow(visiocytehandle image_window, bool b_forceUpdateChannelMinMaxValues); //by PHC, 20120412

	virtual QString getImageName(visiocytehandle image_window) const;
	virtual void setImageName(visiocytehandle image_window, QString name);

	virtual Image4DSimple* getImage(visiocytehandle image_window);
	virtual bool setImage(visiocytehandle image_window, Image4DSimple* image);

	//a special debug function. don't use if you don't know how to use. by PHC 100731.
	virtual bool setImageTest(visiocytehandle image_window, Image4DSimple* image, unsigned char *a);

	virtual LandmarkList  getLandmark(visiocytehandle image_window);
	virtual bool setLandmark(visiocytehandle image_window, LandmarkList& landmark_list);

	virtual ROIList getROI(visiocytehandle image_window);
	virtual bool setROI(visiocytehandle image_window, ROIList & roi_list);

	virtual NeuronTree getSWC(visiocytehandle image_window);
	virtual bool setSWC(visiocytehandle image_window, NeuronTree & nt);

    virtual Image4DSimple * loadImage(char *filename);  //2013-08-09. two more functions for simplied calls to use Visiocyte's image loading and saving functions without linking to additional libs
    virtual Image4DSimple * loadImage(char *filename, VISIOCYTELONG zsliceno);  //2013-11-02
    virtual bool saveImage(Image4DSimple * img, char *filename);

	virtual VISIOCYTE_GlobalSetting getGlobalSetting();
	virtual bool setGlobalSetting( VISIOCYTE_GlobalSetting & gs );

	virtual void open3DWindow(visiocytehandle image_window);
	virtual void close3DWindow(visiocytehandle image_window);
	virtual void openROI3DWindow(visiocytehandle image_window);
	virtual void closeROI3DWindow(visiocytehandle image_window);

	virtual void pushObjectIn3DWindow(visiocytehandle image_window);
	virtual void pushImageIn3DWindow(visiocytehandle image_window);
	virtual int pushTimepointIn3DWindow(visiocytehandle image_window, int timepoint);

	virtual bool screenShot3DWindow(visiocytehandle image_window, QString filename);
	virtual bool screenShotROI3DWindow(visiocytehandle image_window, QString filename);


//VISIOCYTEPluginCallback2
public:
    virtual View3DControl * getView3DControl(visiocytehandle image_window);
	virtual View3DControl * getLocalView3DControl(visiocytehandle image_window);
	virtual TriviewControl * getTriviewControl(visiocytehandle image_window); //aded by PHC, 2010-12-08

    //added PHC 20120406. add a main window handle, to allow access everything in Visiocyte

    virtual MainWindow * getVisiocyteMainWindow();
    virtual QList <VisiocyteR_MainWindow *> getListAll3DViewers();
    virtual VisiocyteR_MainWindow * find3DViewerByName(QString fileName); //the name can be partially matched

    //added PHC 20120406 to allow uses to access the surface data objects in a 3D viewer but based on a tri-view window
	virtual QList <NeuronTree> * getHandleNeuronTrees_3DGlobalViewer(visiocytehandle image_window);
	virtual QList <CellAPO>    * getHandleAPOCellList_3DGlobalViewer(visiocytehandle image_window);
    virtual QList <LabelSurf> getListLabelSurf_3DGlobalViewer(visiocytehandle image_window);
    virtual bool setListLabelSurf_3DGlobalViewer(visiocytehandle image_window, QList <LabelSurf> listLabelSurfinput);

    //added PHC 20120406 to allow uses to access the surface data objects in a 3D viewer
	virtual QList <NeuronTree> * getHandleNeuronTrees_Any3DViewer(VisiocyteR_MainWindow *w);
	virtual QList <CellAPO>    * getHandleAPOCellList_Any3DViewer(VisiocyteR_MainWindow *w);
    virtual QList <LabelSurf> getListLabelSurf_Any3DViewer(VisiocyteR_MainWindow *w);
    virtual bool setListLabelSurf_Any3DViewer(VisiocyteR_MainWindow *w, QList <LabelSurf> listLabelSurfinput);

    //added PHC 20130904 allow a plugin program to refresh and rescan all plugins
    //virtual void refreshMainMenuPluginList(); //not working, by PHC 20130904

    virtual View3DControl * getView3DControl_Any3DViewer(VisiocyteR_MainWindow *w);  //Dec. 02, 2013 by Zhi Zhou
    virtual void screenShot_Any3DViewer(VisiocyteR_MainWindow *w,QString filename);//Dec. 02, 2013 by Zhi Zhou
    virtual void update_3DViewer(VisiocyteR_MainWindow *w);//Dec, 9, 2013. PHC
    virtual void update_NeuronBoundingBox(VisiocyteR_MainWindow *w); //Oct. 08, 2014 by Hanbo Chen

    virtual LandmarkList * getHandleLandmarkList_Any3DViewer(VisiocyteR_MainWindow *w); //Oct. 16, 2014 by Hanbo Chen
    virtual bool setHandleLandmarkList_Any3DViewer(VisiocyteR_MainWindow *w, LandmarkList & landmark_list); //Aug. 2, 2017 by Zhi Zhou

    virtual DataLists_in_3dviewer fetch_3dviewer_datafilelist(QString name3dviewer); //20140122 a conveniece function to access the record of data in a 3D viewer

    // extend the plugin interface by allowing open a 3D surface file , PHC Feb 10, 2015
    virtual VisiocyteR_MainWindow * open3DViewerForSingleSurfaceFile(QString fileName);
    virtual VisiocyteR_MainWindow * open3DViewerForLinkerFile(QString fileName);
    virtual VisiocyteR_MainWindow * createEmpty3DViewer(); //create a 3D viewer with no content
    virtual void setWindowDataTitle(VisiocyteR_MainWindow *w, QString title);
    virtual QString getWindowDataTitle(VisiocyteR_MainWindow *w);
    virtual void moveWindow(VisiocyteR_MainWindow *w, int x, int y);
    virtual void resizeWindow(VisiocyteR_MainWindow *w, int x, int y);
    virtual void setHideDisplayControlButton(VisiocyteR_MainWindow *w);
    //virtual void setResizeEvent(VisiocyteR_MainWindow *w, int x, int y);

#ifdef __ALLOW_VR_FUNCS__
    virtual void openVRWindow(VisiocyteR_MainWindow *w, bool bOnlineMode = false);
    virtual void openVRWindowV2(visiocytehandle image_window, bool bOnlineMode = false);
#endif

    //added TeraFly interface, functions are provided by Alessadnro Bria, the wrapper is provided by Zhi Zhou Aug. 23, 2017
    virtual NeuronTree getSWCTeraFly();
    virtual bool setSWCTeraFly(NeuronTree & nt);
    virtual LandmarkList  getLandmarkTeraFly();
    virtual bool setLandmarkTeraFly(LandmarkList & landmark_list);
    virtual QString getPathTeraFly();
    virtual const Image4DSimple * getImageTeraFly();
    virtual QString versionTeraFly();

    virtual bool getDimTeraFly(const std::string & path, VISIOCYTELONG * & sz);
    virtual unsigned char* getSubVolumeTeraFly(const std::string & path, size_t x0, size_t x1, size_t y0, size_t y1, size_t z0, size_t z1);
    virtual void releaseOpenedVolumesTeraFly();

    virtual bool setImageTeraFly(size_t x, size_t y, size_t z);

	virtual void redrawEditInfo(int editInputNum);

};

#endif
