/* ting1_plugin.h
 * a collaboration with Hanchuan
 * 2012-12-9 : by Ting Zhao and HP
 */
 
#ifndef __TING1_PLUGIN_H__
#define __TING1_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class neutube : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 0.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

