/* NeuroStalker_plugin.h
 * A learning-based tracing algorithm
 * 2015-4-25 : by Siqi Liu, Donghao Zhang
 */
 
#ifndef __NEUROSTALKER_PLUGIN_H__
#define __NEUROSTALKER_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>
#include "lib/ImageOperation.h"
#include "lib/PointOperation.h"
using namespace std;

class NeuroStalker : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};



#endif

