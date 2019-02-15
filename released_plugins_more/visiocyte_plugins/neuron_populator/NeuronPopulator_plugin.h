/* NeuronPopulator_plugin.h
 * Use this plugin to simulate a group of neurons with desired density. This simulator also counts the contacts between axons and dendrites.
 * 2015-8-5 : by Xiaoxiao Liu
 */
 
#ifndef __NEURONPOPULATOR_PLUGIN_H__
#define __NEURONPOPULATOR_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class NeuronPopulator : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    bool func_populate( const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback);
    bool func_prune ( const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback);
    bool func_detect( const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback);


    void menu_populate( VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void menu_prune( VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void menu_detect( VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

