/* Paint_plugin.h
 * This is a paint toolbox
 * 2015-02-04 : by Yujie Li
 */
 
#ifndef __PAINT_PLUGIN_H__
#define __PAINT_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>
#include <QImage>
#include <QPoint>
#include <QWidget>

class paint : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);


private:
    QString fileName;
    VISIOCYTEPluginCallback2 * callback;



};


#endif

