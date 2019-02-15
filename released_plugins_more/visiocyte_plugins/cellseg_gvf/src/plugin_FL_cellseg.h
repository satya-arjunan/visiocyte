/* plugin_FL_cellseg.h
 * cell seg  plugin based on Fuhui Long's code
 * by Hanchuan Peng, 2011-02-22
 */


#ifndef __PLUGIN_FL_CELLSEG_H__
#define __PLUGIN_FL_CELLSEG_H__

#include <QtGui>

#include "visiocyte_interface.h"

#include "label_object_dialog.h"

class FLCellSegPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);
	
public:
    float getPluginVersion() const {return 0.93f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const {return QStringList();}
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
	{return false;}
	
	
};


#endif

