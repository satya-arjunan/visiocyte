
//by Hanchuan Peng
//2010-May-30

#ifndef __EDGE_OF_MASKIMG_PLUGIN_H__
#define __EDGE_OF_MASKIMG_PLUGIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <visiocyte_interface.h>

class EdgeOfMaskImgPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.2f;}
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif
