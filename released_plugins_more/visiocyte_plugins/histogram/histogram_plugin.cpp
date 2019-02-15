/* histogram_plugin.cpp
 * Display histogram of the image
 * 2012-03-01 : by Jianlong Zhou
 */
 
#include "visiocyte_message.h"

#include "histogram_plugin.h"
#include "histogram_func.h"
#include <iostream>
using namespace std;
 
Q_EXPORT_PLUGIN2(histogram, HistogramPlugin);
 
QStringList HistogramPlugin::menulist() const
{
	return QStringList() 
                <<tr("Histogram")
		<<tr("about");
}

QStringList HistogramPlugin::funclist() const
{
	return QStringList()
		<<tr("histogram")
		<<tr("help");
}

void HistogramPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
        if (menu_name == tr("Histogram"))
	{
		compute(callback,parent);
	}
	else
	{
		visiocyte_msg(tr("Display histogram of the image. "
			"Developed by Jianlong Zhou, 2012-03-01"));
	}
}

bool HistogramPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("histogram"))
	{
        return compute(callback, input, output);
	}
	else
	{
		cout<<"\nThis is a plugin to get the histogram of an image. 2012-03-27 by Yinan Wan"<<endl;
		cout<<"\nUsage: visiocyte -x histogram -f histogram -i <image_name> -o <outputfile_name>"<<endl;
		cout<<"\t -i <image_name> :       name of the image to be computed"<<endl;
		cout<<"\t -o <outputfile_name> :  output file name (.csv)"<<endl;
		cout<<"\nDemo visiocyte -x histogram -f histogram -i test.tif -o hist.csv\n"<<endl;
	}
}

