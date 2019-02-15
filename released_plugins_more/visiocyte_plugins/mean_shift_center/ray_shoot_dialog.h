#ifndef RAY_SHOOT_DIALOG_H
#define RAY_SHOOT_DIALOG_H

#include "mean_shift_fun.h"

class ray_shoot_dialog:public QDialog
{
    Q_OBJECT
public:
    ray_shoot_dialog(VISIOCYTEPluginCallback2 *cb);
    mean_shift_fun mean_shift_obj;
    void core();

private:

    VISIOCYTEPluginCallback2 *callback;
    visiocytehandle curwin;
    VISIOCYTELONG sz_img[4];
    unsigned char *image1Dc_in;
    LandmarkList LList,LList_in,LList_new_center;
    vector<VISIOCYTELONG> poss_landmark;
    vector<float> mass_center;
    ImagePixelType pixeltype;

};

#endif // RAY_SHOOT_DIALOG_H
