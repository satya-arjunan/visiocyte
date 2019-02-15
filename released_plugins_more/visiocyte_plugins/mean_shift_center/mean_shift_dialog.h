#ifndef MEAN_SHIFT_DIALOG_H
#define MEAN_SHIFT_DIALOG_H

#include "mean_shift_fun.h"


class mean_shift_dialog:public QDialog
{
    Q_OBJECT
public:
    mean_shift_dialog(VISIOCYTEPluginCallback2 *cb,int method);
    mean_shift_fun mean_shift_obj;
    LandmarkList core();
    //void create_sphere();

private:
    VISIOCYTEPluginCallback2 *callback;
    visiocytehandle curwin;
    VISIOCYTELONG sz_img[4];
    unsigned char *image1Dc_in;
    LandmarkList LList,LList_in,LList_new_center;
    vector<VISIOCYTELONG> poss_landmark;
    vector<float> mass_center;
    ImagePixelType pixeltype;
    int methodcode;

};




#endif // MEAN_SHIFT_DIALOG_H
