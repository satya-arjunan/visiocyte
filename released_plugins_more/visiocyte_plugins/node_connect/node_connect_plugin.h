/* node_connect_plugin.h
 * This is a plugin that takes two marker point inputs, and connects the two swc nodes corresponding to the same location.
 * 2014-08-08 : by SurobhiGanguly
 */
 
#ifndef __NODE_CONNECT_PLUGIN_H__
#define __NODE_CONNECT_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class TestPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1)

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
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

    VISIOCYTEPluginCallback2 & m_visiocyte;

    QGridLayout *gridLayout;

    MyComboBox* combo_surface;
    QLabel* status_load;
    QLabel* status_connect;


private slots:
    void _slot_load();
    void _slot_connect();
    //void _slot_typeset_marker();
    //void _slot_refresh();


    //void _slot_save();

};

#endif

