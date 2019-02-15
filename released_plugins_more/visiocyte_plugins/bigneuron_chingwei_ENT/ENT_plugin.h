/* ENT_plugin.h
 * The plugin is called ensemble neuron tracer.
 * Developers: Ching-Wei Wang, Hilmil Pradana, Cheng-Ta Huang 
 * Institution: National Taiwan University of Science and Technology
 * Website: http://www-o.ntust.edu.tw/~cweiwang/
 * Email: cweiwang@mail.ntust.edu.tw
 * Release date: 2016-1-8
 */
 
#ifndef __APP2_PORTED_PLUGIN_H__
#define __APP2_PORTED_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class APP2_ported : public QObject, public VISIOCYTEPluginInterface2_1
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

