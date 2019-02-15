#include "extract_fun.h"

extract_fun::extract_fun()
{
    data1Dc_float=0;
    mask1D=0;
    sz_image[0]=sz_image[1]=sz_image[2]=sz_image[3]=0;
    page_size=0;

}

extract_fun::~extract_fun()
{
    if(data1Dc_float!=0)
        memory_free_float1D(data1Dc_float);
    if(mask1D!=0)
        memory_free_uchar1D(mask1D);
}


VISIOCYTELONG extract_fun::extract(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all,vector<VISIOCYTELONG>& z_all,unsigned char * label,
                             VISIOCYTELONG seed_ind, int convolute_iter,int bg_thr,int marker)
{
    //qDebug()<<"In extract now"<<bg_thr;
    //qDebug()<<"sz_img:"<<sz_image[0]<<":"<<sz_image[1]<<":"<<sz_image[2]<<":"<<sz_image[3];
    x_all.clear();
    y_all.clear();
    z_all.clear();

    int neighbor_size=2;
    //1) Collect user specified point info
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, bg_thr);
    if(dir.size()<=0) return 0;
    vector<VISIOCYTELONG> seeds,seeds_next,seeds_pre,seeds_all;
    VISIOCYTELONG x,y,z,pos,center_x,center_y,center_z;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    page_size=sz_image[0]*sz_image[1]*sz_image[2];
    vector<VISIOCYTELONG> coord,neighbor(6,0);
    vector<float> color(sz_image[3]);
    vector<VISIOCYTELONG> new_mass_center;
    memset(mask1D, 0, page_size*sizeof(unsigned char));
    float project;
    int nsum;

    coord=pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];y=coord[1];z=coord[2];

    float total_r_grow=0;
    seeds.push_back(seed_ind);
    seeds_pre.push_back(seed_ind);
    seeds_all.push_back(seed_ind);
    label[seed_ind]=marker;
    x_all.push_back(x);
    y_all.push_back(y);
    z_all.push_back(z);
    mask1D[seed_ind]=1;
    float r_grow=1.;
    center_x=x_all[0];
    center_y=y_all[0];
    center_z=z_all[0];

    //1)Start from a sphere with radius 1, region grow several rounds inside the sphere till
    //all eligible seeds are found and stored in the seed_pre;
    //2)Three stop criteria: a) seeds_pre is empty.(No more seeds can be found) b)Among all the newly grown seeds
    // the ones on the foreground is lower than 20%. A stricter rule trying to seperate cells are closely located.
    // c) the r_grow is over 30;
    //3)if criteria satisfied,the radius of the sphere increased. repeat 1).

    while(seeds_pre.size()>0&&(float)seeds_pre.size()/total_r_grow>=0.3)
    {
        seeds_pre.clear();
        total_r_grow=0;
        //qDebug()<<"New while break.";
        do
        {
            seeds_next.clear();
            VISIOCYTELONG sid=0;

            while(sid<seeds.size())
            {
                coord=pos2xyz(seeds[sid], y_offset, z_offset);
                x=coord[0];y=coord[1];z=coord[2];
                for(VISIOCYTELONG dx=MAX(x-1,0); dx<=MIN(sz_image[0]-1,x+1); dx++){
                    for(VISIOCYTELONG dy=MAX(y-1,0); dy<=MIN(sz_image[1]-1,y+1); dy++){
                        for(VISIOCYTELONG dz=MAX(z-1,0); dz<=MIN(sz_image[2]-1,z+1); dz++){
                            pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                            //point has been visited:mask1D=1; point not visited: mask1D=0;
                            if (mask1D[pos]>0) continue;
                            double tmp=(dx-center_x)*(dx-center_x)+(dy-center_y)*(dy-center_y)
                                 +(dz-center_z)*(dz-center_z);
                            double distance=sqrt(tmp);
                            if (distance>r_grow) continue;
                            total_r_grow++;
                            if (label[pos]>0) continue;
                            for(int cid=0; cid<sz_image[3]; cid++){
                                color[cid]=data1Dc_float[pos+cid*page_size];
                            }
                            project=getProjection(color, dir, convolute_iter);
                            mask1D[pos]=1;
                            if(project<bg_thr) continue;

                            //check whether over 4 background nbs. If >4,set that pixel to bg pixel
                            if(pos-1>=0&&pos+1<page_size&&pos-sz_image[0]>=0&&pos+sz_image[0]<page_size
                                    &&pos-z_offset>=0&&pos+z_offset<page_size)
                            {
                                neighbor[0]=pos-1;
                                neighbor[1]=pos+1;
                                neighbor[2]=pos-sz_image[0];
                                neighbor[3]=pos+sz_image[0];
                                neighbor[4]=pos-z_offset;
                                neighbor[5]=pos+z_offset;
                                nsum=0;
                                for (int i=0;i<neighbor.size();i++)
                                {
                                    if(mask1D[neighbor[i]]>0){
                                        if (label[neighbor[i]]>0) nsum++;
                                    }
                                    else
                                    {
                                        for(int cid=0; cid<sz_image[3]; cid++){
                                            color[cid]=data1Dc_float[neighbor[i]+cid*page_size];
                                        }
                                        project=getProjection(color, dir, convolute_iter);
                                        if(project>=bg_thr) nsum++;
                                    }
                                }
                                if (nsum<=2) continue;
                            }

                            x_all.push_back(dx);
                            y_all.push_back(dy);
                            z_all.push_back(dz);
                            seeds_next.push_back(pos);
                            seeds_pre.push_back(pos);
                            seeds_all.push_back(pos);
                            label[pos]=marker;

                        }
                    }
                }
                sid++;
             }

            seeds=seeds_next;

        }while(!seeds_next.empty());

        seeds=seeds_all; //after this round of r_grow finished, sets all seeds as the seeds for next r_grow

        if (r_grow>20)
            break;
        else    r_grow=r_grow+0.5;

        //Calculate the new mass center
        new_mass_center=get_mass_center(x_all,y_all,z_all);
        center_x=new_mass_center[0];
        center_y=new_mass_center[1];
        center_z=new_mass_center[2];
//        qDebug()<<"total_r_grow:"<<total_r_grow<<"sum_count"<<seeds_pre.size()
//               <<":"<<(float)seeds_pre.size()/total_r_grow;
//        qDebug()<<"r_grow"<<r_grow;
    }
    return x_all.size();
}


//        double tmp_num=(double)x_all.size()*3/(4*pi);
//        double new_r=pow(tmp_num,1./3.);

//        fore_count=0; total_count=0;

//        for(VISIOCYTELONG dx=MAX(x-new_r,0); dx<=MIN(sz_image[0]-1,x+new_r); dx++){
//            for(VISIOCYTELONG dy=MAX(y-new_r,0); dy<=MIN(sz_image[1]-1,y+new_r); dy++){
//                for(VISIOCYTELONG dz=MAX(z-new_r,0); dz<=MIN(sz_image[2]-1,z+new_r); dz++){

//                    pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                    //distance to new center
//                    double tmp=(dx-center_x)*(dx-center_x)+(dy-center_y)*(dy-center_y)
//                            +(dz-center_z)*(dz-center_z);
//                    double distance=pow(tmp,1./2.);
//                    if (distance>new_r) continue;

//                    total_count=total_count+1;
//                    for(int cid=0; cid<sz_image[3]; cid++){
//                     color[cid]=data1Dc_float[pos+cid*page_size];
//                    }
//                    project=getProjection(color, dir, convolute_iter);
//                    if(project<bg_thr)  continue;
//                    fore_count=fore_count+1;
//                }
//            }
//        }
//        //total_count=4.*pi*new_r*new_r*new_r/3.;
//        r_grow=r_grow+1;

//        if (x_all.size()==prev_size) {
//            break;
//        }
//        else prev_size=x_all.size();

//        qDebug()<<"fore_count/total_count:"<<fore_count<<"/"<<total_count;
//        qDebug()<<"new_r"<<new_r<<new_r*new_r*new_r*4/3;
//        qDebug()<<"r_grow:"<<r_grow;

//    }
//   return x_all.size();
//}

//Idea borrowed from neuro colorpicker
float extract_fun::getProjection(vector<float> vec, vector<float> dir, int convolute_iter)
{
    float dp=0, norm=0;
    for(int i=0; i<vec.size(); i++){
        dp+=vec[i]*dir[i];
        norm+=vec[i]*vec[i];
    }
    norm=sqrt(norm);
    float scale;
    float proj=dp;
    if(norm>1e-16)
        scale=dp/norm;
    else
        scale=1;
    for(int i=0; i<convolute_iter; i++){
        proj*=scale;
    }
    return proj;
}

vector<float> extract_fun::getProjectionDirection(VISIOCYTELONG seed_ind, int neighbor_size, int bg_thr)
{
    vector<VISIOCYTELONG> seed_coord = pos2xyz(seed_ind, sz_image[0], sz_image[1]*sz_image[0]);
    int delta=neighbor_size/2;

    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    VISIOCYTELONG pos;
    //qDebug()<<"Before loop "<<seed_coord[0]<<" "<<seed_coord[1]<<" "<<seed_coord[2];
    //qDebug()<<"sz_img size:"<<sz_image[0]<<" "<<sz_image[1]<<" "<<sz_image[2];
    int v_count=0;
    vector<float> dir(sz_image[3],0);
    for(VISIOCYTELONG dx=MAX(seed_coord[0]-delta,0); dx<=MIN(sz_image[0]-1,seed_coord[0]+delta); dx++){
        for(VISIOCYTELONG dy=MAX(seed_coord[1]-delta,0); dy<=MIN(sz_image[1]-1,seed_coord[1]+delta); dy++){
            for(VISIOCYTELONG dz=MAX(seed_coord[2]-delta,0); dz<=MIN(sz_image[2]-1,seed_coord[2]+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                float v=data1Dc_float[pos];
                for(VISIOCYTELONG pid=1; pid<sz_image[3]; pid++)
                    v=MAX(v,data1Dc_float[pos+page_size*pid]);
                    //qDebug()<<"v:"<<v;
                if(v<bg_thr)    continue;

                for(VISIOCYTELONG pid=0; pid<sz_image[3]; pid++)
                    dir[pid]+=data1Dc_float[pos+page_size*pid];
                v_count++;

            }
        }
    }
    if(v_count==0){
        qDebug()<<"Warning:found empty seed area, please decrease the threshold or reselect an area!";
        dir.clear();
        return dir;
    }
    //normalize it
    float norm=0;
    for(VISIOCYTELONG pid=0; pid<sz_image[3]; pid++){
        dir[pid]/=v_count;
        norm+=dir[pid]*dir[pid];
    }
    norm=sqrt(norm);
    for(VISIOCYTELONG pid=0; pid<sz_image[3]; pid++){
        dir[pid]/=norm;
    }
    return dir;
}


vector<VISIOCYTELONG> extract_fun::get_mass_center(vector<VISIOCYTELONG> x_all, vector<VISIOCYTELONG> y_all,
                                             vector<VISIOCYTELONG> z_all)
{
        float total_x,total_y,total_z,sum_v;
        vector<VISIOCYTELONG> center(3,0);
        total_x=total_y=total_z=sum_v=0;

        for (int i=0;i<x_all.size();i++)
        {
            total_x=x_all[i]+total_x;
            total_y=y_all[i]+total_y;
            total_z=z_all[i]+total_z;

        }
            sum_v=x_all.size();
        if (x_all.size()>0) {
            center[0]=total_x/sum_v;
            center[1]=total_y/sum_v;
            center[2]=total_z/sum_v;
        }
        //qDebug()<<"center:"<<center[0]<<","<<center[1]<<","<<center[2]<<","<<center[3];
        return center;
}

unsigned char * memory_allocate_uchar1D(const VISIOCYTELONG i_size)
{
    unsigned char *ptr_result;
    ptr_result=(unsigned char *) calloc(i_size, sizeof(unsigned char));
    return(ptr_result);
}

void memory_free_uchar1D(unsigned char *ptr_input)
{
    free(ptr_input);
}

float * memory_allocate_float1D(const VISIOCYTELONG i_size)
{
    float *ptr_result;
    ptr_result=(float *) calloc(i_size, sizeof(float));
    return(ptr_result);
}

void memory_free_float1D(float *ptr_input)
{
    free(ptr_input);
}

vector<VISIOCYTELONG> landMarkList2poss(LandmarkList LandmarkList_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offest_Z)
{
    vector<VISIOCYTELONG> poss_result;
    VISIOCYTELONG count_landmark=LandmarkList_input.count();
    for (VISIOCYTELONG idx_input=0;idx_input<count_landmark;idx_input++)
    {
        poss_result.push_back(landMark2pos(LandmarkList_input.at(idx_input), _offset_Y, _offest_Z));
    }
    return poss_result;
}

VISIOCYTELONG landMark2pos(LocationSimple Landmark_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offset_Z)
{
    float x=0;
    float y=0;
    float z=0;
    Landmark_input.getCoord(x, y, z);
    x=MAX(x,0);
    y=MAX(y,0);
    z=MAX(z,0);
    return (xyz2pos(x-1, y-1, z-1, _offset_Y, _offset_Z));
}

vector<VISIOCYTELONG> pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z)
{
    vector<VISIOCYTELONG> pos3_result (3, -1);
    pos3_result[2]=floor(_pos_input/(double)_offset_Z);
    pos3_result[1]=floor((_pos_input-pos3_result[2]*_offset_Z)/(double)_offset_Y);
    pos3_result[0]=_pos_input-pos3_result[2]*_offset_Z-pos3_result[1]*_offset_Y;
    return pos3_result;
}

VISIOCYTELONG xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z)
{
    return _z*_offset_Z+_y*_offset_Y+_x;
}

