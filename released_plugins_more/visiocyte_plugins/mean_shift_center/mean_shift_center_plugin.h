/* Mean_Shift_Center_plugin.h
 * Search for center using mean-shift
 * 2015-3-4 : by Yujie Li
 */
 
#ifndef __MEAN_SHIFT_CENTER_PLUGIN_H__
#define __MEAN_SHIFT_CENTER_PLUGIN_H__

#include "mean_shift_dialog.h"
#include "ray_shoot_dialog.h"
#include "gradient_transform_dialog.h"


class mean_shift_plugin : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	QStringList funclist() const ;

    //do func functions
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    void mean_shift_center(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, int methodcode);
    void all_method_comp(VISIOCYTEPluginCallback2 *callback);
    void ray_shoot(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input,
                                      VISIOCYTEPluginArgList & output);
    void gradient(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input,
                                     VISIOCYTEPluginArgList & output);

    void all_method_comp_func(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input,
                                                 VISIOCYTEPluginArgList & output);

    void load_image_marker(VISIOCYTEPluginCallback2 & callback,const VISIOCYTEPluginArgList & input,
                  unsigned char * & image1Dc_data,LandmarkList &LList,int &intype,VISIOCYTELONG sz_img[4]);
    //void write_marker(QString qs_output);
    QList <LocationSimple> readPosFile_usingMarkerCode(const char * posFile);
    QList <ImageMarker> readMarker_file(const QString & filename);
    void printHelp();

private:
    VISIOCYTELONG sz_img[4];
    unsigned char *image_data;
    int intype;
    LandmarkList LList,LList_new_center;
};

#endif

