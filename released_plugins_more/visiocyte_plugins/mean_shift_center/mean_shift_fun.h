#ifndef MEAN_SHIFT_FUN_H
#define MEAN_SHIFT_FUN_H

#include <QtGui>
#include <vector>
#include <math.h>
#include "mean_shift_extr_template.h"
#include "basic_surf_objs.h"
#include "fastmarching_dt.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#ifndef ABS
#define ABS(x) ((x) > 0 ? (x) : (-(x)))
#endif

using namespace std;

unsigned char * memory_allocate_uchar1D(const VISIOCYTELONG i_size);
void memory_free_uchar1D(unsigned char *ptr_input);
float * memory_allocate_float1D(const VISIOCYTELONG i_size);
void memory_free_float1D(float *ptr_input);
vector<VISIOCYTELONG>landMarkList2poss(LandmarkList LandmarkList_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offest_Z);
VISIOCYTELONG landMark2pos(LocationSimple Landmark_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offset_Z);
vector<VISIOCYTELONG> pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);
VISIOCYTELONG xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);

void convert2UINT8_meanshift(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
void convert2UINT8_meanshift(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);

vector<float> calc_mean_shift_center(VISIOCYTELONG ind, int windowradius,float *data1Dc_float,VISIOCYTELONG sz_image[],int methodcode);
vector<float> calc_mean_shift_center_mass(VISIOCYTELONG ind, int windowradius,float *data1Dc_float,VISIOCYTELONG sz_image[],int methodcode);

bool load_data(VISIOCYTEPluginCallback2 *cb,unsigned char * &image1Dc_in,LandmarkList &LList,
               ImagePixelType &pixeltype,VISIOCYTELONG sz_img[4],visiocytehandle &curwin);

class mean_shift_fun
{
public:
    mean_shift_fun();
    ~mean_shift_fun();
    vector<float> mean_shift_center(VISIOCYTELONG ind, int windowradius);
    vector<float> mean_shift_center_mass(VISIOCYTELONG ind, int windowradius);  //added by Zhi 90122017

    vector<float> mean_shift_with_constraint(VISIOCYTELONG ind, int windowradius);
    vector<float> ray_shoot_center(VISIOCYTELONG ind, int bg_thr, int j);
    vector<float> gradient_transform(float *&outimg1d,VISIOCYTELONG ind,
                        int bg_thr, int cnn_type,int z_thickness,int halfwindowsize, int search_radius);

private:

    VISIOCYTELONG page_size;
    float * data1Dc_float;

public:

    VISIOCYTELONG sz_image[4];
    //unsigned char * mask1D;
    template <class T>
    void pushNewData(T * data1Dc_in, VISIOCYTELONG sz_img[4])
    {
        if(data1Dc_float!=0){
            memory_free_float1D(data1Dc_float);
        }
        sz_image[0]=sz_img[0];
        sz_image[1]=sz_img[1];
        sz_image[2]=sz_img[2];
        sz_image[3]=sz_img[3];
        page_size=sz_image[0]*sz_image[1]*sz_image[2];

        data1Dc_float = memory_allocate_float1D(sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]);
        for(VISIOCYTELONG i=0; i<page_size*sz_img[3]; i++){
            data1Dc_float[i] = (float)(data1Dc_in[i]);
        }
      //  normalizeEachChannelTo255<float>(data1Dc_float, sz_img);

    }
};

#endif // MEAN_SHIFT_FUN_H
