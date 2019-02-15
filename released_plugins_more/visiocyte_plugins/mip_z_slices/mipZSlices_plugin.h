/* mipZSlices_plugin.h
 * A program to calculate MIP of selected Z slices
 * 2013-08-05 : by Hanchuan Peng
 */
 
#ifndef __MIPZSLICES_PLUGIN_H__
#define __MIPZSLICES_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class MIPZSlices : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 0.9f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

