/* neuron_dist_plugin.cpp
 * The plugin to calculate distance between two neurons. Distance is defined as the average distance among all nearest pairs in two neurons.
 * 2012-05-04 : by Yinan Wan
 */
 
#include "visiocyte_message.h"

#include "neuron_dist_plugin.h"
#include "neuron_dist_func.h"
 
Q_EXPORT_PLUGIN2(neuron_dist, NeuronDistPlugin);
 
QStringList NeuronDistPlugin::menulist() const
{
	return QStringList() 
		<<tr("neuron_dist")
        <<tr("neuron_dist_mask_area")
		<<tr("about");
}

QStringList NeuronDistPlugin::funclist() const
{
	return QStringList()
		<<tr("neuron_distance")
		<<tr("help");
}

void NeuronDistPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("neuron_dist"))
	{
		neuron_dist_io(callback,parent);
    }else if (menu_name == tr("neuron_dist_mask_area"))
    {
        neuron_dist_mask(callback,parent);
    }
	else
	{
		visiocyte_msg(tr("The plugin to calculate distance between two neurons. Distance is defined as the average distance among all nearest pairs in two neurons.. "
            "Developed by Yinan Wan and Hanchuan Peng, 2012-05-04"));
	}
}

bool NeuronDistPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("neuron_distance"))
	{
		return neuron_dist_io(input, output);
	}
	else if (func_name == tr("help"))
	{
		printHelp();
	}
	else if (func_name == tr("TOOLBOXneuron_dist"))
	{
		neuron_dist_toolbox(input, callback);
		return true;
	}
}

