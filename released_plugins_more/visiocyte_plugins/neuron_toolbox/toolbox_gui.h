#ifndef __TOOLBOX_GUI_H__
#define __TOOLBOX_GUI_H__
#include <QtGui>
#include "visiocyte_interface.h"
class SelectPluginDlg : public QDialog
{
	Q_OBJECT

public:
	SelectPluginDlg(QWidget * parent, const VISIOCYTEPluginCallback2 & _callback);
	SelectPluginDlg(QWidget * parent, const VISIOCYTEPluginCallback2 & _callback, const VISIOCYTEPluginArgList & _input);
	
	VISIOCYTEPluginArgList * input;
	QWidget * parent;
	VISIOCYTEPluginCallback2 * callback;
	QTreeWidget * pluginTreeWidget;
	QString root_path;
	QHash<QTreeWidgetItem*, QString> name_table;


public slots:
	bool runMenu();
	bool runFunc();
	bool freeMem();
};

#endif
