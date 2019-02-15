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

Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) ‚ÄúVISIOCYTE enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,‚Äù Nature Biotechnology, Vol. 28, No. 4, pp. 348-353, DOI: 10.1038/nbt.1612. ( http://penglab.janelia.org/papersall/docpdf/2010_NBT_VISIOCYTE.pdf )

Peng, H, Ruan, Z., Atasoy, D., and Sternson, S. (2010) ‚ÄúAutomatic reconstruction of 3D neuron structures using a graph-augmented deformable model,‚Äù Bioinformatics, Vol. 26, pp. i38-i46, 2010. ( http://penglab.janelia.org/papersall/docpdf/2010_Bioinfo_GD_ISMB2010.pdf )

3. This software is provided by the copyright holders (Hanchuan Peng), Howard Hughes Medical Institute, Janelia Farm Research Campus, and contributors "as is" and any express or implied warranties, including, but not limited to, any implied warranties of merchantability, non-infringement, or fitness for a particular purpose are disclaimed. In no event shall the copyright owner, Howard Hughes Medical Institute, Janelia Farm Research Campus, or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; reasonable royalties; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

4. Neither the name of the Howard Hughes Medical Institute, Janelia Farm Research Campus, nor Hanchuan Peng, may be used to endorse or promote products derived from this software without specific prior written permission.

*************/




/**************************************************************************
*
* VISIOCYTE's plug-in loading support functions
*
* 2009-08-11, Zongcai Ruan
* 2009-08-14. Hanchuan Peng. allow the image size and type change
* 2010-08-01: Hanchuan Peng. add stronger check of the possible change of the data buffer when the data is set back
*
***************************************************************************
*/

#include "../3drenderer/visiocyter_glwidget.h"
#include "../basic_c_fun/visiocyte_interface.h"
#include "../3drenderer/renderer_gl1.h"
#include "../3drenderer/visiocyter_mainwindow.h"
#include "../visiocyte/mainwindow.h"
#include "../visiocyte/visiocyte_core.h"
#include "visiocyte_plugin_loader.h"
#include "pluginDialog.h"
#include "../terafly/src/control/CPlugin.h"
#include <QtGlobal>


void pumpEvents(int loops=100)
{
	for (int j=1; j<loops; j++) //try to empty all existing events
	{
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}
}

//=======================================================
// Qt Plugin ONLY supports 1 interface in 1 plugin module

QString     visiocyte_getInterfaceName(QObject *plugin)
{
   	QString name;

    // Derived class must appear first, to be selected
	VISIOCYTESingleImageInterface2_1 *iFilter2_1 = qobject_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
    if (iFilter2_1 )  return (name = "VISIOCYTESingleImageInterface/2.1");

    // Base class must appear later, so derived class has a chance
	VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(plugin);
    if (iFilter )  return (name = "VISIOCYTESingleImageInterface/1.0");

    VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
    if (iface2_1 )  return (name = "VISIOCYTEPluginInterface/2.1");

    VISIOCYTEPluginInterface2 *iface2 = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
    if (iface2 )  return (name = "VISIOCYTEPluginInterface/2.0");

    VISIOCYTEPluginInterface *iface = qobject_cast<VISIOCYTEPluginInterface *>(plugin);
    if (iface )  return (name = "VISIOCYTEPluginInterface/1.1");

    return name;
}

QStringList visiocyte_getInterfaceMenuList(QObject *plugin)
{
	QStringList qslist;

	VISIOCYTESingleImageInterface2_1 *iFilter2_1 = qobject_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
    if (iFilter2_1 )  return (qslist = iFilter2_1->menulist());

	VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(plugin);
    if (iFilter )  return (qslist = iFilter->menulist());

    VISIOCYTEPluginInterface *iface = qobject_cast<VISIOCYTEPluginInterface *>(plugin);
    if (iface )  return (qslist = iface->menulist());

    VISIOCYTEPluginInterface2 *iface2 = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
    if (iface2 )  return (qslist = iface2->menulist());

    VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
    if (iface2_1 )  return (qslist = iface2_1->menulist());

    return qslist;
}

QStringList visiocyte_getInterfaceFuncList(QObject *plugin)
{
	QStringList qslist;

    VISIOCYTEPluginInterface *iface = qobject_cast<VISIOCYTEPluginInterface *>(plugin);
    if (iface )  return (qslist = iface->funclist());

    VISIOCYTEPluginInterface2 *iface2 = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
    if (iface2 )  return (qslist = iface2->funclist());

    VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
    if (iface2_1 )  return (qslist = iface2_1->funclist());

    return qslist;
}

//==============================================================

//void VisiocytePluginMenu::mousePressEvent ( QMouseEvent * e )
//{
//    visiocyte_msg("enter mouse press");
//    if (ploader)
//        ploader->rescanPlugins();
//    QMenu::mousePressEvent(e);
//};

//===============================================================

Visiocyte_PluginLoader::Visiocyte_PluginLoader(QMenu* menuPlugin, MainWindow* mainwindow)
{
	this->visiocyte_menuPlugin = menuPlugin;
	this->visiocyte_mainwindow = mainwindow;

	pluginList.clear();

	rescanPlugins();
}

Visiocyte_PluginLoader::Visiocyte_PluginLoader(MainWindow* mainwindow)
{
    if (mainwindow) //20130904
    {
        this->visiocyte_mainwindow = mainwindow;
        this->visiocyte_menuPlugin = mainwindow->pluginProcMenu; //changed from 0, 20130904, PHC
    }
}


//QObject * QPluginLoader::instance () returns the root component object of the plugin.
//The plugin is loaded if necessary.
//If the root component object was destroyed, calling this function creates a new instance.
//The root component, returned by this function, is not deleted when the QPluginLoader is destroyed.
//If you want to ensure that the root component is deleted, you should call unload() as soon you don't need to access the core component anymore.
//When the library is finally unloaded, the root component will automatically be deleted.

void Visiocyte_PluginLoader::clear()
{
	plugin_menu.clear();
	foreach (QPluginLoader* loader, pluginList)
    {
        //while (loader->isLoaded())
        	loader->unload();

        delete loader;
    }
	pluginList.clear();

	pluginFilenameList.clear();
}

//	foreach (QPluginLoader* loader, pluginList)
//    {
//        //100805 RZC: try to unload root instance of this plugin
//        while (loader->isLoaded())
//        {
//        	loader->unload();
//        }
////        QObjectList list = loader->staticInstances();
////        for ( ; list.size()>0; )
////        {
////        	loader->unload();
////        	list = loader->staticInstances();
////        }
//    }

void Visiocyte_PluginLoader::rescanPlugins()
{
    clear();

	loadPlugins();

	populateMenus();
}

// Return a list of directories that will be searched for plugins
QList<QDir> Visiocyte_PluginLoader::getPluginsDirList()
{
    QList<QDir> pluginsDirList;

    pluginsDirList.clear();
	QDir testPluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (testPluginsDir.dirName().toLower() == "debug" || testPluginsDir.dirName().toLower() == "release")
        testPluginsDir.cdUp();

	qDebug() << testPluginsDir.absolutePath();
#elif defined(Q_OS_MAC)
    // In a Mac app bundle, plugins directory could be either
    //  a - below the actual executable i.e. visiocyte.app/Contents/MacOS/plugins/
    //  b - parallel to visiocyte.app i.e. foo/visiocyte.app and foo/plugins/
    if (testPluginsDir.dirName() == "MacOS") {
        QDir testUpperPluginsDir = testPluginsDir;
        testUpperPluginsDir.cdUp();
        testUpperPluginsDir.cdUp();
        testUpperPluginsDir.cdUp(); // like foo/plugins next to foo/visiocyte.app
        if (testUpperPluginsDir.cd("plugins"))
            pluginsDirList.append(testUpperPluginsDir);
    }
#endif
    if (testPluginsDir.cd("plugins"))
        pluginsDirList.append(testPluginsDir);

    return pluginsDirList;
}

void Visiocyte_PluginLoader::loadPlugins()
{
	QAction *plugin_manager = new QAction(tr("Plug-in manager"), this);
	connect(plugin_manager, SIGNAL(triggered()), this, SLOT(aboutPlugins()));
    QAction *plugin_rescan = new QAction(tr("Re-scan all plugins"), this);
	connect(plugin_rescan, SIGNAL(triggered()), this, SLOT(rescanPlugins()));
    QAction * plugin_clear = new QAction(tr("Clear used plugins history"),this);
    connect(plugin_clear, SIGNAL(triggered()), this, SLOT(clear_recentPlugins()));
	{
		plugin_menu.addAction(plugin_manager);
		plugin_menu.addAction(plugin_rescan);

        addrecentPlugins(&plugin_menu); //added by Zhi Z 20140721
        updated_recentPlugins();

        plugin_menu.addAction(plugin_clear);
		plugin_menu.addSeparator();
	}

    QList<QDir> pluginsDirList = getPluginsDirList();

    if (pluginsDirList.size() == 0)
    {
    	qDebug("Cannot find ./plugins directory!");
        return;
    }

    qDebug("Searching in ./plugins ...... ");
    foreach (const QDir& pluginsDir, pluginsDirList)
    {
    	searchPluginDirs(&plugin_menu, pluginsDir);
        searchPluginFiles(&plugin_menu, pluginsDir);
        qDebug("Searching ./plugins done.");
    }
}

//added by Zhi Z 20140721
void Visiocyte_PluginLoader::addrecentPlugins(QMenu* menu)
{
    if (! menu)  return;
    QSettings settings("HHMI", "Visiocyte");

    recentpluginsList = settings.value("recentPluginList").toStringList();
    recentpluginsIndex = settings.value("recentPluginIndex").toList();

    plugin_recent = new QMenu(tr("Recently used plugins"));
    plugin_most = new QMenu(tr("Most used plugins"));
    menu->addMenu(plugin_recent);
    menu->addMenu(plugin_most);

}
void Visiocyte_PluginLoader::updated_recentPlugins()
{
    plugin_recent->clear();
    plugin_most->clear();

    if(recentpluginsIndex.size()>0)
    {
        QList<QVariant> recentpluginsIndex_temp = recentpluginsIndex;
        QList <int> sort_index;

        for(VISIOCYTELONG i = 0; i < recentpluginsList.size(); i++)
            sort_index.append(i);

        if(recentpluginsIndex.size()>1)
        {
            for(VISIOCYTELONG i = 0; i < recentpluginsList.size(); i++)
            {
                if (i > 0)
                {
                    VISIOCYTELONG j = i;
                    while(j > 0 && recentpluginsIndex_temp.at(j-1).toInt()<recentpluginsIndex_temp.at(j).toInt())
                    {
                        recentpluginsIndex_temp.swap(j,j-1);
                        sort_index.swap(j,j-1);
                        j--;
                    }
                }
            }
        }

        QRegExp reg("%");
        for(int i = 0; i < recentpluginsList.size(); i++)
        {
            QStringList plugininfo = recentpluginsList.at(i).split(reg);
            QAction *action = new QAction(plugininfo.at(0),this);
            connect(action, SIGNAL(triggered()), this, SLOT(runRecentPlugin()));
            plugin_recent->addAction(action);

            QStringList plugininfo_index = recentpluginsList.at(sort_index[i]).split(reg);
            QAction *action_index = new QAction(plugininfo_index.at(0),this);
            connect(action_index, SIGNAL(triggered()), this, SLOT(runRecentPlugin()));
            plugin_most->addAction(action_index);
        }
    }
}

void Visiocyte_PluginLoader::runRecentPlugin()
{
    QAction *action = qobject_cast<QAction *>(sender());
    int i;
    QRegExp reg("%");
    QStringList plugininfo;
    for(i = 0; i< recentpluginsList.size(); i++)
    {
        plugininfo = recentpluginsList.at(i).split(reg);
        if(plugininfo.at(0) == action->text())
            break;
    }
    QPluginLoader* loader = new QPluginLoader(plugininfo.at(1));
    return runPlugin(loader,plugininfo.at(0));
}

void Visiocyte_PluginLoader::clear_recentPlugins()
{
    if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to clear the history of used plugins?"), QMessageBox::Yes, QMessageBox::No))
    {

        QSettings settings("HHMI", "Visiocyte");
        settings.remove("recentPluginList");
        settings.remove("recentPluginIndex");

        plugin_recent->clear();
        plugin_most->clear();
    }

}

void Visiocyte_PluginLoader::searchPluginDirs(QMenu* menu, const QDir& pluginsDir)
{
	if (! menu)  return;

    QStringList dirList = pluginsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString dirName, dirList)
    {
        QMenu* submenu = new QMenu(dirName);
        if (! submenu)  return;
        menu->addMenu(submenu);

        QDir subDir = pluginsDir;
        subDir.cd(dirName);

        searchPluginDirs(submenu, subDir);
        searchPluginFiles(submenu, subDir);
    }
}

void Visiocyte_PluginLoader::searchPluginFiles(QMenu* menu, const QDir& pluginsDir)
{
	if (!menu)  return;

    QStringList fileList = pluginsDir.entryList(QDir::Files);
    foreach (QString fileName, fileList)
    {
        QString fullpath = pluginsDir.absoluteFilePath(fileName);

		// Skip older versions that have been backed up with ".old" suffix.
		if (fullpath.endsWith(".old")) continue;
		if (fullpath.endsWith(".new")) continue;

        QPluginLoader* loader = new QPluginLoader(fullpath);
        if (! loader)
        {
            qDebug("ERROR in Visiocyte_PluginLoader::searchPluginFiles: new QPluginLoader(%s)", qPrintable(fullpath));
            return;
        }

        QObject *plugin = loader->instance(); //a new instance
        if (plugin)
        {
            //qDebug()<< "plugin: " << fullpath;
            pluginFilenameList += fullpath;

            pluginList.append(loader); /////

            //--------------------------------------------------
            QString iname = visiocyte_getInterfaceName(plugin);
            if (iname.size())
            {
                //addToMenu(menu, plugin, visiocyte_getInterfaceFeatures(plugin), SLOT(runPlugin()));
                addToMenu(menu, loader, visiocyte_getInterfaceMenuList(plugin), SLOT(runPlugin()));
            }
            //----------------------------------------------------
        }
        else
        {
            qDebug() << "Fail instantiation("<<loader->errorString().toStdString().c_str()<<"): " <<fullpath;

            //qDebug() << "Fail instantiation: " <<fullpath;
        }

        //unload or left ? is a problem
        //loader->unload();     //qDebug() << "unload: " <<fileName;
    }
}

void Visiocyte_PluginLoader::addToMenu(QMenu *menu,
		QObject *plugin, const QStringList &texts, const char *member)
{
    foreach (QString text, texts)
    {
		if (text.startsWith("HIDDEN"))
			continue; //do not add some hidden menu items // by PHC, 2010-Dec-16
			
        QAction *action = new QAction(text, plugin);
        connect(action, SIGNAL(triggered()), this, member);
        menu->addAction(action);
    }
}

//hook menu to visiocyte, called by rescanPlugins, MainWindow::updateProcessingMenu
void Visiocyte_PluginLoader::populateMenus()
{
	if (visiocyte_menuPlugin)
	{
		QAction* a = visiocyte_menuPlugin->menuAction();
		a->setMenu(&plugin_menu);
	}
}

//=========================================================

void Visiocyte_PluginLoader::aboutPlugins()
{
    QList<QDir> pluginsDirList = getPluginsDirList();
    PluginDialog dialog("Visiocyte", pluginsDirList, pluginFilenameList, visiocyte_mainwindow);
    dialog.exec();
}

void Visiocyte_PluginLoader::runPlugin(QPluginLoader *loader, const QString & menuString)
{
    if (!loader)
    {
    	visiocyte_msg("ERROR in Visiocyte_PluginLoader::runPlugin: invalid pointer to the plugin loader detected.");
    	return;
    }
	

#if QT_VERSION < 0x040806 // MK, 09242017, attempting to solve plugin issue on Windows with Qt4.8 and higher. Still not sure why it's ok with Qt4.7.
	loader->unload();
#endif

	QObject *plugin = loader->instance();
    if (!plugin)
    {
    	visiocyte_msg("ERROR in Visiocyte_PluginLoader::runPlugin: loader->instance()");
    	return;
    }
	
    //added by Zhi Z, 20140724
    QSettings settings("HHMI", "Visiocyte");
    recentpluginsList = settings.value("recentPluginList").toStringList();
    recentpluginsIndex = settings.value("recentPluginIndex").toList();

    QString CurrentpluginInfo = menuString + "%" + loader->fileName();
    int flag = 0;
    for(int i = 0; i < pluginFilenameList.size();i++)
    {
        if(pluginFilenameList.at(i) == loader->fileName())
        {
            flag = 1;
            break;
        }
    }

    if(flag == 1)
    {
        int currentIndex = 0;

        if(recentpluginsIndex.size() > 0)
        {
            for(int i=0; i< recentpluginsList.size(); i++)
            {
                if(recentpluginsList.at(i) == CurrentpluginInfo)
                {
                    currentIndex = recentpluginsIndex.at(i).toInt();
                    recentpluginsList.removeAt(i);
                    recentpluginsIndex.removeAt(i);
                    break;
                }
            }
        }
        recentpluginsList.prepend(CurrentpluginInfo);
        recentpluginsIndex.prepend(currentIndex + 1);

        settings.setValue("recentPluginList", recentpluginsList);
        settings.setValue("recentPluginIndex", recentpluginsIndex);

        updated_recentPlugins();
        plugin_menu.update();
    }

    bool done = false;
	if (!done)  { done = runPluginInterface2_1(plugin, menuString); visiocyte_msg("done with runPluginInterface2_1().",0); }
	if (!done)  { done = runPluginInterface2(plugin, menuString); visiocyte_msg("done with runPluginInterface2().",0); }
	if (!done)  { done = runPluginInterface(plugin, menuString); visiocyte_msg("done with runPluginInterface().",0); }
    // runSingleImageInterface works with both 1.0 and 2.1
    if (!done)  { done = runSingleImageInterface(plugin, menuString); visiocyte_msg("done with runSingleImageInterface().",0); }
	//
    if (!done)  {visiocyte_msg("No interface found.",0);}

	
	visiocyte_msg(QString("already run! done status=%1").arg(done), 0);
	// 100804 RZC: MUST do not unload plug-ins that has model-less dialog
	//    if (loader->isLoaded())
	//    {
	//    	loader->unload();      qDebug() << "unload: " <<fileName;
	//    }
}

void Visiocyte_PluginLoader::runPlugin()
{
    QAction *action = qobject_cast<QAction *>(sender());
    //VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(action->parent());
    QPluginLoader *loader = qobject_cast<QPluginLoader *>(action->parent());
    if (!loader)
    {
    	visiocyte_msg("ERROR in Visiocyte_PluginLoader::runPlugin: qobject_cast<QPluginLoader *>");
    	return;
    }

	return runPlugin(loader, action->text());
}

bool Visiocyte_PluginLoader::runSingleImageInterface(QObject* plugin, const QString &command)
{
    // if (plugin == NULL) {visiocyte_msg("plugin is null", 0);}
    // VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(plugin);
    // if (iFilter == NULL) {visiocyte_msg("plugin cannot be cast to VISIOCYTESingleImageInterface*", 0);}
    // VISIOCYTESingleImageInterface2_1 *iFilter2 = qobject_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
    // if (iFilter2 == NULL) {visiocyte_msg("plugin cannot be cast to VISIOCYTESingleImageInterface2_1*", 0);}
    // VISIOCYTESingleImageInterface *iFilter3 = dynamic_cast<VISIOCYTESingleImageInterface *>(plugin);
    // if (iFilter2 == NULL) {visiocyte_msg("plugin cannot be dynamic_cast to VISIOCYTESingleImageInterface*", 0);}
    
    // For some reason dynamic_cast works, but qobject_cast fails, when plugin is VISIOCYTESingleImageInterface2_1 //this should be wrong. by PHC 110705

    VISIOCYTESingleImageInterface2_1 *iFilter21 = qobject_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
    VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(plugin);

    if (!iFilter21 && !iFilter)
    {
       return false;
    }

    bool done = true;

    qDebug()<<"VisiocyteSingleImageInterface or VisiocyteSingleImageInterface2_1..."<<iFilter;

    if (visiocyte_mainwindow)
    {
        My4DImage* image = visiocyte_mainwindow->currentImage();
		if (!image)
		{
			visiocyte_msg("No image is open.");
			return done;
		}

		//make a copy of the property of the image input.
		VISIOCYTELONG szx0 = image->getXDim(), szy0 = image->getYDim(), szz0 = image->getZDim(), szc0 = image->getCDim();
		ImagePixelType datatype0 = image->getDatatype();
		unsigned char *dataptr0 = image->getRawData();

        try
        {
        	if (iFilter21)
               	iFilter21->processImage(command, (Image4DSimple*)image, (QWidget*)0); //do not pass the mainwindow widget
            else
               	iFilter->processImage(command, (Image4DSimple*)image, (QWidget*)0); //do not pass the mainwindow widget
        }
        catch (...)
        {
        	visiocyte_msg(QString("The plugin [%1] fails to run. Check your plugin code please.").arg(command));
        }

		//check if any of the image properties changes; if yes, then also regenerate all 4D image pointers, etc
		VISIOCYTELONG szx_new = image->getXDim(), szy_new = image->getYDim(), szz_new = image->getZDim(), szc_new = image->getCDim();
		ImagePixelType datatype_new = image->getDatatype();
		unsigned char *dataptr_new = image->getRawData();
		if (dataptr0!=dataptr_new || szx0!=szx_new || szy0!=szy_new || szz0!=szz_new || szc0!=szc_new || datatype0!=datatype_new)
		{
			try
			{
				unsigned char * datanew = new unsigned char [image->getTotalBytes()];
				memcpy(datanew, image->getRawData(), image->getTotalBytes());

				//has to set the original size back first, otherwise the delete 4d pointer will not be correct
				image->setXDim(szx0);image->setYDim(szy0);image->setZDim(szz0);image->setCDim(szc0);image->setDatatype(datatype0);
				if (image->setNewImageData(datanew, szx_new, szy_new, szz_new, szc_new, datatype_new)==false)
				{
					visiocyte_msg("Fail to update the new image content returned by the plugin to the window.");
					return done;
				}
			}
			catch (...)
			{
				visiocyte_msg("Fail to allocate temporary memory of some other errors for handling the returned image contents of the plugin.");
				return done;
			}
		}
		else 
		{
			image->updateminmaxvalues(); //since the data have been changed, thus the min max values should be updated even the sizes remain the same
		}

		//visiocyte_msg(QString("after %1 %2 %3 %4").arg(image->getXDim()).arg(image->getYDim()).arg(image->getZDim()).arg(image->getCDim()));

        if (image)  image->updateViews();
        return done;
    }
    return done;
}

bool Visiocyte_PluginLoader::runPluginInterface(QObject* plugin, const QString& command)
{
    VISIOCYTEPluginInterface *iface = qobject_cast<VISIOCYTEPluginInterface *>(plugin);
	VISIOCYTEPluginCallback *callback = dynamic_cast<VISIOCYTEPluginCallback *>(this);

        qDebug()<<"runPluginInterface ..."<<iface;

	if (iface && callback)
    {
        try
        {
        	iface->domenu(command, *callback, (QWidget*)0); //do not pass the mainwindow widget
        }
        catch (...)
        {
        	visiocyte_msg(QString("The plugin fails to run [%1] . Check your plugin code please.").arg(command));
        }
        return true;
    }
	return false;
}

bool Visiocyte_PluginLoader::runPluginInterface2(QObject* plugin, const QString& command)
{
    VISIOCYTEPluginInterface2 *iface = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
	VISIOCYTEPluginCallback2 *callback = dynamic_cast<VISIOCYTEPluginCallback2 *>(this);

        qDebug()<<"runPluginInterface2 ..."<<iface;

	if (iface && callback)
    {
        try
        {
        	iface->domenu(command, *callback, (QWidget*)0); //do not pass the mainwindow widget
        }
        catch (...)
        {
        	visiocyte_msg(QString("The plugin fails to run [%1]. Check your plugin code please.").arg(command));
        }
        return true;
    }
	return false;
}

bool Visiocyte_PluginLoader::runPluginInterface2_1(QObject* plugin, const QString& command)
{
    VISIOCYTEPluginInterface2_1 *iface = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
	VISIOCYTEPluginCallback2 *callback = dynamic_cast<VISIOCYTEPluginCallback2 *>(this);

        qDebug()<<"runPluginInterface2_1 ..."<<iface;

	if (iface && callback)
    {
        try
        {
        	iface->domenu(command, *callback, (QWidget*)0); //do not pass the mainwindow widget
        }
        catch (...)
        {
        	visiocyte_msg(QString("The plugin fails to run [%1]. Check your plugin code please.").arg(command));
        }
        return true;
    }
	return false;
}

//====================================================================

bool Visiocyte_PluginLoader::callPluginFunc(const QString &plugin_name,
		const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output)
{
    if (pluginFilenameList.isEmpty()) //added by PHC 20130904 to avoid duplicated menu of YuY's code below
        loadPlugins(); // ensure pluginFilenameList unempty 20110520 YuY
	
	QString fullpath;
    QList<QDir> pluginsDirList = getPluginsDirList();

    QStringList existingPluginsList = getPluginNameList();

    foreach (const QDir& pluginsDir, pluginsDirList)
    {
//        // Find the first plugin directory with such a file
//        if (pluginsDir.exists(plugin_name)) {
//            fullpath = pluginsDir.absoluteFilePath(plugin_name);
//            break;
//        }

//the following was sugegsted by Zhi Zhou to do partial name match. 20130706

        std::cout << pluginsDir.dirName().toStdString() << std::endl;
        // Find the first plugin directory with such a file name or partial name by  Zhi Zhou 20130705
        foreach (QString file, existingPluginsList )
        if (file.contains(plugin_name))
        {
            fullpath = pluginsDir.absoluteFilePath(file);
            break;
        }
    }
	qDebug()<<"callPluginFunc fullpath: " <<fullpath;
	int idx = pluginFilenameList.indexOf(fullpath);
	//qDebug()<<"callPluginFunc idx: " <<idx;
	if (idx < 0)
	{
		qDebug()<<QString("ERROR: callPluginFunc cannot find this plugin_name: '%1'").arg(plugin_name);
		return false;
	}

	Q_ASSERT(idx>=0 && idx<pluginList.size());
	QPluginLoader *loader = pluginList.at(idx);

	loader->unload(); ///
    QObject *plugin = loader->instance();
    if (! plugin)
    {
    	qDebug("ERROR in Visiocyte_PluginLoader::callPluginFunc: loader->instance()");
    	return false;
    }

//	QStringList funclist = visiocyte_getInterfaceFuncList(plugin);
//	if (! funclist.contains(func_name))
//	{
//		qDebug()<<Qstring("ERROR: callPluginFunc cannot find this func_name: '%1' in '%2'").arg(func_name).arg(plugin_name);
//		return false;
//	}

	VISIOCYTEPluginInterface2 *iface = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
	VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
	VISIOCYTEPluginCallback2 *callback = dynamic_cast<VISIOCYTEPluginCallback2 *>(this);
    if (iface && callback)
    {
        try
        {
            return iface->dofunc(func_name, input, output, *callback, (QWidget*)0);
        }
        catch (...)
        {
            visiocyte_msg(QString("The plugin fails to call [%1]. Check your plugin code please.").arg(func_name));
            return false;
        }
        return true;
    }
    else if (iface2_1 && callback) {
         try
        {
            return iface2_1->dofunc(func_name, input, output, *callback, (QWidget*)0);
        }
        catch (...)
        {
            visiocyte_msg(QString("The plugin fails to call [%1]. Check your plugin code please.").arg(func_name));
            return false;
        }
        return true;
    }
    else // (! (iface && callback) )
	{
		qDebug()<<QString("ERROR: callPluginFunc cannot cast (VisiocytePluginInterface2_1) of plugin '%1'").arg(plugin_name);
		return false;
	}

}

////a bug caused in v2.823?
/*
bool Visiocyte_PluginLoader::callPluginMenu(const QString &plugin_name,
                                      const QString &menu_name)
{
    
	loadPlugins(); // ensure pluginFilenameList unempty 20110520 YuY
	
	QString fullpath;
    QList<QDir> pluginsDirList = getPluginsDirList();
    foreach (const QDir& pluginsDir, pluginsDirList)
    {
        // Find the first plugin directory with such a file
        if (pluginsDir.exists(plugin_name)) {
            fullpath = pluginsDir.absoluteFilePath(plugin_name);
            break;
        }
    }
	qDebug()<<"callPluginMenu fullpath: " <<fullpath;
	int idx = pluginFilenameList.indexOf(fullpath);
	//qDebug()<<"callPluginFunc idx: " <<idx;
	if (idx < 0)
	{
		qDebug()<<QString("ERROR: callPluginFunc cannot find this plugin_name: '%1'").arg(plugin_name);
		return false;
	}
    
	Q_ASSERT(idx>=0 && idx<pluginList.size());
	QPluginLoader *loader = pluginList.at(idx);
    
	loader->unload(); ///
    QObject *plugin = loader->instance();
    if (! plugin)
    {
    	qDebug("ERROR in Visiocyte_PluginLoader::callPluginMenu: loader->instance()");
    	return false;
    }
    
	VISIOCYTEPluginInterface2 *iface = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
	VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
	VISIOCYTEPluginCallback2 *callback = dynamic_cast<VISIOCYTEPluginCallback2 *>(this);
    if (iface && callback) {
        try
        {
            iface->domenu(menu_name, *callback, (QWidget*)0);
            return true;
        }
        catch (...)
        {
            visiocyte_msg(QString("The plugin fails to call [%1]. Check your plugin code please.").arg(menu_name));
            return false;
        }
        return true;
    }
    else if (iface2_1 && callback) {
        try
        {
            iface2_1->domenu(menu_name, *callback, (QWidget*)0);
            return true;
        }
        catch (...)
        {
            visiocyte_msg(QString("The plugin fails to call [%1]. Check your plugin code please.").arg(menu_name));
            return false;
        }
        return true;
    }
    else // (! (iface && callback) )
	{
		qDebug()<<QString("ERROR: callPluginMenu cannot cast (VisiocytePluginInterface2_1) of plugin '%1'").arg(plugin_name);
		return false;
	}
}

*/

//==================================================================


#define __get_or_push_with_visiocyte__
visiocytehandleList Visiocyte_PluginLoader::getImageWindowList() const
{
	visiocytehandleList list;
	if (visiocyte_mainwindow)
	{
		list = visiocyte_mainwindow->allWindowList();
	}
	return list;
}
visiocytehandle Visiocyte_PluginLoader::currentImageWindow()
{
	XFormWidget* w = 0;
	if (visiocyte_mainwindow)
	{
		w = visiocyte_mainwindow->currentImageWindow();
	}
	return visiocytehandle(w);
}
visiocytehandle Visiocyte_PluginLoader::curHiddenSelectedWindow()
{
	XFormWidget* w = 0;
	if (visiocyte_mainwindow)
	{
		w = visiocyte_mainwindow->curHiddenSelectedWindow();
	}
	return visiocytehandle(w);
}
visiocytehandle Visiocyte_PluginLoader::newImageWindow(QString name)
{
	XFormWidget* w = 0;
	if (visiocyte_mainwindow)
	{
		w = visiocyte_mainwindow->newImageWindow(name);
		qDebug() << "Visiocyte_PluginLoader newImageWindow: " << w;
	}
	return visiocytehandle(w);
}
void Visiocyte_PluginLoader::updateImageWindow(visiocytehandle image_window, bool b_forceUpdateChannelMinMaxValues) //by PHC, 20120412
{
	if (visiocyte_mainwindow)
	{
		XFormWidget* w = visiocyte_mainwindow->updateImageWindow(image_window, b_forceUpdateChannelMinMaxValues);
		qDebug() << "Visiocyte_PluginLoader updateImageWindow: " << w <<"/"<< image_window;
	}

	pumpEvents(); //100804 RZC
}


QString Visiocyte_PluginLoader::getImageName(visiocytehandle image_window) const
{
	QString name;
	if (visiocyte_mainwindow)
	{
		name = visiocyte_mainwindow->getWindowName(image_window);
	}
	return name;
}
void Visiocyte_PluginLoader::setImageName(visiocytehandle image_window, QString name)
{
	if (visiocyte_mainwindow)
	{
		XFormWidget* w = visiocyte_mainwindow->setImageName(image_window, name);
		qDebug() << "Visiocyte_PluginLoader setImageName: " << w <<"/"<< image_window;
	}
}


Image4DSimple* Visiocyte_PluginLoader::getImage(visiocytehandle image_window)
{
	My4DImage* image = 0;
	if (visiocyte_mainwindow)
	{
		image = visiocyte_mainwindow->getImage(image_window);
	}
	return (Image4DSimple*)image;
}
bool Visiocyte_PluginLoader::setImage(visiocytehandle image_window, Image4DSimple* image)
{
	if (!image)
	{
		visiocyte_msg("The new image pointer is invalid in Visiocyte_PluginLoader::setImage(). Do nothing.\n",0);
		return false;
	}
	if (visiocyte_mainwindow)
	{
		qDebug() << "Visiocyte_PluginLoader setImage: " << image_window << image;
		Image4DSimple *targetP = visiocyte_mainwindow->getImage(image_window);
		if (!targetP) //in this case, the pointers cannot be the same, and thus directly set data
    {
			return visiocyte_mainwindow->setImage(image_window, image);
    }
		else
		{
			if (targetP==image ||     // in this case no need for further judgment as the data MUST be the same
	                    image->isSameDataBuffer(targetP) )
			{
				visiocyte_msg("You try to set the same image pointer to itself; thus nothing is needed. \n",0);
				return true;
			}  //in this case the data buffer would be updated directly, and thus no need to update the actual data
                           //note that a possible bug is that if the plugin developer change the dimensions/datatype of the data
                           //buffer, but still keep the buffer pointer unchanged. Then here we should recreate the 4d pointers.
                           // This is a to-do in the near future. by PHC. 2010-08-01.

			return visiocyte_mainwindow->setImage(image_window, image);
		}
	}
	return false;
}

bool Visiocyte_PluginLoader::setImageTest(visiocytehandle image_window, Image4DSimple* image, unsigned char *a)
{
	if (visiocyte_mainwindow)
	{
		qDebug() << "Visiocyte_PluginLoader setImage: " << image_window << image;

		return visiocyte_mainwindow->setImage(image_window, image);
	}
	return false;
}


LandmarkList  Visiocyte_PluginLoader::getLandmark(visiocytehandle image_window)
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->getLandmark(image_window);
	}
	return LandmarkList();
}
bool Visiocyte_PluginLoader::setLandmark(visiocytehandle image_window, LandmarkList& landmark_list)
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->setLandmark(image_window, landmark_list);
	}
	return false;
}


ROIList Visiocyte_PluginLoader::getROI(visiocytehandle image_window)
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->getROI(image_window);
	}
	return ROIList();
}
bool Visiocyte_PluginLoader::setROI(visiocytehandle image_window, ROIList & roi_list)
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->setROI(image_window, roi_list);
	}
	return false;
}

NeuronTree Visiocyte_PluginLoader::getSWC(visiocytehandle image_window)
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->getSWC(image_window);
	}
	return NeuronTree();
}
bool Visiocyte_PluginLoader::setSWC(visiocytehandle image_window, NeuronTree & nt)
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->setSWC(image_window, nt);
	}
	return false;
}

Image4DSimple * Visiocyte_PluginLoader::loadImage(char *filename)  //2013-08-09. two more functions for simplied calls to use Visiocyte's image loading and saving functions without linking to additional libs
{
    if (!filename)
        return 0;

    Image4DSimple * myimg = new Image4DSimple;
    myimg->loadImage(filename, false); //first try libtiff
    if (myimg->valid()==false) //add this double-loading as of 140630 to cope with Zhi's request
    {
        myimg->loadImage(filename, true); //add using mylib support 20131105
        if (myimg->valid())
            return myimg;
        else
            return 0;
    }
    else
        return myimg;
}

Image4DSimple * Visiocyte_PluginLoader::loadImage(char *filename, VISIOCYTELONG zsliceno)  //2013-11-02
{
    if (!filename)
        return 0;

    if (zsliceno<0)
        return loadImage(filename);

    bool b_UseMyLib=true;
    Image4DSimple * myimg = new Image4DSimple;
    myimg->loadImage_slice(filename, b_UseMyLib, zsliceno);
    if (myimg->valid())
        return myimg;
    else
        return 0;
}


bool Visiocyte_PluginLoader::saveImage(Image4DSimple * img, char *filename)
{
    if (!img || !filename || !img->valid())
        return false;

    return img->saveImage(filename);
}



VISIOCYTE_GlobalSetting Visiocyte_PluginLoader::getGlobalSetting()
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->getGlobalSetting();
	}
	return VISIOCYTE_GlobalSetting();
}
bool Visiocyte_PluginLoader::setGlobalSetting( VISIOCYTE_GlobalSetting & gs )
{
	if (visiocyte_mainwindow)
	{
		return visiocyte_mainwindow->setGlobalSetting(gs);
	}
	return false;
}

QStringList Visiocyte_PluginLoader::getPluginNameList()
{
	return pluginFilenameList;
}


////////////////////////////////////////////////////////////////////////////
//100810 RZC: add this macro
#define if_XFormWidget(w, image_window) \
	XFormWidget* w=0; \
	if (visiocyte_mainwindow && (w = visiocyte_mainwindow->validateImageWindow(image_window)))


void Visiocyte_PluginLoader::open3DWindow(visiocytehandle image_window)
{
	if_XFormWidget(w, image_window)
	{
		w->open3DWindow();
		qDebug() << "Visiocyte_PluginLoader open3DWindow: " << w <<"/"<< image_window;
	}
}
void Visiocyte_PluginLoader::close3DWindow(visiocytehandle image_window)
{
	if_XFormWidget(w, image_window)
	{
		w->close3DWindow();
		qDebug() << "Visiocyte_PluginLoader close3DWindow: " << w <<"/"<< image_window;

		pumpEvents(); //100804 RZC
	}
}
void Visiocyte_PluginLoader::openROI3DWindow(visiocytehandle image_window)
{
	if_XFormWidget(w, image_window)
	{
		w->openROI3DWindow();
		qDebug() << "Visiocyte_PluginLoader openROI3DWindow: " << w <<"/"<< image_window;
	}
}
void Visiocyte_PluginLoader::closeROI3DWindow(visiocytehandle image_window)
{
	if_XFormWidget(w, image_window)
	{
		w->closeROI3DWindow();
		qDebug() << "Visiocyte_PluginLoader closeROI3DWindow: " << w <<"/"<< image_window;

		pumpEvents(); //100804 RZC
	}
}

VisiocyteR_MainWindow * Visiocyte_PluginLoader::open3DViewerForSingleSurfaceFile(QString fileName) //By PHC 20150210
{
    if (visiocyte_mainwindow)
    {
        QFileInfo curfile_info(fileName);
        QString cur_suffix = curfile_info.suffix().toUpper();
        if (cur_suffix=="APO" ||
                 cur_suffix=="SWC" ||
                 cur_suffix=="ESWC" ||
                 cur_suffix=="OBJ" ||
                 cur_suffix=="VISIOCYTES" ||
                 cur_suffix=="VISIOCYTES" ||
                 cur_suffix=="NULL3DVIEWER" || fileName=="NULL3DVIEWER")
        {
            visiocyte_mainwindow->loadVISIOCYTEFile(fileName, true, this->visiocyte_mainwindow->global_setting.b_autoOpenImg3DViewer);
            return visiocyte_mainwindow->find3DViewer(fileName);
        }
    }

    return 0;
}

VisiocyteR_MainWindow * Visiocyte_PluginLoader::open3DViewerForLinkerFile(QString fileName) //By PHC 20150210
{
    if (visiocyte_mainwindow)
    {
        QFileInfo curfile_info(fileName);
        QString cur_suffix = curfile_info.suffix().toUpper();
        if (cur_suffix=="ANO")
        {
            visiocyte_mainwindow->loadVISIOCYTEFile(fileName, true, this->visiocyte_mainwindow->global_setting.b_autoOpenImg3DViewer);
            return visiocyte_mainwindow->find3DViewer(fileName);
        }
    }

    return 0;
}

VisiocyteR_MainWindow * Visiocyte_PluginLoader::createEmpty3DViewer() //By PHC 20150210
{
    if (visiocyte_mainwindow)
    {
        VisiocyteR_MainWindow *p = open3DViewerForSingleSurfaceFile("noname.NULL3DVIEWER");
        return p;
    }

    return 0;
}

void Visiocyte_PluginLoader::moveWindow(VisiocyteR_MainWindow *w, int x, int y)
{
    if (visiocyte_mainwindow )
    {
      w->move(x,y);
    }
}

void Visiocyte_PluginLoader::resizeWindow(VisiocyteR_MainWindow *w, int x, int y)
{
    if (visiocyte_mainwindow )
    {
      w->resize(x,y);
    }
}
void Visiocyte_PluginLoader::setHideDisplayControlButton(VisiocyteR_MainWindow *w)
{
    if(visiocyte_mainwindow)
    {
        w->hideDisplayControls();
    }
}

//void Visiocyte_PluginLoader::setResizeEvent(VisiocyteR_MainWindow *w, int x, int y)
//{
//    if(visiocyte_mainwindow)
//    {
//        w->resizeEvent((QResizeEvent *)QSize(x,y));
//    }
//}

void Visiocyte_PluginLoader::setWindowDataTitle(VisiocyteR_MainWindow * w, QString title)
{
    if (visiocyte_mainwindow )
    {
      w->setDataTitle(title);
    }
}

QString Visiocyte_PluginLoader::getWindowDataTitle(VisiocyteR_MainWindow * w)
{
    if (visiocyte_mainwindow )
    {
      return w->getDataTitle();
    }
}



void Visiocyte_PluginLoader::pushObjectIn3DWindow(visiocytehandle image_window)
{
	if_XFormWidget(w, image_window)
	{
		w->pushObjectIn3DWindow();
		qDebug() << "Visiocyte_PluginLoader pushObjectIn3DWindow: " << w <<"/"<< image_window;
	}
}

void Visiocyte_PluginLoader::pushImageIn3DWindow(visiocytehandle image_window)
{
	if_XFormWidget(w, image_window)
	{
		w->pushImageIn3DWindow();
		qDebug() << "Visiocyte_PluginLoader pushImageIn3DWindow: " << w <<"/"<< image_window;
	}
}

int Visiocyte_PluginLoader::pushTimepointIn3DWindow(visiocytehandle image_window, int timepoint)
{
	int t=0;
	if_XFormWidget(w, image_window)
	{
		t = w->pushTimepointIn3DWindow(timepoint);
		qDebug() << "Visiocyte_PluginLoader pushTimepointIn3DWindow: " << t <<"/"<< timepoint;
	}
	return t;
}

bool Visiocyte_PluginLoader::screenShot3DWindow(visiocytehandle image_window, QString filename)
{
	bool r =false;
	if_XFormWidget(w, image_window)
	{
		r = w->screenShot3DWindow(filename);
		qDebug() << "Visiocyte_PluginLoader screenShotIn3DWindow: " << r <<"/"<< filename;
	}

	return r;
}

//added Dec. 02,2013 by Zhi Zhou

void Visiocyte_PluginLoader::screenShot_Any3DViewer(VisiocyteR_MainWindow *w,QString filename)
{
    if (w)
    {
        VisiocyteR_GLWidget * v = w->getGLWidget();
        if (v)
            v->screenShot(filename);
    }
}

bool Visiocyte_PluginLoader::screenShotROI3DWindow(visiocytehandle image_window, QString filename)
{
	bool r =false;
	if_XFormWidget(w, image_window)
	{
		r = w->screenShotROI3DWindow(filename);
		qDebug() << "Visiocyte_PluginLoader screenShotROI3DWindow: " << r <<"/"<< filename;
	}

	return r;
}

//added Oct. 08, 2014 by Hanbo Chen
void Visiocyte_PluginLoader::update_NeuronBoundingBox(VisiocyteR_MainWindow *w)
{
    if(w && w->getGLWidget())
    {
        VisiocyteR_GLWidget * g = w->getGLWidget();
        Renderer_gl1 * gp = (Renderer_gl1 *) g->getRenderer();
        gp->updateNeuronBoundingBox();
        gp->updateBoundingBox();
    }
}

LandmarkList * Visiocyte_PluginLoader::getHandleLandmarkList_Any3DViewer(VisiocyteR_MainWindow *w) //Oct. 16, 2014 by Hanbo Chen
{
    if (!w) return 0;
    VisiocyteR_GLWidget * vi = w->getGLWidget();
    if (!vi) return 0;
    else
    {
        Renderer_gl1 * gp = (Renderer_gl1 *)(vi->getRenderer());
        if (!gp)
            return 0;
        else
            return (gp->getHandleLandmark());
    }
}

bool Visiocyte_PluginLoader::setHandleLandmarkList_Any3DViewer(VisiocyteR_MainWindow *w, LandmarkList & landmark_list)//Aug. 2, 2017 by Zhi Zhou
{
    if (!w) return false;
    VisiocyteR_GLWidget * vi = w->getGLWidget();
    if (!vi) return false;
    else
    {
        Renderer_gl1 * gp = (Renderer_gl1 *)(vi->getRenderer());
        if (!gp)
            return false;
        else
        {
            gp->setHandleLandmark(landmark_list);
            gp->updateLandmark();
            return true;
        }
    }
}

//VISIOCYTEPluginCallback2

View3DControl * Visiocyte_PluginLoader::getView3DControl(visiocytehandle image_window)
{
	View3DControl * vi = 0;
	if_XFormWidget(w, image_window)
	{
		//vi = w->getView3DControl();
        vi = dynamic_cast<View3DControl *>(w->getView3D());
		qDebug() << "Visiocyte_PluginLoader getView3DControl = "<< vi <<" : " << w <<"/"<< image_window;
	}
	return vi;
}

//added Dec. 02,2013 by Zhi Zhou

View3DControl * Visiocyte_PluginLoader::getView3DControl_Any3DViewer(VisiocyteR_MainWindow *w)
{
    View3DControl * vi = 0;
    if (w)
    {
        return (vi = dynamic_cast<View3DControl *>(w->getGLWidget()));
    }
}

View3DControl * Visiocyte_PluginLoader::getLocalView3DControl(visiocytehandle image_window)
{
	View3DControl * vi = 0;
	if_XFormWidget(w, image_window)
	{
		//vi = w->getLocalView3DControl();
		vi = dynamic_cast<View3DControl *>(w->getLocalView3D());
		qDebug() << "Visiocyte_PluginLoader getLocalView3DControl = "<< vi <<" : " << w <<"/"<< image_window;
	}
	return vi;
}

TriviewControl * Visiocyte_PluginLoader::getTriviewControl(visiocytehandle image_window)
{
	TriviewControl * tvi = 0;
	if_XFormWidget(w, image_window)
	{
		tvi = dynamic_cast<TriviewControl *>(w); // w->getTriview()
		qDebug() << "Visiocyte_PluginLoader getTriviewControl = "<< tvi <<" : " << w <<"/"<< image_window;
	}
	return tvi;
}

//added PHC 20120406. add a main window handle, to allow access everything in Visiocyte

MainWindow * Visiocyte_PluginLoader::getVisiocyteMainWindow()
{
    return visiocyte_mainwindow;
}

QList <VisiocyteR_MainWindow *> Visiocyte_PluginLoader::getListAll3DViewers()
{
    QList <VisiocyteR_MainWindow *> mylist;
    if (visiocyte_mainwindow)
        return visiocyte_mainwindow->list_3Dview_win;
    else
        return mylist;
}

VisiocyteR_MainWindow * Visiocyte_PluginLoader::find3DViewerByName(QString fileName) 
{
    if (visiocyte_mainwindow)
        return visiocyte_mainwindow->find3DViewer(fileName);
    else
        return 0;
}

void Visiocyte_PluginLoader::update_3DViewer(VisiocyteR_MainWindow *w) //Dec, 9, 2013. PHC
{
    if (w)
    {
        VisiocyteR_GLWidget * v = w->getGLWidget();
        if (v)
            v->update();
    }

    pumpEvents(); //131209 PHC

}

//the following 12 functions are added PHC 20120406 to allow uses to access the surface data objects in a 3D viewer

QList <NeuronTree> * getHandleNeuronTrees_3DGLWidget(VisiocyteR_GLWidget *vi) //a utility function
{
    if (!vi) return 0;
    else
    {
        Renderer_gl1 * gp = (Renderer_gl1 *)(vi->getRenderer());
        if (!gp)
            return 0;
        else
            return gp->getHandleNeuronTrees();
    }    
}

QList <NeuronTree> * Visiocyte_PluginLoader::getHandleNeuronTrees_3DGlobalViewer(visiocytehandle image_window)
{
	VisiocyteR_GLWidget * vi = (VisiocyteR_GLWidget *)(getView3DControl(image_window));
    return getHandleNeuronTrees_3DGLWidget(vi);
}

QList <NeuronTree> * Visiocyte_PluginLoader::getHandleNeuronTrees_Any3DViewer(VisiocyteR_MainWindow *w)
{
    if (!w) return 0;
	VisiocyteR_GLWidget * vi = w->getGLWidget();
    return getHandleNeuronTrees_3DGLWidget(vi);
}

//=======

QList <CellAPO>    * getHandleAPOCellList_3DGLWidget(VisiocyteR_GLWidget *vi) //a utility function
{
    if (!vi) return 0;
    else
    {
        Renderer_gl1 * gp = (Renderer_gl1 *)(vi->getRenderer());
        if (!gp)
            return 0;
        else
            return gp->getHandleAPOCellList();
    }    
}

QList <CellAPO>    * Visiocyte_PluginLoader::getHandleAPOCellList_3DGlobalViewer(visiocytehandle image_window)
{
	VisiocyteR_GLWidget * vi = (VisiocyteR_GLWidget *)(getView3DControl(image_window));
    return getHandleAPOCellList_3DGLWidget(vi);
}

QList <CellAPO>    * Visiocyte_PluginLoader::getHandleAPOCellList_Any3DViewer(VisiocyteR_MainWindow *w) 
{
    if (!w) return 0;
	VisiocyteR_GLWidget * vi = w->getGLWidget();
    return getHandleAPOCellList_3DGLWidget(vi);
}

//====

QList <LabelSurf> getListLabelSurf_3DGLWidget(VisiocyteR_GLWidget *vi) //a utility function
{
    QList <LabelSurf> mylabelsurf;
    if (!vi) return mylabelsurf;
    else
    {
        Renderer_gl1 * gp = (Renderer_gl1 *)(vi->getRenderer());
        if (!gp)
            return mylabelsurf;
        else
            return gp->getListLabelSurf();
    }    
}

QList <LabelSurf> Visiocyte_PluginLoader::getListLabelSurf_3DGlobalViewer(visiocytehandle image_window)
{
    QList <LabelSurf> mylabelsurf;
    
	VisiocyteR_GLWidget * vi = (VisiocyteR_GLWidget *)(getView3DControl(image_window));
    return getListLabelSurf_3DGLWidget(vi);
}

QList <LabelSurf> Visiocyte_PluginLoader::getListLabelSurf_Any3DViewer(VisiocyteR_MainWindow *w)
{
    QList <LabelSurf> mylabelsurf;
    if (!w) return mylabelsurf;
	VisiocyteR_GLWidget * vi = w->getGLWidget();
    return getListLabelSurf_3DGLWidget(vi);
}

DataLists_in_3dviewer Visiocyte_PluginLoader::fetch_3dviewer_datafilelist(QString name3dviewer)  //20140122 a conveniece function to access the record of data in a 3D viewer
{
    DataLists_in_3dviewer s;

    VisiocyteR_MainWindow * w = find3DViewerByName(name3dviewer);
    if (!w)
    {
        visiocyte_msg(QString("Fail to find valid VisiocyteR_MainWindow for the name [%1].").arg(name3dviewer));
        return s;
    }
    if (!(w->_idep))
    {
        visiocyte_msg(QString("The found VisiocyteR_MainWindow for the name [%1] has a NULL pointer to the 3D data.").arg(name3dviewer));
        return s;
    }

    s.swc_file_list = w->_idep->swc_file_list;
    s.pointcloud_file_list = w->_idep->pointcloud_file_list;
    s.surface_file = w->_idep->surface_file;
    s.labelfield_file = w->_idep->labelfield_file;

    if (w->_idep->image4d)
        s.imgfile = w->_idep->image4d->getFileName();

    return s;
}

//==========

bool setListLabelSurf_3DGLWidget(VisiocyteR_GLWidget *vi, QList <LabelSurf> listLabelSurfinput) //a utility function
{
    if (!vi) return false;
    else
    {
        Renderer_gl1 * gp = (Renderer_gl1 *)(vi->getRenderer());
        if (!gp)
            return false;
        else
        {
            gp->setListLabelSurf(listLabelSurfinput);
            return true;
        }
    }            
}

bool Visiocyte_PluginLoader::setListLabelSurf_3DGlobalViewer(visiocytehandle image_window, QList <LabelSurf> listLabelSurfinput)
{
	VisiocyteR_GLWidget * vi = (VisiocyteR_GLWidget *)(getView3DControl(image_window));
    return setListLabelSurf_3DGLWidget(vi, listLabelSurfinput);
}

bool Visiocyte_PluginLoader::setListLabelSurf_Any3DViewer(VisiocyteR_MainWindow *w, QList <LabelSurf> listLabelSurfinput)
{
    if (!w) return false;
	VisiocyteR_GLWidget * vi = w->getGLWidget();
    return setListLabelSurf_3DGLWidget(vi, listLabelSurfinput);
}

//added PHC 20130904 allow a plugin program to refresh and rescan all plugins //not working by PHC 20130904
//void Visiocyte_PluginLoader::refreshMainMenuPluginList()
//{
//    rescanPlugins();
//}

//added TeraFly interface, functions are provided by Alessadnro Bria, the wrapper is provided by Zhi Zhou Aug. 23, 2017
NeuronTree Visiocyte_PluginLoader::getSWCTeraFly()
{
    return terafly::PluginInterface::getSWC();
}

bool Visiocyte_PluginLoader::setSWCTeraFly(NeuronTree & nt)
{
    return terafly::PluginInterface::setSWC(nt);
}

LandmarkList Visiocyte_PluginLoader::getLandmarkTeraFly()
{
    return terafly::PluginInterface::getLandmark();
}

bool Visiocyte_PluginLoader::setLandmarkTeraFly(LandmarkList & landmark_list)
{
    return terafly::PluginInterface::setLandmark(landmark_list);
}

QString Visiocyte_PluginLoader::getPathTeraFly()
{
    return QString(terafly::PluginInterface::getPath().c_str());
}

const Image4DSimple * Visiocyte_PluginLoader::getImageTeraFly()
{
    return terafly::PluginInterface::getImage();
}

QString Visiocyte_PluginLoader::versionTeraFly()
{
    return QString(terafly::PluginInterface::version().c_str());
}

bool Visiocyte_PluginLoader::getDimTeraFly(const std::string & path, VISIOCYTELONG * & sz)
{
    sz = new VISIOCYTELONG [5];
    if (!sz)
    {
        return false;
    }
    sz[0] = terafly::PluginInterface::getXDim(path);
    sz[1] = terafly::PluginInterface::getYDim(path);
    sz[2] = terafly::PluginInterface::getZDim(path);
    sz[3] = terafly::PluginInterface::getCDim(path);
    sz[4] = terafly::PluginInterface::getTDim(path);
    return true;
}

unsigned char* Visiocyte_PluginLoader::getSubVolumeTeraFly(const std::string & path, size_t x0, size_t x1, size_t y0, size_t y1, size_t z0, size_t z1)
{
    return terafly::PluginInterface::getSubVolume(path,x0,x1,y0,y1,z0,z1);
}


void Visiocyte_PluginLoader::releaseOpenedVolumesTeraFly()
{
    terafly::PluginInterface::releaseOpenedVolumes();
}

bool Visiocyte_PluginLoader::setImageTeraFly(size_t x, size_t y, size_t z)
{
    return terafly::PluginInterface::setImage(x,y,z);
}

void Visiocyte_PluginLoader::redrawEditInfo(int editInputNum)
{
	terafly::PluginInterface::drawEditInfo(editInputNum);
}

#ifdef __ALLOW_VR_FUNCS__
void Visiocyte_PluginLoader::openVRWindow(VisiocyteR_MainWindow *w, bool bOnlineMode)
{
    qDebug("Visiocyte_PluginLoader::openVRWindow ");
    if (w)
    {
        qDebug("Visiocyte_PluginLoader::openVRWindow ----if w");
        VisiocyteR_GLWidget * v = w->getGLWidget();
        if (v)
        {
            qDebug("Visiocyte_PluginLoader::openVRWindow ----if v");
            v->doimageVRView(bOnlineMode);
        }
    }
}

void Visiocyte_PluginLoader::openVRWindowV2(visiocytehandle image_window, bool bOnlineMode)
{
    qDebug("Visiocyte_PluginLoader::openVRWindow version 2");
    VisiocyteR_GLWidget * vi = (VisiocyteR_GLWidget *)(getView3DControl(image_window));
    if(vi) vi->doimageVRView(bOnlineMode);
}
#endif
