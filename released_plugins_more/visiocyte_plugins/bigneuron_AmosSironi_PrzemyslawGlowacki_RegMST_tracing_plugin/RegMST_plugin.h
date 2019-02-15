/* RegMST_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2015-6-23 : by Amos Sironi and Przemyslaw Glowacki
 */
 
#ifndef __REGMST_PLUGIN_H__
#define __REGMST_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

#include "visiocyte_message.h"
#include <vector>
//#include "SQBTree_plugin.h"
//#include "sqb_trees.h"


#include "../bigneuron_AmosSironi_PrzemyslawGlowacki_SQBTree_plugin/libs/sqb_0.1/src/MatrixSQB/visiocyte_link.h"

#include "../bigneuron_AmosSironi_PrzemyslawGlowacki_SQBTree_plugin/libs/regression/sep_conv.h"
#include "../bigneuron_AmosSironi_PrzemyslawGlowacki_SQBTree_plugin/libs/regression/util.h"
//#include "regression/regression_test2.h"

class RegMST : public QObject, public VISIOCYTEPluginInterface2_1
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

