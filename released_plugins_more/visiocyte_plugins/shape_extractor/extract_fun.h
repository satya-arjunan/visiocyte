#ifndef EXTRACT_FUN_H
#define EXTRACT_FUN_H

#include <QtGui>
#include <visiocyte_interface.h>
#include <vector>
#include <math.h>
#include "shape_extr_template.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
//#define pi 3.14

using namespace std;

unsigned char * memory_allocate_uchar1D(const VISIOCYTELONG i_size);
void memory_free_uchar1D(unsigned char *ptr_input);
float * memory_allocate_float1D(const VISIOCYTELONG i_size);
void memory_free_float1D(float *ptr_input);
vector<VISIOCYTELONG>landMarkList2poss(LandmarkList LandmarkList_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offest_Z);
VISIOCYTELONG landMark2pos(LocationSimple Landmark_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offset_Z);
vector<VISIOCYTELONG> pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);
VISIOCYTELONG xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);

class extract_fun
{
public:
    extract_fun();
    ~extract_fun();

    vector<float> getProjectionDirection(VISIOCYTELONG seed_ind, int neighbor_size, int bg_thr);
    float getProjection(vector<float> vec, vector<float> dir, int convolute_iter);
    VISIOCYTELONG extract(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all,vector<VISIOCYTELONG>& z_all,unsigned char * label,VISIOCYTELONG seed_ind,
                     int convolute_iter,int bg_thr,int marker);
    vector<VISIOCYTELONG> get_mass_center(vector<VISIOCYTELONG> x_all, vector<VISIOCYTELONG> y_all, vector<VISIOCYTELONG> z_all);

private:

    VISIOCYTELONG page_size;
    float * data1Dc_float;

public:

    VISIOCYTELONG sz_image[4];
    unsigned char * mask1D;
    template <class T>
    void pushNewData(T * data1Dc_in, VISIOCYTELONG sz_img[4])
    {
        if(data1Dc_float!=0){
            memory_free_float1D(data1Dc_float);
        }
        if(mask1D!=0){
            memory_free_uchar1D(mask1D);
        }

        sz_image[0]=sz_img[0];
        sz_image[1]=sz_img[1];
        sz_image[2]=sz_img[2];
        sz_image[3]=sz_img[3];
        page_size=sz_image[0]*sz_image[1]*sz_image[2];

        data1Dc_float = memory_allocate_float1D(sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]);
        for(VISIOCYTELONG i=0; i<page_size*sz_img[3]; i++){
            data1Dc_float[i]=(float) (data1Dc_in[i]);
        }
        normalizeEachChannelTo255<float>(data1Dc_float, sz_img);
        mask1D = memory_allocate_uchar1D(page_size);
        memset(mask1D, 0, page_size*sizeof(unsigned char));

    }
};


#endif // EXTRACT_FUN_H
