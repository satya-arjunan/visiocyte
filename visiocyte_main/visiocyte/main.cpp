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




/****************************************************************************
**
VISIOCYTE main program

By Hanchuan Peng

July 21, 2006
060924: visiocyte v1.2

Last update: 2008-04-25: try to add command line based utilities
Last update: 2010-04-12: add a global try-catch to catch all exceptions
Last update: 2010-11-19: change some of the help info
Last update: 2011-04-19: fix some potential problem of null mainWin pointer
Last update: 2011-08-25: remove some uncalled old code, and adjust the inconsistent return values of the main function

****************************************************************************/

#define COMPILE_TO_COMMANDLINE 1

#include "../3drenderer/visiocyter_common.h"

#include "visiocyte_compile_constraints.h"

#include <QApplication>
#include <QFile>

#include <iostream>
#include <vector>

#include "mainwindow.h"
#include "visiocyte_application.h"

#include <string>
using namespace std;

#include "visiocyte_core.h"

#include "visiocyte_commandlineparser.h"
#include "visiocyte_version_info.h"
#include "../plugin_loader/visiocyte_plugin_loader.h"

void printHelp_visiocyte();
void printHelp_align();
void printHelp_straight();
void printHelp_trace();

VisiocyteApplication* VisiocyteApplication::theApp = 0;

void printHelp_visiocyte()
{
	cout<<endl<<"Visiocyte: a 3D image visualization and analysis platform developed by Hanchuan Peng and colleagues."<<endl;
	cout<<endl<<"Usage: visiocyte -h -M moduleCode [all other options specific to different modules]"<<endl;

	cout<<"    -h/H         help information."<<endl;

	cout<<"    -i <file>                    open single or multiple image (.tif/.tiff, .lsm, .mrc, .raw/.visiocyteraw) / object (.ano, .apo, .swc, .marker) files"<<endl;
    cout<<"    -o <file>                    indicates single or multiple outputs"<<endl;
	cout<<"    -x <plugin_dll_full_path or unique partial path>    a string indicates the full path or a unique partial path of a dll (for a plugin) to be launched."<<endl;
	cout<<"    -m <menu_name>               a string indicates which menu of a plugin will be called."<<endl;
    cout<<"    -f <function_name>           a string indicates which function of a plugin will be called."<<endl;
    cout<<"    -p <parameters>              a string indicates parameters that plugin function use"<<endl;
    cout<<"    -pf <configuration>          a string read from configuration file indicates parameters that plugin function use"<<endl;

	cout<<"    -v                           force to open a 3d viewer when loading an image, otherwise use the default visiocyte global setting (from \"Adjust Preference\")"<<endl;
    cout<<"    -na                          open NeuronAnnotator work-mode directly"<<endl;
    cout<<"    -cmd  [headless command-line arguments, intended for compute grid use. Try \'-cmd -h\' for more information on this option]"<<endl;

    //added by Hanchuan Peng, 20120217
    VisiocyteApplication* app = VisiocyteApplication::getInstance();
    if (!app) return;
    MainWindow* mainWin=app->getMainWindow();
    if (!mainWin) return;
    QStringList existingPluginsList = mainWin->pluginLoader->getPluginNameList();
    if (existingPluginsList.size()>0)
        cout << endl << "Found [" << existingPluginsList.size() << "] plugins"<<endl;
    for (int i=0;i<existingPluginsList.size();i++)
        cout << "#" << i+1 << "          " << existingPluginsList.at(i).toLocal8Bit().constData() << endl;

	return;
}

int main(int argc, char **argv)
{
 for (int myii=0; myii<argc;myii++)
 {
     visiocyte_msg(QString("[%1]").arg(argv[myii]));
 }


#ifdef COMPILE_TO_COMMANDLINE

	CLP parser;

	parser.parse(argc, argv, printHelp_visiocyte); // parse command lines to visiocyte_cl_interface Nov. 23, 2010 by YuY

	if(parser.i_visiocyte.clp_finished)
	{
		return false;
	}
	else
	{
		if(parser.i_visiocyte.openVISIOCYTE)
		{
			// ------ VISIOCYTE GUI handling module ---------------------
			Q_INIT_RESOURCE(visiocyte);

            VisiocyteApplication* app = VisiocyteApplication::getInstance(argc, argv);
            if(!parser.i_visiocyte.hideVISIOCYTE)
            {
                app->activateMainWindow();
            }

            MainWindow* mainWin=app->getMainWindow();

			if (!mainWin)
			{
				visiocyte_msg("Unable to open the Visiocyte main window. Quit.");
				return false;
			}

            app->installEventFilter(mainWin);

            if (mainWin)
            {
                mainWin->visiocyteclp.copy(parser.i_visiocyte);

                if(!parser.i_visiocyte.hideVISIOCYTE)
                {
                    mainWin->show();
                    if(parser.i_visiocyte.openNeuronAnnotator)
                    {
#ifdef _ALLOW_WORKMODE_MENU_
                        mainWin->func_procModeNeuronAnnotator();
#endif
                    }
                }
            }

			// plugin module
			if(parser.i_visiocyte.pluginname)
			{
				mainWin->setBooleanCLplugin(true);
				mainWin->setPluginName(parser.i_visiocyte.pluginname);
				mainWin->setPluginMethod(parser.i_visiocyte.pluginmethod);
				mainWin->setPluginFunc(parser.i_visiocyte.pluginfunc);
			}

			// multiple image/object handling module
			if(parser.i_visiocyte.fileList.size()==0 || parser.i_visiocyte.hideVISIOCYTE)
			{
				if(parser.i_visiocyte.pluginname)
				{
					mainWin->triggerRunPlugin();
				}
			}
			else
			{
				for(int i=0; i<parser.i_visiocyte.fileList.size(); i++)
				{
					char *filename = parser.i_visiocyte.fileList.at(i);

					visiocyte_msg("now try open files ...", 0);

					QString qFile(filename);

					if(!QFile(qFile).exists()) // supporting both local and web files. Nov. 18, 2010. YuY
					{
						// judge whether the file exists on the web
						// "://" like "http://" "https://" "ftp://"

						if(qFile.contains("://"))
						{
							QUrl url(filename);

							if(!url.isValid()) // valid or invalid url
							{
								visiocyte_msg(QString("The file path [%1] is not valid! Do nothing.").arg(filename), 0);
								return false;
							}
							else if(url.scheme().toUpper() == "HTTP" || url.scheme().toUpper() == "HTTPS" || url.scheme().toUpper() == "FTP")
							{
								// load image/object
								mainWin->loadVISIOCYTEUrl(QUrl(filename), true, parser.i_visiocyte.open3Dviewer);
							}
							//how about smb:// etc?? //by PHC, 20101123 question
						}
						else // impossible be a url
						{
							visiocyte_msg(QString("The file path [%1] seems invalid (not a local file or a URL)! Do nothing.").arg(filename), 0);
							return false;
						}
					}
					else
					{
						QString curSuffix = QFileInfo(qFile).suffix();
						// load image/object
						mainWin->loadVISIOCYTEFile(filename, true, parser.i_visiocyte.open3Dviewer);
					}
				}
			}


            // Check for software updates.
            // But not if VISIOCYTE has been invoked with a file to open immediately.
            // Like the logic Fiji uses: http://pacific.mpi-cbg.de/wiki/index.php/Update_Fiji
            // To help decide whether to check for VISIOCYTE software updates at start up,
            // determine whether any file has been opened.
            // This works for files open on the command line, but not for dropped files on Mac
            int nchild;
            mainWin->retrieveAllMdiChild(nchild);
            // TODO - don't check for updates if Mac OpenFileEvent is pending.
            // Don't check for updates if the current user does not have write permission.
            bool userCanUpdate = false;
            QFile visiocyteProgramFile(argv[0]);
            if (visiocyteProgramFile.exists() && (visiocyteProgramFile.permissions() & QFile::WriteUser))
            {
                // visiocyte_msg("User can write to visiocyte executable",0);
                userCanUpdate = true;
            }
            // Does the user have permission to write to any plugin folders?
            QList<QDir> pluginsDirList = Visiocyte_PluginLoader::getPluginsDirList();
            foreach (const QDir& pluginsDir, pluginsDirList)
            {
                QStringList nameFilter;
                nameFilter << ".";
                QFileInfoList dirInfoList = pluginsDir.entryInfoList(nameFilter);
                foreach (const QFileInfo& dirInfo, dirInfoList) {
                    if (dirInfo.permissions() & QFile::WriteUser) {
                        userCanUpdate = true;
                        // visiocyte_msg("User can write to plugin directory " + pluginsDir.absolutePath(), 0);
                    }
                }
            }
            //if ( (nchild == 0) && userCanUpdate )

#ifndef VISIOCYTE_SKIP_AUTO_VERSION_CHECK
            {
                // This is the automatic check for latest version
                visiocyte_msg("Starting Visiocyte version checker...", 0);
                visiocyte::VISIOCYTEVersionChecker *versionChecker = new visiocyte::VISIOCYTEVersionChecker(mainWin);
                //if (versionChecker->shouldCheckNow()) {
                    // visiocyte_msg("It is time to check for software updates...",0);
                    //versionChecker->checkForLatestVersion(false); // turn on 20130831. PHC  turn OFF 2017.03.28 BRL
                //}
            }
#endif

#define __AUTOLAUNCH_OPEN_NEURON_GAME___
#ifdef CGS_AUTOLAUNCH
            /// RZC 20170620: disable auto launch
            // mainWin->func_open_neuron_game();
#endif
			// launch visiocyte
			try
			{
                if(!parser.i_visiocyte.hideVISIOCYTE)
                    return app->exec();
                else
                    return false;
			}
			catch (...)
			{
				visiocyte_msg("Catch an exception at the main application level. Basically you should never see this. Please click Ok to quit and send the error log to the Visiocyte developers to figure out the problem.");
				return false;
			}
			// -------------------------------------------------------

		}
		else
		{
			return false;
		}

	}

#else  //why the following would have a pos() segment fault?? by PHC, 110825

	Q_INIT_RESOURCE(visiocyte);

	QApplication app(argc, argv);

	//090812 RZC: best solution for QMainWindow is using new, then deleteLater itself in its closeEvent.
	MainWindow* mainWin = new MainWindow;
	if (!mainWin)
	{
		visiocyte_msg("Unable to open the Visiocyte main window. Quit.");
		return false;
	}

    // On Mac, allow mainWin to get QFileOpen events, such as when a tif
    // file is dragged onto the application icon.
    // CMB Nov-12-2010

    app.installEventFilter(mainWin);

	mainWin->show();

	//*************************
	// DO NOT USE THE FOLLOWING AS I CHANGED TO MDI APPLICATIONS
	//*************************

	//XFormWidget xformWidget(0);
	//XFormWidget xformWidget;
	//XFormWidget xformWidget1(0); //useful to display multiple stacks

	/*
	//set style

	QStyle *arthurStyle = new ArthurStyle();
	xformWidget.setStyle(arthurStyle);

	QList<QWidget *> widgets = qFindChildren<QWidget *>(&xformWidget);
	foreach (QWidget *w, widgets)
		w->setStyle(arthurStyle);
	*/

	//display

	//xformWidget.show();
	//xformWidget1.show(); //useful to display multiple stacks

	//iDrawMainWindow my3ddraw;
	//my3ddraw.show();

	try
	{
		return app.exec();
	}
	catch (...)
	{
		visiocyte_msg("Catch an exception at the main application level. Basically you should never see this. Please click Ok to quit and send the error log to the Visiocyte developers to figure out the problem.");
		return false;
	}

#endif

}
