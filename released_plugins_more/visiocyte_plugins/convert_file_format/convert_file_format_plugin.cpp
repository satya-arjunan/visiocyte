/* convert_file_format_plugin.cpp
 * Convert a few file formats, e.g. visiocyteraw to tif and vice versa.
 * 2012-2013 : by Hanchuan Peng
 */

#define _ALLOW_WORKMODE_MENU_

#include "visiocyte_message.h"
#include <vector>
#include "convert_file_format_plugin.h"
using namespace std;
Q_EXPORT_PLUGIN2(convert_file_format, ConvertFileFormatPlugin);

 
QStringList ConvertFileFormatPlugin::menulist() const
{
	return QStringList() 
		<<tr("about");
}

QStringList ConvertFileFormatPlugin::funclist() const
{
	return QStringList()
		<<tr("convert_format")
		<<tr("help");
}

void ConvertFileFormatPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("about"))
	{
        visiocyte_msg(tr("This plugin converts between a few file formats, e.g. visiocyteraw to tif and vice versa. This plugin is designed for command line use ONLY right now. \n\n"
                   "If the file format is not supported, then the file reading may fail and thus the writing of NULL data may result in a crash. \n\n"
                   "Developed by Hanchuan Peng, 2012-2013"));
	}
}

bool ConvertFileFormatPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("convert_format"))
	{
        if (infiles.size()<1 || outfiles.size()<1 )
        {
            visiocyte_msg("No input image or output image has been specified.");
            return false;
        }

        qDebug() << "infile=[" << infiles.at(0) << "]";
        qDebug() << "outfile=[" << outfiles.at(0) << "]";

        Image4DSimple *inimg = callback.loadImage(infiles.at(0));
        if (!inimg || !inimg->valid())
            return false;

        callback.saveImage(inimg, outfiles.at(0));

        if (inimg) {delete inimg; inimg=0;}
    }
	else if (func_name == tr("help"))
	{
        visiocyte_msg("This plugin converts file formats from oen to another, based on the extension of the file names provided.", 0);
        visiocyte_msg("Usage: visiocyte -x dll_name -f convert_format -i input_image_file -o output_image_file", 0);
    }
	else return false;

	return true;
}

