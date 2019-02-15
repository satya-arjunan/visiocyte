#ifndef GRADIENT_TRANSFORM_DIALOG_H
#define GRADIENT_TRANSFORM_DIALOG_H

#include "mean_shift_fun.h"

class gradient_transform_dialog:public QDialog
{
    Q_OBJECT
public:
    gradient_transform_dialog(VISIOCYTEPluginCallback2 *cb);
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
#endif // GRADIENT_TRANSFORM_DIALOG_H




