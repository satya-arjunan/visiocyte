
//by Hanchuan Peng
//2009-06-26

#ifndef __example_reset_xyz_resolution_PLUGIN_H__
#define __example_reset_xyz_resolution_PLUGIN_H__


#include <visiocyte_interface.h>

class example_reset_xyz_resolutionPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1)

public:
     float getPluginVersion() const {return 1.2f;}

     QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

};

#endif
