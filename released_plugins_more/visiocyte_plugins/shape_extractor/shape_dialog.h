#ifndef SHAPE_DIALOG_H
#define SHAPE_DIALOG_H

#include <QtGui>
#include <QDialog>
#include <QToolBar>
#include <QVBoxLayout>
#include <QToolButton>
#include <visiocyte_interface.h>
#include "extract_fun.h"
#include "basic_surf_objs.h"

using namespace std;

#define NAME_INWIN "Input_Shape_Extractor"
#define NAME_OUTWIN "Output_Shape_Extractor"

class shape_dialog: public QDialog
{
    Q_OBJECT
public:
    explicit shape_dialog(VISIOCYTEPluginCallback2 *cb);
    void core();

private:
    extract_fun shape_ext_obj;
    VISIOCYTEPluginCallback2 *callback;
    VISIOCYTELONG sz_img[4];
    unsigned char *image1Dc_in,*image1Dc_out, *label;//,*label_m;
    LandmarkList LList,LList_in;
    vector<VISIOCYTELONG> poss_landmark;//,volume;
    vector<VISIOCYTELONG> x_all,y_all,z_all;
    int rgb[3];
    visiocytehandle curwin;
    int bg_thr;
    QSpinBox *para_bg;

private:
    void GetColorRGB(int* rgb, int idx);
    void convert2UINT8(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
    void convert2UINT8(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
    void extract();

};
#endif // SHAPE_DIALOG_H
