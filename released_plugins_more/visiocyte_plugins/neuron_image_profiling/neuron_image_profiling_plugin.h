/* neuron_image_profiling_plugin.h
 * This is a plugin for profiling image qualities for neuron tracing, via SNR and tubuliarty measures.
 * 2015-7-6 : by Xiaoxiao Liu and Zhi Zhou
 */
 
#ifndef __NEURON_IMAGE_PROFILING_PLUGIN_H__
#define __NEURON_IMAGE_PROFILING_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class image_profiling : public QObject, public VISIOCYTEPluginInterface2_1
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

