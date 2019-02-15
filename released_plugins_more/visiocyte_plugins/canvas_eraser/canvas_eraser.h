
//by Hanchuan Peng
//2009-08-14

// upgrade to VISIOCYTEPluginInterface2_1 2012-04-06, by Jianlong Zhou

#ifndef __CanvasEraserPLUGIN_H__
#define __CanvasEraserPLUGIN_H__


#include <visiocyte_interface.h>

class CanvasEraserPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1)


public:
     float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif
