/* neuron_tile_display_plugin.h
 * This Plugin will tile neuron to display
 * 2014-10-28 : by Hanbo Chen
 */
 
#ifndef __NEURON_TILE_DISPLAY_PLUGIN_H__
#define __NEURON_TILE_DISPLAY_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>
#include "neuron_tile_display_dialog.h"

class neuron_tile_display : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    void dotile(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void doxytile(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void dotile_groups(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif

