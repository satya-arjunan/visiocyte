#ifndef __PLUGIN_SNAPSHOTS_3DVIEWER_H__
#define __PLUGIN_SNAPSHOTS_3DVIEWER_H__

#include <QtGui>
#include "visiocyte_interface.h"

class SnapShots_3Dviewer: public QObject, public VISIOCYTEPluginInterface2_1
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


#endif

