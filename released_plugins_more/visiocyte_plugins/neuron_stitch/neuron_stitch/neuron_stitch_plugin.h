/* neuron_stitch_plugin.h
 * This plugin is for link neuron segments across stacks.
 * 2014-10-07 : by Hanbo Chen
 */
 
#ifndef __NEURON_STITCH_PLUGIN_H__
#define __NEURON_STITCH_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>
#include "basic_surf_objs.h"


class neuron_stitch : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 2.0f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);


private:
    void domatch(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void dostitch(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void doadjust(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void printHelp();
    void doperformancetest(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback);
    void dogmhashtest(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback);
};

#endif

