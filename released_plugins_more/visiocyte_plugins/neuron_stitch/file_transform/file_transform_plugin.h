/* file_transform_plugin.h
 * This plugin will transform and combine image by given affine matrix. Some functions are based on littleQuickWarper.
 * 2015-3-18 : by Hanbo Chen
 */
 
#ifndef __FILE_TRANSFORM_PLUGIN_H__
#define __FILE_TRANSFORM_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class file_transform : public QObject, public VISIOCYTEPluginInterface2_1
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
    void doAffineImage(VISIOCYTEPluginCallback2 & callback, QString fname_sub, QString fname_tar, QString fname_amat,
                       QString fname_output, int interpMethod, bool b_negativeShift, bool b_channelSeperate);
    int dotransform_swc(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    int dotransform_marker(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void printHelp();
};


#endif

