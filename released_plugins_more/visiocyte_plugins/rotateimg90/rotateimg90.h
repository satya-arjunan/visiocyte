
// upgrade to VISIOCYTEPluginInterface2_1 2012-04-06, by Jianlong Zhou


#ifndef ROTATEIMG90PLUGIN_H
#define ROTATEIMG90PLUGIN_H


#include <visiocyte_interface.h>

class RotateImg90Plugin : public QObject, public VISIOCYTEPluginInterface2_1
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
