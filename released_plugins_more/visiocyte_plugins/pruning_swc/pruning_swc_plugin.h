/* pruning_swc_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2014-05-02 : by Zhi Zhou
 */
 
#ifndef __pruning_SWC_PLUGIN_H__
#define __pruning_SWC_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class pruning_swc : public QObject, public VISIOCYTEPluginInterface2_1
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

private slots:
     void _slot_prune();
     void _slot_save();
     void reject();


};
