//plugin_atlasguided_stranno_partial.h
// by Lei Qu
//2010-11-09
// Upgraded to VISIOCYTEPluginInterface2_1 by Jianlong Zhou, 2012-04-05
// version 1.11 by Hanchuan Peng, 2013-11-21

#ifndef __PLUGIN_ATLASGUIDED_STRANNO_PARTIAL_H__
#define __PLUGIN_ATLASGUIDED_STRANNO_PARTIAL_H__

#include "visiocyte_interface.h"


class AtlasGuidedStrAnnoPartialPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.12f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};


#endif
