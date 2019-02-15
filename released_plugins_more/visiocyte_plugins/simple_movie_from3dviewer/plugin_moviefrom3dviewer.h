//plugin_moviefrom3dviewer.cpp
//by Lei Qu
//2010-11-08


#ifndef __PLUGIN_MOVIEWFROM3DVIEWER_H__
#define __PLUGIN_MOVIEWFROM3DVIEWER_H__

#include <QtGui>
#include "visiocyte_interface.h"

class MovieFrom3DviewerPlugin: public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const {return QStringList();}
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
        {return false;}
        float getPluginVersion() const {return 1.1f;}
};

class controlPanel: public QDialog
{
	Q_OBJECT

public:
	controlPanel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
	~controlPanel();

public:
	long m_lframeind;
	QLineEdit *m_pLineEdit_filepath;
	QLineEdit *m_pLineEdit_fps;
	VISIOCYTEPluginCallback2 &m_visiocyte;
	static controlPanel*m_pLookPanel;
	QTimer *m_pTimer;

private slots:
	void _slot_start();
	void _slot_stop();
	void _slots_openFileDlg_output();
	void _slot_timerupdate();

};

#endif

