//celegans straighten plugin
//by Lei Qu
//2010-08-09
// Upgraded to VISIOCYTEPluginInterface2_1 by Jianlong Zhou, 2012-04-05

#ifndef __PLUGIN_CELEGANS_STRAIGHTEN_H__
#define __PLUGIN_CELEGANS_STRAIGHTEN_H__

#include <visiocyte_interface.h>


class CElegansStraightenPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;

     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};



#endif
