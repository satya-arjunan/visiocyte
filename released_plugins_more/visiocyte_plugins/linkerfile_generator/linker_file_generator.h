// generate an linker file (.ano) for all swc, apo, etc files in the given directory
// by Lei Qu
// 2009-12-30
//last change: by Hanchuan Peng. 2010-04-21

#ifndef __PLUGIN_ANOGENERATOR_H__
#define __PLUGIN_ANOGENERATOR_H__

#include "visiocyte_interface.h"

class LinkerPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
     float getPluginVersion() const {return 1.1f;}
};

#endif

