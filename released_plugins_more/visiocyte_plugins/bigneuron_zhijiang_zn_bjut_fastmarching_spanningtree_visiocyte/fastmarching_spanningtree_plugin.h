/* fastmarching_spanningtree_plugin.h
 * ported method from John Wan
 * 2015-3-16 : by John Wan, Hancuuan Peng
 */
 
#ifndef __FASTMARCHING_SPANNINGTREE_PLUGIN_H__
#define __FASTMARCHING_SPANNINGTREE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class fastmarching_spanningtree_Plugin : public QObject, public VISIOCYTEPluginInterface2_1
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

