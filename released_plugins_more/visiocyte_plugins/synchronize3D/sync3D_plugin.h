/* sync3D_plugin.h
 * 2013-07-09 : by Zhi Zhou
 */
 
//last edit: by PHC, remove the redundant data variables, 2013-07-12

#ifndef __SYNC3D_PLUGIN_H__
#define __SYNC3D_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class sync3D : public QObject, public VISIOCYTEPluginInterface2_1
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



class lookPanel: public QDialog
{	
	Q_OBJECT

public:
	lookPanel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
	~lookPanel();

public:
    QComboBox* combo_master;
    QComboBox* combo_slave;
    QLabel* label_master;
    QLabel* label_slave;
	QCheckBox* check_rotation;
	QCheckBox* check_shift;
	QCheckBox* check_zoom;
	QGridLayout *gridLayout;
	visiocytehandleList win_list;
	visiocytehandleList win_list_past;			
    VISIOCYTEPluginCallback2 & m_visiocyte;
	QTimer *m_pTimer;
	QPushButton* syncAuto;
    View3DControl *view_master;
    View3DControl *view_slave;
	int xRot_past, yRot_past,zRot_past;	
	int xShift_past,yShift_past,zShift_past;
	int zoom_past;
    bool b_autoON;

private:
    void resetSyncAutoState();

private slots:
	void _slot_syncAuto();
    void _slot_sync_onetime();
	void _slot_timerupdate();
    void reject();
};

class MyComboBox : public QComboBox
{
    Q_OBJECT

public:
    VISIOCYTEPluginCallback2 * m_visiocyte;
    MyComboBox(VISIOCYTEPluginCallback2 * ini_visiocyte) {m_visiocyte = ini_visiocyte;}

    void enterEvent(QEvent * event);

public slots:
    void updateList();
};
class controlPanel: public QDialog
{
    Q_OBJECT

public:
    controlPanel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
    ~controlPanel();

public:
    visiocytehandle curwin;
    VisiocyteR_MainWindow *surface_win;
    View3DControl *view;

    QList <VisiocyteR_MainWindow *> list_3dviewer;
    visiocytehandleList list_triview;
    QGridLayout *gridLayout;
    VISIOCYTEPluginCallback2 & m_visiocyte;

    MyComboBox* combo_surface;
    QLabel* label_surface;
    QCheckBox* check_rotation;
    QCheckBox* check_shift;
    QCheckBox* check_zoom;
    QTimer *m_pTimer;
    QPushButton* syncAuto;
    View3DControl *view_master;
    View3DControl *view_slave;
    int xRot_past, yRot_past,zRot_past;
    int xShift_past,yShift_past,zShift_past;
    int zoom_past;
    bool b_autoON;

private:
    void resetSyncAutoState();

private slots:
     void _slot_syncAuto();
     void _slot_sync_onetime();
     void _slot_timerupdate();
     void reject();


};

#endif
