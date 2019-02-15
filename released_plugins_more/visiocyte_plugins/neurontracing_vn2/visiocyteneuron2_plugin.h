//last change: by Hanchuan Peng. 2012-09-21

#ifndef __APP2_PLUGIN_H__
#define __APP2_PLUGIN_H__

#include <visiocyte_interface.h>

//NTApp2: NeuronTracing-APP2

class NTApp2Plugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.21f;}

	QStringList menulist() const;

	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;

	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};


#endif

