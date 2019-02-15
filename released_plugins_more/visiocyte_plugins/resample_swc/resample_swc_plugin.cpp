/* resample_swc_plugin.cpp
 * This is a plugin to resample neuron swc subject to a fixed step length.
 * 2012-03-02 : by Yinan Wan
 */
 
#include "visiocyte_message.h"

#include "resample_swc_plugin.h"
#include "resample_swc_func.h"
 
Q_EXPORT_PLUGIN2(resample_swc, ResamplePlugin);
 
QStringList ResamplePlugin::menulist() const
{
	return QStringList() 
		<<tr("resample")
		<<tr("about");
}

QStringList ResamplePlugin::funclist() const
{
	return QStringList()
		<<tr("resample_swc")
		<<tr("help");
}

void ResamplePlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("resample"))
	{
		resample_swc(callback,parent);
	}
	else
	{
		visiocyte_msg(tr("This is a plugin to resample neuron swc subject to a fixed step length.\n"
			"Developed by Yinan Wan, 2012-03-02"));
	}
}

bool ResamplePlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("resample_swc"))
	{
		return resample_swc(input, output);
	}
	else if (func_name == tr("TOOLBOXresample"))
	{
		resample_swc_toolbox(input);
		return true;
	}
	if (func_name == tr("help"))
	{
		printf("\n\n(version 1.0) Resample points in a swc file subject to a fixed step length. Developed by Yinan Wan 2012-03-02\n");
		printf("usage:\n");
		printf("\t-f <function_name>:     resample_swc\n");
		printf("\t-i <input_file_name>:   input .swc\n");
		printf("\t-o <output_file_name>:  (not required) resampled swc file. DEFAULT: 'inputName_resampled.swc'\n");
		printf("\t-p <step_length>:       step length for resampling.\n");
		printf("Demo: visiocyte -x resample_swc -f resample_swc -i test.swc -o test_resampled.swc -p 1\n\n");
	}
	return false;
}

