#ifndef __OPENSWCDIALOG_H_
#define __OPENSWCDIALOG_H_
#include <QtGui>
#include "visiocyte_interface.h"
class OpenSWCDialog: public QDialog
{
	Q_OBJECT

public:
	VISIOCYTEPluginCallback2 * callback;
	QListWidget * listWidget;
	QList<NeuronTree> _3DViewerTreeList;
	NeuronTree  nt;
	QString file_name;

	OpenSWCDialog(QWidget * parent, VISIOCYTEPluginCallback2 * callback);
	void getAllNeuronTreeNames(VISIOCYTEPluginCallback2 * callback);

public slots:
	bool run();
	bool setTree(const QString & file);
};


#endif
