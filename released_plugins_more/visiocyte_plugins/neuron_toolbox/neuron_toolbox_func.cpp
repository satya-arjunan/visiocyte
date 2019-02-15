/* neuron_toolbox_func.cpp
 * This is a super plugin that gather all sub-plugins related to neuron structure processing
 * 2012-04-06 : by Yinan Wan
 */

#include <visiocyte_interface.h>
#include "visiocyte_message.h"
#include "neuron_toolbox_func.h"
#include "toolbox_gui.h"
#include "customary_structs/visiocyte_neurontoolbox_para.h"
#include <iostream>
using namespace std;

const QString title = QObject::tr("Neuron Toolbox");

//dofunc for neuron toolbox is reserved for pop-up menu in main program
bool neuron_toolbox_func(VISIOCYTEPluginCallback2 & callback, QWidget * parent)
{
	SelectPluginDlg * selectDlg = new SelectPluginDlg(parent, callback);
	selectDlg->setWindowTitle("Select plugin to run...");
	selectDlg->show();
	return true;

}

bool neuron_toolbox_func(VISIOCYTEPluginCallback2 & callback, QWidget * parent, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	QString test_str1 = QString(((visiocyte_neurontoolbox_paras *)(input.at(0).p))->nt.file);
	QString test_str = QFileInfo(test_str1).baseName() + "." + QFileInfo(test_str1).completeSuffix();
	SelectPluginDlg * selectDlg = new SelectPluginDlg(parent, callback, input);
	selectDlg->setWindowTitle(test_str + " - Select plugin to run...");
	selectDlg->show();
	return true;
}

void help(bool b_useWin)
{
    visiocyte_msg("This is a super plugin that gather all sub-plugins related to neuron structure processing.. Developed by Yinan Wan, 2012-04-06", b_useWin);
}


