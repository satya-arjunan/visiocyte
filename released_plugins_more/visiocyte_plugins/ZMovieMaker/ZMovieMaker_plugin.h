/* ZMovieMaker_plugin.h
 * This plugin can be used to generate a smooth movie by several points
 * 2013-11-21 : by Zhi Zhou
 */
 
#ifndef __ZMOVIEMAKER_PLUGIN_H__
#define __ZMOVIEMAKER_PLUGIN_H__

#include <QtGui>

#include "visiocyte_interface.h"

class ZMovieMaker : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 0.96f;}

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
    static controlPanel *panel;


    QList <VisiocyteR_MainWindow *> list_3dviewer;
    visiocytehandleList list_triview;

    VISIOCYTEPluginCallback2 & m_visiocyte;

    QGridLayout *gridLayout;
    QListWidget *list_anchors;
    QSpinBox* box_SampleRate;

    MyComboBox* combo_surface;
    QLabel* label_surface;

    bool saveAnchorFile(QString filename);

private slots:
    void _slot_record();
    void _slot_preview();
    void _slot_show_item(QListWidgetItem *item);
    void _slot_show();
    void _slot_delete();
    void _slot_save();
    void _slot_load();
    void _slot_up();
    void _slot_down();
    void _slot_batch();
    void _slot_snapshot();

};

bool _saveAnchorFile(QString filename, QStringList ParaLists, bool b_append);


#endif

