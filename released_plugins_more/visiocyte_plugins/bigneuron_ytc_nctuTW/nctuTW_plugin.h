/* nctuTW_plugin.h
 * This is the implementation of the tracing algorithm published in PLoS Computational Biology, "High-throughput Computer Method for 3D Neuronal Structure Reconstruction from the Image Stack of the Drosophila Brain and Its Applications", September 13, 2013. Implemented by P. C. Lee, K. W. He, C. C. Cheng, Y. T. Ching.
 * 2015-5-23 : by P. C. Lee, K. W. He, C. C. Cheng, Y. T. Ching.
 */
 
#ifndef __NCTUTW_PLUGIN_H__
#define __NCTUTW_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class nctuTW : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

