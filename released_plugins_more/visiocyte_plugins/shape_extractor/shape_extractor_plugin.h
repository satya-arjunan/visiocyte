/* shape_extractor_plugin.h
 * A tool to extract cell shapes
 * 2015-2-17 : by Yujie Li
 */
 
#ifndef __SHAPE_EXTRACTOR_PLUGIN_H__
#define __SHAPE_EXTRACTOR_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class shape_extr_plugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);


public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void shape_extract(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);


private:
    VISIOCYTELONG sz_img[4];
    unsigned char *image_data;
};

#endif

