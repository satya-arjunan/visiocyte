//plugin_warp_affine_tps.h
//by Lei Qu
//2010-03-22

#ifndef __PLUGIN_WARP_AFFINE_TPS_H__
#define __PLUGIN_WARP_AFFINE_TPS_H__

#include <visiocyte_interface.h>


class WarpAffineTPSPlugin : public QObject, public VISIOCYTEPluginInterface
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
