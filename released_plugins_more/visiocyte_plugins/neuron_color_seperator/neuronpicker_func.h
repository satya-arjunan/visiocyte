#ifndef NEURONPICKER_FUNC_H
#define NEURONPICKER_FUNC_H

#include <QtGui>
#include <visiocyte_interface.h>
#include <vector>
#include "neuronpicker_templates.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

using namespace std;

class neuronPickerMain
{
public:
    neuronPickerMain() {}
    ~neuronPickerMain() {}

    static unsigned char * memory_allocate_uchar1D(const VISIOCYTELONG i_size);
    static void memory_free_uchar1D(unsigned char *ptr_input);
    static int * memory_allocate_int1D(const VISIOCYTELONG i_size);
    static void memory_free_int1D(int *ptr_input);
    static float * memory_allocate_float1D(const VISIOCYTELONG i_size);
    static void memory_free_float1D(float *ptr_input);

    static vector<VISIOCYTELONG> landMarkList2poss(LandmarkList LandmarkList_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offest_Z);

    static void initChannels_rgb(unsigned char *image1Dc, int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, VISIOCYTELONG sz_img[4], const int bg_thr);
    static VISIOCYTELONG extract_mono(int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, unsigned char *image1D_out, VISIOCYTELONG seed, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4]);
    static VISIOCYTELONG extract(int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, unsigned char *image1D_out, VISIOCYTELONG seed, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4]);
    static VISIOCYTELONG extract_color(int *image1D_h, unsigned char *image1Dc_in, unsigned char *image1D_s, unsigned char *image1Dc_out, VISIOCYTELONG seed, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4]);
    static void autoSeeds(int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, vector<VISIOCYTELONG>& seeds, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4], int fgthr, int sizethr);
    static void saveSingleMarker(VISIOCYTELONG pos_landmark, QString fname, VISIOCYTELONG sz_img[4]);

//private:
    static int huedis(int a, int b);
    static unsigned char saturationdis(unsigned char a, unsigned char b);
    static void findMaxVal(unsigned char *image1D_v, VISIOCYTELONG len, VISIOCYTELONG & maxIdx, unsigned char & maxVal);
    static VISIOCYTELONG landMark2pos(LocationSimple Landmark_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offset_Z);
    static vector<VISIOCYTELONG> pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);
    static VISIOCYTELONG xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);
    static void rgb2hsv(const unsigned char R, const unsigned char G, const unsigned char B, int & h, unsigned char & s, unsigned char & v);
    static int rgb2hue(const unsigned char R, const unsigned char G, const unsigned char B);

};

class neuronPickerMain2
{
public:
    neuronPickerMain2();
    ~neuronPickerMain2();

    //check eligiblility of the extraction
    //1st, cannot be a dense extraction
    //2nd, if touch a face, must be relatively (thr_touching) far away from that face
    bool checkEligibility(vector<VISIOCYTELONG> x_all, vector<VISIOCYTELONG> y_all, vector<VISIOCYTELONG> z_all, float thr_sparse, float thr_touching, int cubeSize);
    //extract the eligiable area only
    VISIOCYTELONG extract(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all, vector<VISIOCYTELONG>& z_all, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr);
    //extract the eligiable area only, the energy matrix will be used for eligibility measurement
    VISIOCYTELONG extract_eng(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all, vector<VISIOCYTELONG>& z_all, float * energy, float * energy_seed, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, double thr_bg[5]);
    //extract both the eligialbe area and close neighbors
    VISIOCYTELONG extractMore(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all, vector<VISIOCYTELONG>& z_all, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr);
    //extract and output extraction to uchar
    VISIOCYTELONG extract_uchar(unsigned char*& image1D_out, VISIOCYTELONG sz_out[4], VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr);
    //extract and output extraction to uchar with margin value
    VISIOCYTELONG extractMargin_uchar(unsigned char*& image1D_out, VISIOCYTELONG sz_out[4], VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr, int margin_size);
    //extract the sub area for neuron detected
    VISIOCYTELONG extractSub_uchar(unsigned char*& image1D_out, VISIOCYTELONG sz_out[4], VISIOCYTELONG& pos_new, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr);
    //find the seeds based on extractMore
    VISIOCYTELONG autoSeeds(vector<VISIOCYTELONG>& seeds, int cubSize, int conviter, int fgthr, int bgthr, int sizethr);
    //find, extract and save
    //thr_bg[0/1]: intensity threshould
    //thr_bg[2]: 1st channel threshould
    //thr_bg[3]: 2nd channel threshould
    //thr_bg[4]: 3rd channel threshould
    VISIOCYTELONG autoAll(QString fname_output, VISIOCYTEPluginCallback2 * callback, int cubSize, int conviter, int fgthr, double bgthr[5], int sizethr, int margin_size,  float thr_sparse, float thr_touching);

private:
    vector<float> getProjectionDirection(VISIOCYTELONG seed_ind, int neighbor_size, int bg_thr, int convolute_iter);
    float getProjection(vector<float> vec, vector<float> dir, int convolute_iter);

private:
    float * data1Dc_float;
    unsigned char * mask1D;
    VISIOCYTELONG sz_image[4];
    VISIOCYTELONG page_size;
public:
    float innerScale;

public:
    template <class T>
    void pushNewData(T * data1Dc_in, VISIOCYTELONG sz_img[4])
    {
        if(data1Dc_float!=0){
            neuronPickerMain::memory_free_float1D(data1Dc_float);
        }
        if(mask1D!=0){
            neuronPickerMain::memory_free_uchar1D(mask1D);
        }

        sz_image[0]=sz_img[0];
        sz_image[1]=sz_img[1];
        sz_image[2]=sz_img[2];
        sz_image[3]=sz_img[3];
        page_size=sz_image[0]*sz_image[1]*sz_image[2];

        data1Dc_float = neuronPickerMain::memory_allocate_float1D(sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]);
        for(VISIOCYTELONG i=0; i<page_size*sz_img[3]; i++){
            data1Dc_float[i]=(float) (data1Dc_in[i]);
        }
        normalizeEachChannelTo255<float>(data1Dc_float, sz_img);
//        shift2bitsTo255<float>(data1Dc_float, sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]);
        mask1D = neuronPickerMain::memory_allocate_uchar1D(page_size);
    }

    template <class T>
    double estimateThrByMask(T *data1D_mask, T maskthr, double estimates[5])
    {
        double vol_num=0;
        estimates[0]=0; //mean all channel
        estimates[1]=0; //mean max all channel
        estimates[2]=0; //mean first channel
        estimates[3]=0; //mean second channel
        estimates[4]=0; //mean third channel
        for(VISIOCYTELONG vid=0; vid<page_size; vid++){
            if(data1D_mask[vid]>maskthr){
                double maxval=MAX(data1Dc_float[vid],data1Dc_float[vid+page_size]);
                maxval=MAX(maxval,data1Dc_float[vid+page_size*2]);
                double minval=MIN(data1Dc_float[vid],data1Dc_float[vid+page_size]);
                minval=MIN(minval,data1Dc_float[vid+page_size*2]);
                estimates[0]+=(double)minval;
                estimates[1]+=(double)maxval;
                estimates[2]+=(double)data1Dc_float[vid];
                estimates[3]+=(double)data1Dc_float[vid+page_size];
                estimates[4]+=(double)data1Dc_float[vid+page_size*2];
                vol_num++;
            }
        }
        estimates[1]/=(vol_num/5);
        estimates[2]/=(vol_num/5);
        estimates[3]/=(vol_num/5);
        estimates[4]/=(vol_num/5);
        estimates[0]/=(vol_num/5);//=MIN(MIN(estimates[2],estimates[3]),estimates[4]);

        qDebug()<<"~~~~~~ "<<estimates[0]<<":"<<estimates[1]<<":"
               <<estimates[2]<<":"<<estimates[3]<<":"<<estimates[4]<<":"<<vol_num/page_size;

        return estimates[0];
    }
};



#endif // NEURONPICKER_FUNC_H
