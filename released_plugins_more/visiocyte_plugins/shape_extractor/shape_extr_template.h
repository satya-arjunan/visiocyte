#ifndef SHAPE_EXTR_TEMPLATE_H
#define SHAPE_EXTR_TEMPLATE_H

#include <visiocyte_interface.h>

template <class T>
void findMaxMinVal(T *data1D, VISIOCYTELONG len, VISIOCYTELONG & max_ind, T & max_val,  VISIOCYTELONG & min_ind, T & min_val)
{
    max_val=data1D[0];
    min_val=data1D[0];
    max_ind=0;
    min_ind=0;
    for(VISIOCYTELONG vid=0; vid<len; vid++){
        if(data1D[vid]<min_val){
            min_val=data1D[vid];
            min_ind=vid;
        }
        if(data1D[vid]>max_val){
            max_val=data1D[vid];
            max_ind=vid;
        }
    }

}

template <class T>
void normalizeEachChannelTo255(T * data1Dc, VISIOCYTELONG sz_img[4])
{
    VISIOCYTELONG page_size = sz_img[0]*sz_img[1]*sz_img[2];
    for(VISIOCYTELONG cid=0; cid<sz_img[3]; cid++){
        VISIOCYTELONG page = page_size*cid;
        T max_val, min_val;
        VISIOCYTELONG max_ind, min_ind;
        findMaxMinVal<T>(data1Dc+page, page_size, max_ind, max_val, min_ind, min_val);

        float scale=(float)(max_val-min_val);
        if(scale>1e-16){
            scale/=255;
            for(VISIOCYTELONG vid=page; vid<page+page_size; vid++){
                data1Dc[vid]-=min_val;
                data1Dc[vid]/=scale;
            }
        }else{
            memset(data1Dc+page,0,page_size*sizeof(T));
        }
    }
}
#endif // SHAPE_EXTR_TEMPLATE_H
