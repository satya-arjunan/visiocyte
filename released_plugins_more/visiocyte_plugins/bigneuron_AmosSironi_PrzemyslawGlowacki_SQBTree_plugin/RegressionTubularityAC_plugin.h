/* SQBTree_plugin.h
 */
 
#ifndef __SQBTREE_PLUGIN_H__
#define __SQBTREE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

#include "visiocyte_message.h"
#include <vector>

#include "./libs/sqb_0.1/src/MatrixSQB/visiocyte_link.h"
#include "./libs/regression/sep_conv.h"
#include "./libs/regression/util.h"


class RegressionTubularityACPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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


bool testTubularityImage(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);


#endif

