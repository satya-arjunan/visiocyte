// generate an ano file for all swc files in the given directory
// by Lei Qu
// 2009-12-30
// rewritten by Hanchuan Peng, 2010-04-20. add other VISIOCYTE file types
// add dofunc() interface by Jianlong Zhou, 2012-04-18

#include <QtGui>
#include <stdlib.h>
#include <iostream>
#include "visiocyte_message.h"

#include "linker_file_generator.h"

using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(linker_file_generator, LinkerPlugin);

void GeneratorAno4FilesInDir(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);
bool GeneratorAno4FilesInDir(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void createANO(QString qs_dir_swc, QString ano_file, int method_code);

QStringList LinkerPlugin::menulist() const
{
    return QStringList()
    << tr("for all SWC/ESWC (e.g. neuron) files under a directory")
	<< tr("for all APO (point cloud) files under a directory")
	<< tr("for all TIF images under a directory")
	<< tr("for all LSM images under a directory")
	<< tr("for all VISIOCYTE Raw images under a directory")
	<< tr("for all images (*.tif, *.tiff, *.raw) under a directory")
	<< tr("for all VISIOCYTE-recognizable files under a directory");
}


void LinkerPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("for all SWC/ESWC (e.g. neuron) files under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 1);
	else if (menu_name == tr("for all APO (point cloud) files under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 2);
	else if (menu_name == tr("for all TIF images under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 3);
	else if (menu_name == tr("for all LSM images under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 4);
	else if (menu_name == tr("for all VISIOCYTE Raw images under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 5);
	else if (menu_name == tr("for all images (*.tif, *.tiff, *.lsm, *.raw) under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 6);
	else if (menu_name == tr("for all VISIOCYTE-recognizable files under a directory"))
		GeneratorAno4FilesInDir(callback, parent, 7);
	else
		visiocyte_msg("Un-recognizable menu detected");
}



QStringList LinkerPlugin::funclist() const
{
	return QStringList()
		<<tr("linker")
		<<tr("help");
}


bool LinkerPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     if (func_name == tr("linker"))
	{
		return GeneratorAno4FilesInDir(input, output);
	}
	else if(func_name == tr("help"))
	{
		cout<<"Usage : visiocyte -x linker_file -f linker -i <in_folder> -o <ano_file> -p <method_code> "<<endl;
		cout<<endl;
		cout<<"in_folder    choose a directory that contain specified files"<<endl;
          cout<<"ano_file     specify the ano file name to be created" <<endl;
		cout<<"method_code  ano options, default 1"<<endl;
          cout<<"             1 for for all SWC/ESWC (e.g. neuron) files under a directory"<< endl;
          cout<<"             2 for for all APO (point cloud) files under a directory"<< endl;
          cout<<"             3 for all TIF images under a directory"<< endl;
          cout<<"             4 for all LSM images under a directory"<< endl;
          cout<<"             5 for all VISIOCYTE Raw images under a directory"<< endl;
          cout<<"             6 for all images (*.tif, *.tiff, *.raw) under a directory"<< endl;
          cout<<"             7 for all VISIOCYTE-recognizable files under a directory"<< endl;
		cout<<endl;
		cout<<"e.g. visiocyte -x linker_file -f linker -i input_folder -o mylinker.ano -p 1"<<endl;
		cout<<endl;
		return true;
	}
}

bool GeneratorAno4FilesInDir(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	cout<<"Welcome to linker file generator"<<endl;
	if (input.size() < 1) return false;

     int method_code = 1;
     if (input.size()>=2)
     {
          vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
          if(paras.size() >= 1) method_code = atoi(paras.at(0));
	}

     char * ano_file = 0;
	char * input_path = ((vector<char*> *)(input.at(0).p))->at(0);
     if (output.size() >= 1)
     {
          ano_file = ((vector<char*> *)(output.at(0).p))->at(0);
          cout<<"ano_file = "<<ano_file<<endl;
     }
	cout<<"input_path = "<<input_path<<endl;

     //choose a directory that contain specified files
	// QString qs_dir_swc;
     // qs_dir_swc = QString(input_path);

     createANO(QString(input_path), QString(ano_file), method_code);
     return true;
}



void GeneratorAno4FilesInDir(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code)
{
	//choose a directory that contain swc files
	QString qs_dir_swc;
	qs_dir_swc=QFileDialog::getExistingDirectory(parent,QString(QObject::tr("Choose the directory that contains the specified files")));

     createANO(qs_dir_swc, QString(""), method_code);
}


void createANO(QString qs_dir_swc, QString ano_file, int method_code)
{
     //find all swc filename in given directory
	QDir dir(qs_dir_swc);
	QStringList qsl_filelist,qsl_filters;

	switch (method_code)
	{
		case 1:
			qsl_filters+="*.swc";
            qsl_filters+="*.eswc";
			break;
		case 2:
			qsl_filters+="*.apo";
			break;
		case 3:
			qsl_filters+="*.tif";
			qsl_filters+="*.tiff";
			break;
		case 4:
			qsl_filters+="*.lsm";
			break;
		case 5:
			qsl_filters+="*.raw";
            qsl_filters+="*.visiocyteraw";
			break;
		case 6:
			qsl_filters+="*.tif";
			qsl_filters+="*.tiff";
			qsl_filters+="*.lsm";
			qsl_filters+="*.raw";
            qsl_filters+="*.visiocyteraw";
			break;
		case 7:
			qsl_filters+="*.tif";
			qsl_filters+="*.tiff";
			qsl_filters+="*.lsm";
			qsl_filters+="*.raw";
			qsl_filters+="*.swc";
            qsl_filters+="*.eswc";
			qsl_filters+="*.apo";
			qsl_filters+="*.visiocytes";
			qsl_filters+="*.obj";
            qsl_filters+="*.visiocyteraw";
			break;
		default:
			visiocyte_msg("You should never see this, - check with the developer of this plugin.");
			break;
	}

	foreach(QString file, dir.entryList(qsl_filters,QDir::Files))
	{
		qsl_filelist+=file;
	}

	if(qsl_filelist.size()==0)
	{
		visiocyte_msg("Cannot find the respective files in the given directory!\nTry another diretory");
		return;
	}

	//generator ano file
	QString qs_filename_out;
     if(ano_file.isEmpty())
          qs_filename_out = qs_dir_swc+"/mylinker.ano"; // for domenu()
     else
          qs_filename_out =  ano_file;

	QFile qf_anofile(qs_filename_out);
	if(!qf_anofile.open(QIODevice::WriteOnly))
	{
		visiocyte_msg("Cannot open file for writing!");
		return;
	}

	QTextStream out(&qf_anofile);
	for(VISIOCYTELONG i=0;i<qsl_filelist.size();i++)
	{
		QString temp;

		//see ../basic_c_fun/basic_surf_objs.cpp for keywords

		QFileInfo curfile_info(qsl_filelist[i]);
		if (curfile_info.suffix().toUpper()=="SWC")
			temp = qsl_filelist[i].prepend("SWCFILE=");
        else if (curfile_info.suffix().toUpper()=="ESWC")
            temp = qsl_filelist[i].prepend("SWCFILE=");
		else if (curfile_info.suffix().toUpper()=="APO")
			temp = qsl_filelist[i].prepend("APOFILE=");
		else if (curfile_info.suffix().toUpper()=="VISIOCYTES")
			temp = qsl_filelist[i].prepend("SURFILE=");
		else if (curfile_info.suffix().toUpper()=="OBJ")
			temp = qsl_filelist[i].prepend("SURFILE=");
		else if (curfile_info.suffix().toUpper()=="RAW")
			temp = qsl_filelist[i].prepend("RAWIMG=");
		else if (curfile_info.suffix().toUpper()=="TIF")
			temp = qsl_filelist[i].prepend("RAWIMG=");
		else if (curfile_info.suffix().toUpper()=="TIFF")
			temp = qsl_filelist[i].prepend("RAWIMG=");
		else if (curfile_info.suffix().toUpper()=="LSM")
			temp = qsl_filelist[i].prepend("RAWIMG=");
        else if (curfile_info.suffix().toUpper()=="VISIOCYTERAW")
            temp = qsl_filelist[i].prepend("RAWIMG=");
		else
			visiocyte_msg("You should never see this, - check with the developer of this plugin.");

		out << temp << endl;
		visiocyte_msg(qPrintable(temp), 0);
	}

	//show message box
     if(ano_file.isEmpty())
          visiocyte_msg(QString("Save the linker file to: \n\n%1\n\nComplete!").arg(qs_filename_out));
     else
          visiocyte_msg(QString("Save the linker file to: \n\n%1\n\nComplete!").arg(qs_filename_out), 0);
}
