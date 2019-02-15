/* affine_transform_swc_plugin.h
 * This plugin can be used to apply an input affine transform to a single SWC file.
 * 2015-6-25 : by Xiaoxiao liu
 */
 
#ifndef __AFFINE_TRANSFORM_SWC_PLUGIN_H__
#define __AFFINE_TRANSFORM_SWC_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class AffineTransformSWC : public QObject, public VISIOCYTEPluginInterface2_1
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

