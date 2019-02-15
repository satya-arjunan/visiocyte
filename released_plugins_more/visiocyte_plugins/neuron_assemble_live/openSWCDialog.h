#ifndef __OPENSWCDIALOG_H_
#define __OPENSWCDIALOG_H_
#include <QtGui>
#include "visiocyte_interface.h"
class OpenSWCDialog: public QDialog
{
	Q_OBJECT

public:
	VISIOCYTEPluginCallback2 * callback;
    QListWidget * listWidget, * listWidget1;
    QStringList _3DViewerTreeList_name;
    QList<int> _3DViewerTreeList_id;
    QList<VisiocyteR_MainWindow *> _3DViewerTreeList_window;
    QList<VisiocyteR_MainWindow *> _3DViewerList;
    QList<NeuronTree>  ntList;
    QString file_name;
    Image4DSimple * p_img4d;
    LandmarkList markerlist;

	OpenSWCDialog(QWidget * parent, VISIOCYTEPluginCallback2 * callback);
private:
    void getAllWindowsAndNeurons(VISIOCYTEPluginCallback2 * callback);
    void getImage();

public slots:
    bool chooseNeuron();
    bool chooseWindow();
    bool chooseNeuronTF();

	bool setTree(const QString & file);
};


#endif
