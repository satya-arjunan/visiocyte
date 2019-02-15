//q_paradialog_stranno.h
//by Lei Qu
//2010-11-04

#ifndef __Q_PARADIALOG_STRANNO_H__
#define __Q_PARADIALOG_STRANNO_H__

#include <QDialog>
#include <visiocyte_interface.h>

#include "ui_paradialog_stranno.h"


class CParaDialog_stranno : public QDialog, public Ui::ParaDialog_stranno
{
	Q_OBJECT

public:
	CParaDialog_stranno(VISIOCYTEPluginCallback &callback,QWidget *parent);
	~CParaDialog_stranno();

private slots:
	void _slots_openAtlasDlg();
	void _slots_openCelloiDlg();
	void _slots_openCelloiDlg_2();
	void _slots_saveAtlasDlg();
	void _slots_saveSeglabelDlg();

public:
	void IniDialog(VISIOCYTEPluginCallback &callback);

	visiocytehandleList h_wndlist;

};


#endif
