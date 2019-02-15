/* marker_minspanningtree_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2013-08-03 : by Zhi Zhou
 */
 
#ifndef __MARKER_MINSPANNINGTREE_PLUGIN_H__
#define __MARKER_MINSPANNINGTREE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class markertree : public QObject, public VISIOCYTEPluginInterface2_1
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

