
//by Lei Qu
//2009-11-10

#ifndef __PLUGIN_POINTCLOUD_MATCH_H__
#define __PLUGIN_POINTCLOUD_MATCH_H__

#include <visiocyte_interface.h>


class PointCloudMatchPlugin : public QObject, public VISIOCYTEPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface);

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback &callback, QWidget *parent);

	QStringList funclist() const {return QStringList();}
	void dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, QWidget *parent) {}
};


#endif
