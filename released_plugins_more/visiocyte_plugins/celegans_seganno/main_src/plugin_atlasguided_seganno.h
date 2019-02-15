//plugin_atlasguided_seganno.h
// by Lei Qu
//2010-07-31

#ifndef __PLUGIN_ATLASGUIDED_SEGANNO_H__
#define __PLUGIN_ATLASGUIDED_SEGANNO_H__

#include <visiocyte_interface.h>


class AtlasGuidedSegAnnoPlugin : public QObject, public VISIOCYTEPluginInterface
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
