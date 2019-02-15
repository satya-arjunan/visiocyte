/* plugin_watershed.h
 * watershed plugin based on Fuhui Long's watershed code
 * by Hanchuan Peng, 2011-02-21
 */


#ifndef __PLUGIN_WATERSHED_VS_H__
#define __PLUGIN_WATERSHED_VS_H__

#include <QtGui>

#include "visiocyte_interface.h"

#include "label_object_dialog.h"

class WatershedVSPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);
	
public:
	float getPluginVersion() const {return 0.9f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const {return QStringList();}
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
	{return false;}
	
	
};


#endif

