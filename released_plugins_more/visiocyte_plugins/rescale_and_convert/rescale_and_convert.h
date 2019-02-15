/* rescale_and_convert.h
 * by Hanchuan Peng, 2012-04-11
 */


#ifndef __RESCALE_AND_CONVERT_H__
#define __RESCALE_AND_CONVERT_H__ 

#include <visiocyte_interface.h>

class RescaleConvertPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
     Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 0.91f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif


