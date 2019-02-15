/* convert_file_format_plugin.h
 * Convert a few file formats, e.g. visiocyteraw to tif and vice versa.
 * 2012-2013 : by Hanchuan Peng
 */
 
#ifndef __CONVERT_FILE_FORMAT_PLUGIN_H__
#define __CONVERT_FILE_FORMAT_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class ConvertFileFormatPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

