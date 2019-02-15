/* NifTi_reader_plugin.h
 * Reader for NifTi format file
 * 2014-09-29 : by Xiang Li
 */
 
#ifndef __NIFTI_READER_PLUGIN_H__
#define __NIFTI_READER_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class NifTi_reader : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

