/* iBioformatIO.cpp
 * 2011-12-02: create this program by Yang Yu
 */

// add dofunc() by Jianlong Zhou, 2012-04-08

//

#ifndef __IBIOFORMATIO_CPP__
#define __IBIOFORMATIO_CPP__

#include <QtGui>
#include <QFileInfo>

//#include <cmath>
#include <stdlib.h>
#include <ctime>

#include "iBioformatIO.h"

//#include "basic_surf_objs.h"
//#include "stackutil.h"
//#include "volimg_proc.h"
//#include "img_definition.h"
//#include "basic_landmark.h"

//#include "mg_utilities.h"
//#include "mg_image_lib.h"

//#include "basic_landmark.h"
//#include "basic_4dimage.h"

#include <iostream>
using namespace std;

#include <string.h>


//plugin interface
const QString title = "Image IO Using Bioformat";
static QString pluginRootPath;

Q_EXPORT_PLUGIN2(imageIO_Bioformat, IBioformatIOPlugin);

QStringList IBioformatIOPlugin::menulist() const
{
    return QStringList() << tr("load an image using Bioformats Java library")
                         << tr("click me if you are unhappy with the loading result...")
                         << tr("About");
}

void IBioformatIOPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("load an image using Bioformats Java library"))
    {

        // input
        QString m_FileName = QFileDialog::getOpenFileName(parent, QObject::tr("Open An Image"),
                                                          QDir::currentPath(),
                                                          QObject::tr("Image File (*.*)"));

        if(m_FileName.isEmpty())
        {
             printf("\nError: Your image does not exist!\n");
             return;
        }

        // temp
        QString baseName = QFileInfo(m_FileName).baseName();
        QString tmpfile = QDir::tempPath().append("/").append(baseName).append(".tif");

        //
        QFile tmpqfile(tmpfile);
        if (tmpqfile.exists()) system(qPrintable(QString("rm -f \"%1\"").arg(tmpfile)));

        //look for loci_tools.jar
        QString lociDir = ("loci_tools.jar");
        if (!QFile(lociDir).exists())
        {
             printf("loci_tools.jar is not in current directory, search visiocyte app path.\n");
             lociDir = getAppPath().append("/loci_tools.jar");
             printf(qPrintable(lociDir));
             printf("\n");
             if (!QFile(lociDir).exists())
             {
                  visiocyte_msg("Cannot find loci_tools.jar, please download it and make sure it is put under the Visiocyte executable folder, parallel to the Visiocyte executable and the plugins folder.");
                  return;
             }
        }

#if defined(Q_OS_WIN32)
        QSettings settings("HHMI", "Visiocyte");
        QString fileOpenName = settings.value("JavaPath").toString();
        if(fileOpenName.isEmpty())
        {
            fileOpenName = QFileDialog::getOpenFileName(0, QObject::tr("Open Java Executable File"),
                    "",
                    QObject::tr("Supported file (*.exe)"
                        ));
            if(fileOpenName.isEmpty())
                return;
            settings.setValue("JavaPath", fileOpenName);
        }

        QString cmd_loci = QString("\"\"%1\" -cp \"%2\" loci.formats.tools.ImageConverter \"%3\" \"%4\"\"").arg(fileOpenName.toStdString().c_str()).arg(lociDir.toStdString().c_str()).arg(m_FileName.toStdString().c_str()).arg(tmpfile.toStdString().c_str());
#else
        QString cmd_loci = QString("java -cp \"%1\" loci.formats.tools.ImageConverter \"%2\" \"%3\"").arg(lociDir.toStdString().c_str()).arg(m_FileName.toStdString().c_str()).arg(tmpfile.toStdString().c_str());
#endif
        visiocyte_msg(cmd_loci, 0);

        system(qPrintable(cmd_loci));

        if (!tmpqfile.exists())
        {
            visiocyte_msg("The temprary file does not exist. The conversion of format using Bioformats has failed. Please sue another way to convert and load using Visiocyte.\n");
            return;
        }


        // load
        VISIOCYTELONG sz_relative[4];
        int datatype_relative = 0;
        unsigned char* relative1d = 0;

        if (simple_loadimage_wrapper(callback, const_cast<char *>(tmpfile.toStdString().c_str()), relative1d, sz_relative, datatype_relative)!=true)
        {
             fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",tmpfile.toStdString().c_str());
             return;
        }

        // visualize
        Image4DSimple p4DImage;

        if(datatype_relative == VISIOCYTE_UINT8)
        {
             p4DImage.setData((unsigned char*)relative1d, sz_relative[0], sz_relative[1], sz_relative[2], sz_relative[3], VISIOCYTE_UINT8);
        }
        else if(datatype_relative == VISIOCYTE_UINT16)
        {
             p4DImage.setData((unsigned char*)relative1d, sz_relative[0], sz_relative[1], sz_relative[2], sz_relative[3], VISIOCYTE_UINT16);
        }
        else if(datatype_relative == VISIOCYTE_FLOAT32)
        {
             p4DImage.setData((unsigned char*)relative1d, sz_relative[0], sz_relative[1], sz_relative[2], sz_relative[3], VISIOCYTE_FLOAT32);
        }
        else
        {
             printf("\nError: The program only supports UINT8, UINT16, and FLOAT32 datatype.\n");
             return;
        }

        visiocytehandle newwin = callback.newImageWindow();
        callback.setImage(newwin, &p4DImage);
        callback.setImageName(newwin, tmpfile.toStdString().c_str());
        callback.updateImageWindow(newwin);

    }
    else if (menu_name == tr("click me if you are unhappy with the loading result..."))
    {
       visiocyte_msg("This program is designed to use a system call to invoke the LOCI Bioformats Image IO Java library loci_tools.jar"
               " to load an image. It first calls bioformats library to generate a temporary 3D TIF file on your harddrive"
               " and then uses Visiocyte to load that temporary file. Therefore, if you see some wrong loading result using this plugin, it is"
               " likely that you will get the same thing if you run the bioformats library directly. Of course, you may find a newer"
               " version of the loci_tools.jar at the LOCI website http://loci.wisc.edu/bio-formats/downloads; we encourage you"
               " to copy the latest version to the Visiocyte executable folder and try if it would fix your problem.", 1);
       return;
    }
    else if (menu_name == tr("About"))
    {
        QMessageBox::information(parent, "Version info", QString("Simple image reading using Bioformats library %1 (2011-2012) developed by Yang Yu, Yinan Wan, and Hanchuan Peng. (Janelia Research Farm Campus, HHMI)").arg(getPluginVersion()).append("\n"));
        return;
    }
}

// plugin func
QStringList IBioformatIOPlugin::funclist() const
{
    return QStringList() << tr("imgiobiof")
                         << tr("help");
}

bool IBioformatIOPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
     if (func_name == tr("imgiobiof"))
	{
          cout<<"Welcome to imageIOBioformat loader"<<endl;
          if(input.size() != 1 || output.size() != 1) return false;

          char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
          char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
          cout<<"inimg_file = "<<inimg_file<<endl;
          cout<<"outimg_file = "<<outimg_file<<endl;

          QString m_FileName(inimg_file); // use inimge_file name

          if(m_FileName.isEmpty())
          {
               printf("\nError: Your image does not exist!\n");
               return false;
          }

          // temp
          QString baseName = QFileInfo(m_FileName).baseName();
          //QString tmpfile = QDir::tempPath().append("/").append(baseName).append(".tif");
          QString tmpfile(outimg_file); //use outimg_file name

          //
          QFile tmpqfile(tmpfile);
          if (tmpqfile.exists()) system(qPrintable(QString("rm -f \"%1\"").arg(tmpfile)));

          //look for loci_tools.jar
          QString lociDir = ("loci_tools.jar");
          if (!QFile(lociDir).exists())
          {
               printf("loci_tools.jar is not in current directory, search visiocyte app path.\n");
               lociDir = getAppPath().append("/loci_tools.jar");
               printf(qPrintable(lociDir));
               printf("\n");
               if (!QFile(lociDir).exists())
               {
                    visiocyte_msg("Cannot find loci_tools.jar, please download it and make sure it is put under the Visiocyte executable folder, parallel to the Visiocyte executable and the plugins folder.", 0);
                    return false;
               }
          }

          QString cmd_loci = QString("java -cp %1 loci.formats.tools.ImageConverter \"%2\" \"%3\"").arg(lociDir.toStdString().c_str()).arg(m_FileName.toStdString().c_str()).arg(tmpfile.toStdString().c_str());
          visiocyte_msg(cmd_loci, 0);

          system(qPrintable(cmd_loci));

          if (!tmpqfile.exists())
          {
              visiocyte_msg("The temprary file does not exist. The conversion of format using Bioformats has failed. Please use another way to convert and load using Visiocyte.\n");
              return false;
          }

          return true;
	}
	else if(func_name == tr("help"))
	{
		cout<<"Usage : visiocyte -x imageIO_Bioformat -f imgiobiof -i <inimg_file> -o <outimg_file>"<<endl;
		cout<<"e.g. visiocyte -x imageIO_Bioformat -f imgiobiof -i input.raw -o output.raw"<<endl;
		cout<<endl;
		return true;
	}

}

QString getAppPath()
{
	QString visiocyteAppPath("~/Work/visiocyte_external/visiocyte");
	QDir testPluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	if (testPluginsDir.dirName().toLower() == "debug" || testPluginsDir.dirName().toLower() == "release")
		testPluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	// In a Mac app bundle, plugins directory could be either
	//  a - below the actual executable i.e. visiocyte.app/Contents/MacOS/plugins/
	//  b - parallel to visiocyte.app i.e. foo/visiocyte.app and foo/plugins/
	if (testPluginsDir.dirName() == "MacOS") {
		QDir testUpperPluginsDir = testPluginsDir;
		testUpperPluginsDir.cdUp();
		testUpperPluginsDir.cdUp();
		testUpperPluginsDir.cdUp(); // like foo/plugins next to foo/visiocyte.app
		if (testUpperPluginsDir.cd("plugins")) testPluginsDir = testUpperPluginsDir;
		testPluginsDir.cdUp();
	}
#endif

	visiocyteAppPath = testPluginsDir.absolutePath();
	return visiocyteAppPath;
}

#endif


