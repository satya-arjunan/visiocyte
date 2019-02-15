/* convert_neuron_file_to_SWC_plugin.h
 * The plugin will convert neuron file to swc format. It is based on the code of L-Measure, Sridevi Polavaram. Imported by Hanbo Chen.
 * 2015-4-27 : by Hanbo Chen
 */
 
#ifndef __CONVERT_NEURON_FILE_TO_SWC_PLUGIN_H__
#define __CONVERT_NEURON_FILE_TO_SWC_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class neuronConverter : public QObject, public VISIOCYTEPluginInterface2_1
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

bool doASC2SWC(QString fname_open, QString fname_save);
void printHelp();
#endif

