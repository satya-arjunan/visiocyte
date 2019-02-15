//plugin_atlasguided_stranno.h
// by Lei Qu
//2010-11-01

#ifndef __PLUGIN_ATLASGUIDED_STRANNO_H__
#define __PLUGIN_ATLASGUIDED_STRANNO_H__

#include <visiocyte_interface.h>


class AtlasGuidedStrAnnoPartialPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const {return QStringList();}
	void dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, QWidget *parent) {}
};


#endif
