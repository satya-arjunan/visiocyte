//q_paradialog_combine.h
//by Lei Qu
//2010-10-10

#ifndef __Q_PARADIALOG_COMBINE_H__
#define __Q_PARADIALOG_COMBINE_H__

#include <QDialog>
#include <visiocyte_interface.h>

#include "ui_paradialog_combine.h"


class ParaDialogCombine : public QDialog, public Ui::ParaDialogCombine
{
	Q_OBJECT

public:
	ParaDialogCombine(VISIOCYTEPluginCallback &callback,QWidget *parent);

private slots:
	void _slots_openAtlasDlg();
	void _slots_saveAtlasDlg();

public:
	void IniDialog(VISIOCYTEPluginCallback &callback);

	visiocytehandleList h_wndlist;

};


#endif
