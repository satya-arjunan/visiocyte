/* sync_swc_terafly_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2017-8-29 : by Zhi Zhou
 */
 
#ifndef __SYNC_SWC_TERAFLY_PLUGIN_H__
#define __SYNC_SWC_TERAFLY_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class sync_swc_terafly : public QObject, public VISIOCYTEPluginInterface2_1
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

class lookPanel: public QDialog
{
    Q_OBJECT

public:
    lookPanel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
    ~lookPanel();

    QGridLayout *gridLayout;
    VISIOCYTEPluginCallback2 & m_visiocyte;


private slots:
    void _slot_sync_onetime();
    void _slot_set_markers();

};

#endif

