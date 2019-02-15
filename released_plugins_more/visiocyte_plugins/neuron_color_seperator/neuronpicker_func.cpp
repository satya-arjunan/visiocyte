#include "neuronpicker_func.h"

neuronPickerMain2::neuronPickerMain2()
{
    data1Dc_float=0;
    mask1D=0;
    sz_image[0]=sz_image[1]=sz_image[2]=sz_image[3];
    page_size=0;
    innerScale=1;
}

neuronPickerMain2::~neuronPickerMain2()
{
    if(data1Dc_float!=0)
        neuronPickerMain::memory_free_float1D(data1Dc_float);
    if(mask1D!=0)
        neuronPickerMain::memory_free_uchar1D(mask1D);
}

//image1D_out do not need to get initialized, will be re-initialized
VISIOCYTELONG neuronPickerMain2::extractSub_uchar(unsigned char*& image1D_out, VISIOCYTELONG sz_out[4], VISIOCYTELONG& pos_new, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr)
{
    if(image1D_out!=0){
        neuronPickerMain::memory_free_uchar1D(image1D_out);
        image1D_out=0;
    }

    sz_out[0]=sz_out[1]=sz_out[2]=0;
    sz_out[3]=1;

    //get the projection direction from the seed
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, bg_thr, convolute_iter);
    if(dir.size()<=0){
        return 0;
    }

    QString tmp_str="";
    for(VISIOCYTELONG cid=0; cid<sz_image[3]; cid++){
        tmp_str=tmp_str+":"+QString::number(dir[cid]*255);
    }
    qDebug()<<"==========NeuronPicker:\tseed:"<<seed_ind<<"\tcolor:"<<tmp_str;

    //start region grow
    vector<VISIOCYTELONG> seeds;
    vector<VISIOCYTELONG> x_all, y_all, z_all;
    vector<unsigned char> project_all;
    VISIOCYTELONG delta=neighbor_size/2;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    vector<VISIOCYTELONG> coord;
    vector<float> color(sz_image[3]);
    memset(mask1D, 0, sz_image[0]*sz_image[1]*sz_image[2]*sizeof(unsigned char));
    float project;

    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];

    //for subspace calculation
    VISIOCYTELONG x_max, x_min, y_max, y_min, z_max, z_min;
    x_max=x_min=x;
    y_max=y_min=y;
    z_max=z_min=z;

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                mask1D[pos]=1;

                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                }
                project=getProjection(color, dir, convolute_iter);
                if(project<bg_thr)
                    continue;

                x_all.push_back(dx);
                y_all.push_back(dy);
                z_all.push_back(dz);
                project_all.push_back((unsigned char) project);
                seeds.push_back(pos);

                //calculate sub-space
                x_max=MAX(x_max,dx);
                x_min=MIN(x_min,dx);
                y_max=MAX(y_max,dy);
                y_min=MIN(y_min,dy);
                z_max=MAX(z_max,dz);
                z_min=MIN(z_min,dz);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=neuronPickerMain::pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(mask1D[pos]>0) continue;
                    mask1D[pos]=1;

                    for(int cid=0; cid<sz_image[3]; cid++){
                        color[cid]=data1Dc_float[pos+cid*page_size];
                    }
                    project=getProjection(color, dir, convolute_iter);
                    if(project<bg_thr) continue;

                    x_all.push_back(dx);
                    y_all.push_back(dy);
                    z_all.push_back(dz);
                    project_all.push_back((unsigned char) project);
                    seeds.push_back(pos);

                    //calculate sub-space
                    x_max=MAX(x_max,dx);
                    x_min=MIN(x_min,dx);
                    y_max=MAX(y_max,dy);
                    y_min=MIN(y_min,dy);
                    z_max=MAX(z_max,dz);
                    z_min=MIN(z_min,dz);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    //generate output matrix in sub-space
    sz_out[0]=x_max-x_min+2;
    sz_out[1]=y_max-y_min+2;
    sz_out[2]=z_max-z_min+2;
    sz_out[3]=1;

    image1D_out=neuronPickerMain::memory_allocate_uchar1D(sz_out[0]*sz_out[1]*sz_out[2]);
    memset(image1D_out, 0, sz_out[0]*sz_out[1]*sz_out[2]*sizeof(unsigned char));
    for(int i=0; i<project_all.size(); i++){
        //sub-space
        x=x_all[i]-x_min+1;
        y=y_all[i]-y_min+1;
        z=z_all[i]-z_min+1;

        image1D_out[neuronPickerMain::xyz2pos(x,y,z,sz_out[0],sz_out[1]*sz_out[0])]=project_all[i];
    }
    //markers in subspace
    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    pos_new=neuronPickerMain::xyz2pos(coord[0]-x_min+1,coord[1]-y_min+1,coord[2]-z_min+1,sz_out[0],sz_out[0]*sz_out[1]);

    return seeds.size();
}

//image1D_out do not need to get initialized, will be re-initialized
VISIOCYTELONG neuronPickerMain2::extract_uchar(unsigned char*& image1D_out, VISIOCYTELONG sz_out[4], VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr)
{
    if(image1D_out!=0){
        neuronPickerMain::memory_free_uchar1D(image1D_out);
        image1D_out=0;
    }

    sz_out[0]=sz_out[1]=sz_out[2]=0;
    sz_out[3]=1;

    //get the projection direction from the seed
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, bg_thr, convolute_iter);
    if(dir.size()<=0){
        return 0;
    }

    QString tmp_str="";
    for(VISIOCYTELONG cid=0; cid<sz_image[3]; cid++){
        tmp_str=tmp_str+":"+QString::number(dir[cid]*255);
    }
    qDebug()<<"==========NeuronPicker:\tseed:"<<seed_ind<<"\tcolor:"<<tmp_str;

    //start region grow
    vector<VISIOCYTELONG> seeds;
    vector<VISIOCYTELONG> x_all, y_all, z_all;
    vector<float> project_all;
    VISIOCYTELONG delta=neighbor_size/2;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    vector<VISIOCYTELONG> coord;
    vector<float> color(sz_image[3]);
    memset(mask1D, 0, sz_image[0]*sz_image[1]*sz_image[2]*sizeof(unsigned char));
    float project;

    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                mask1D[pos]=1;

                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                }
                project=getProjection(color, dir, convolute_iter);
                if(project<bg_thr)
                    continue;

                x_all.push_back(dx);
                y_all.push_back(dy);
                z_all.push_back(dz);
                project_all.push_back(project);
                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=neuronPickerMain::pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(mask1D[pos]>0) continue;
                    mask1D[pos]=1;

                    for(int cid=0; cid<sz_image[3]; cid++){
                        color[cid]=data1Dc_float[pos+cid*page_size];
                    }
                    project=getProjection(color, dir, convolute_iter);
                    if(project<bg_thr) continue;

                    x_all.push_back(dx);
                    y_all.push_back(dy);
                    z_all.push_back(dz);
                    project_all.push_back((unsigned char) project);
                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";
    //generate output matrix in original space
    sz_out[0]=sz_image[0];
    sz_out[1]=sz_image[1];
    sz_out[2]=sz_image[2];
    sz_out[3]=1;

    image1D_out=neuronPickerMain::memory_allocate_uchar1D(sz_out[0]*sz_out[1]*sz_out[2]);
    memset(image1D_out, 0, sz_out[0]*sz_out[1]*sz_out[2]*sizeof(unsigned char));
    for(int i=0; i<project_all.size(); i++){
        //original space
        x=x_all[i];
        y=y_all[i];
        z=z_all[i];

        image1D_out[neuronPickerMain::xyz2pos(x,y,z,sz_out[0],sz_out[1]*sz_out[0])]=(unsigned char)MIN(project_all[i]*innerScale,255);
    }

    return seeds.size();
}


//image1D_out do not need to get initialized, will be re-initialized
VISIOCYTELONG neuronPickerMain2::extractMargin_uchar(unsigned char*& image1D_out, VISIOCYTELONG sz_out[4], VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr, int margin_size)
{
    if(image1D_out!=0){
        neuronPickerMain::memory_free_uchar1D(image1D_out);
        image1D_out=0;
    }

    sz_out[0]=sz_out[1]=sz_out[2]=0;
    sz_out[3]=1;

    //get the projection direction from the seed
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, bg_thr, convolute_iter);
    if(dir.size()<=0){
        return 0;
    }

    QString tmp_str="";
    for(VISIOCYTELONG cid=0; cid<sz_image[3]; cid++){
        tmp_str=tmp_str+":"+QString::number(dir[cid]*255);
    }
    qDebug()<<"==========NeuronPicker:\tseed:"<<seed_ind<<"\tcolor:"<<tmp_str;

    //start region grow
    vector<VISIOCYTELONG> seeds;
    vector<VISIOCYTELONG> x_all, y_all, z_all;
    vector<unsigned char> project_all;
    VISIOCYTELONG delta=neighbor_size/2;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    vector<VISIOCYTELONG> coord;
    vector<float> color(sz_image[3]);
    memset(mask1D, 0, sz_image[0]*sz_image[1]*sz_image[2]*sizeof(unsigned char));
    float project;

    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                mask1D[pos]=1;

                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                }
                project=getProjection(color, dir, convolute_iter);
                if(project<bg_thr)
                    continue;

                x_all.push_back(dx);
                y_all.push_back(dy);
                z_all.push_back(dz);
                project_all.push_back((unsigned char) project);
                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=neuronPickerMain::pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(mask1D[pos]>0) continue;
                    mask1D[pos]=1;

                    for(int cid=0; cid<sz_image[3]; cid++){
                        color[cid]=data1Dc_float[pos+cid*page_size];
                    }
                    project=getProjection(color, dir, convolute_iter);
                    if(project<bg_thr) continue;

                    x_all.push_back(dx);
                    y_all.push_back(dy);
                    z_all.push_back(dz);
                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";
    //generate output matrix in original space
    sz_out[0]=sz_image[0];
    sz_out[1]=sz_image[1];
    sz_out[2]=sz_image[2];
    sz_out[3]=1;

    for(int i=0; i<x_all.size(); i++){
        x=x_all[i];
        y=y_all[i];
        z=z_all[i];
        for(VISIOCYTELONG dx=MAX(x-margin_size,0); dx<=MIN(sz_image[0]-1,x+margin_size); dx++){
            for(VISIOCYTELONG dy=MAX(y-margin_size,0); dy<=MIN(sz_image[1]-1,y+margin_size); dy++){
                for(VISIOCYTELONG dz=MAX(z-margin_size,0); dz<=MIN(sz_image[2]-1,z+margin_size); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    mask1D[pos]=1;
                }
            }
        }
    }

    image1D_out=neuronPickerMain::memory_allocate_uchar1D(sz_out[0]*sz_out[1]*sz_out[2]);
    memset(image1D_out, 0, sz_out[0]*sz_out[1]*sz_out[2]*sizeof(unsigned char));
    for(int i=0; i<page_size; i++){
        if(mask1D[i]<1)
            continue;
        for(int cid=0; cid<sz_image[3]; cid++){
            color[cid]=data1Dc_float[i+cid*page_size];
        }
        project=getProjection(color, dir, convolute_iter);

        image1D_out[i]=(unsigned char)MIN(project*innerScale,255);
    }

    return seeds.size();
}

VISIOCYTELONG neuronPickerMain2::extract(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all, vector<VISIOCYTELONG>& z_all, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr)
{
    x_all.clear();
    y_all.clear();
    z_all.clear();

    //get the projection direction from the seed
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, bg_thr, convolute_iter);
    if(dir.size()<=0){
        return 0;
    }

    //start region grow
    vector<VISIOCYTELONG> seeds;
    VISIOCYTELONG delta=neighbor_size/2;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    vector<VISIOCYTELONG> coord;
    vector<float> color(sz_image[3]);
    memset(mask1D, 0, sz_image[0]*sz_image[1]*sz_image[2]*sizeof(unsigned char));
    float project;

    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];

    VISIOCYTELONG x_max, x_min, y_max, y_min, z_max, z_min;
    x_max=x_min=x;
    y_max=y_min=y;
    z_max=z_min=z;

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                mask1D[pos]=1;

                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                }
                project=getProjection(color, dir, convolute_iter);
                if(project<bg_thr)
                    continue;

                x_all.push_back(dx);
                y_all.push_back(dy);
                z_all.push_back(dz);

                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=neuronPickerMain::pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(mask1D[pos]>0) continue;
                    mask1D[pos]=1;

                    for(int cid=0; cid<sz_image[3]; cid++){
                        color[cid]=data1Dc_float[pos+cid*page_size];
                    }
                    project=getProjection(color, dir, convolute_iter);
                    if(project<bg_thr) continue;

                    x_all.push_back(dx);
                    y_all.push_back(dy);
                    z_all.push_back(dz);

                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    return seeds.size();
}

VISIOCYTELONG neuronPickerMain2::extract_eng(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all, vector<VISIOCYTELONG>& z_all, float* energy, float* energy_seed, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, double thr_bg[5])
{
    x_all.clear();
    y_all.clear();
    z_all.clear();

    //init parameters
    vector<VISIOCYTELONG> seeds;
    VISIOCYTELONG delta=neighbor_size/2;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    vector<VISIOCYTELONG> coord;
    vector<float> color(sz_image[3]);
    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];

    //mask the init seed region
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                energy_seed[pos]=0;
            }
        }
    }

    //get the projection direction from the seed
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, (int)(thr_bg[1]), convolute_iter);
    if(dir.size()<=0){
        return 0;
    }

    //start region grow
    memset(mask1D, 0, sz_image[0]*sz_image[1]*sz_image[2]*sizeof(unsigned char));
    float project;

    VISIOCYTELONG x_max, x_min, y_max, y_min, z_max, z_min;
    x_max=x_min=x;
    y_max=y_min=y;
    z_max=z_min=z;

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                mask1D[pos]=1;
                energy_seed[pos]=0;

                if(data1Dc_float[pos]<thr_bg[2] &&
                        data1Dc_float[pos+1*page_size]<thr_bg[3] &&
                        data1Dc_float[pos+2*page_size]<thr_bg[4])
                    continue;

                float v=0;
                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                    v=MAX(v,color[cid]);
                }
                project=getProjection(color, dir, convolute_iter);

                if(project*(energy[pos]/v)<thr_bg[0])
                    continue;

                x_all.push_back(dx);
                y_all.push_back(dy);
                z_all.push_back(dz);

                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=neuronPickerMain::pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(mask1D[pos]>0) continue;

                    mask1D[pos]=1;

                    if(data1Dc_float[pos]<thr_bg[2] &&
                            data1Dc_float[pos+1*page_size]<thr_bg[3] &&
                            data1Dc_float[pos+2*page_size]<thr_bg[4])
                        continue;

                    float v=0;
                    for(int cid=0; cid<sz_image[3]; cid++){
                        color[cid]=data1Dc_float[pos+cid*page_size];
                        v=MAX(v,color[cid]);
                    }
                    project=getProjection(color, dir, convolute_iter);
                    energy_seed[pos]-=project;

                    if(project*(energy[pos]/v)<thr_bg[0]) continue;

                    x_all.push_back(dx);
                    y_all.push_back(dy);
                    z_all.push_back(dz);

                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    return seeds.size();
}

VISIOCYTELONG neuronPickerMain2::extractMore(vector<VISIOCYTELONG>& x_all, vector<VISIOCYTELONG>& y_all, vector<VISIOCYTELONG>& z_all, VISIOCYTELONG seed_ind, int convolute_iter, int neighbor_size, int bg_thr)
{
    x_all.clear();
    y_all.clear();
    z_all.clear();

    //get the projection direction from the seed
    vector<float> dir = getProjectionDirection(seed_ind, neighbor_size, bg_thr, convolute_iter);
    if(dir.size()<=0){
        return 0;
    }

    //start region grow
    vector<VISIOCYTELONG> seeds;
    VISIOCYTELONG delta=neighbor_size/2;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    vector<VISIOCYTELONG> coord;
    vector<float> color(sz_image[3]);
    memset(mask1D, 0, sz_image[0]*sz_image[1]*sz_image[2]*sizeof(unsigned char));
    float project;

    coord=neuronPickerMain::pos2xyz(seed_ind, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];

    VISIOCYTELONG x_max, x_min, y_max, y_min, z_max, z_min;
    x_max=x_min=x;
    y_max=y_min=y;
    z_max=z_min=z;

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                mask1D[pos]=1;
                x_all.push_back(dx);
                y_all.push_back(dy);
                z_all.push_back(dz);

                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                }
                project=getProjection(color, dir, convolute_iter);
                if(project<bg_thr)
                    continue;

                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=neuronPickerMain::pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(mask1D[pos]>0) continue;
                    mask1D[pos]=1;
                    x_all.push_back(dx);
                    y_all.push_back(dy);
                    z_all.push_back(dz);

                    for(int cid=0; cid<sz_image[3]; cid++){
                        color[cid]=data1Dc_float[pos+cid*page_size];
                    }
                    project=getProjection(color, dir, convolute_iter);
                    if(project<bg_thr) continue;

                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    return seeds.size();
}

VISIOCYTELONG neuronPickerMain2::autoSeeds(vector<VISIOCYTELONG>& seeds, int cubSize, int conviter, int fgthr, int bgthr, int sizethr)
{
    seeds.clear();
    multimap<VISIOCYTELONG, VISIOCYTELONG, std::greater<VISIOCYTELONG> > seedMap; //first:

    float v=0;
    unsigned char * mask1D_v=neuronPickerMain::memory_allocate_uchar1D(page_size);
    for(int i=0; i<page_size; i++){
        v=data1Dc_float[i];
        for(int cid=1; cid<sz_image[3]; cid++){
            v=MAX(v,data1Dc_float[i+page_size*cid]);
        }
        mask1D_v[i]=(unsigned char)v;
    }
    unsigned char max_val, min_val;
    VISIOCYTELONG max_ind, min_ind;
    vector<VISIOCYTELONG> x_all, y_all, z_all;
    VISIOCYTELONG rsize;
    findMaxMinVal<unsigned char>(mask1D_v, page_size, max_ind, max_val, min_ind, min_val);
    while((int)max_val>fgthr){
        rsize=extractMore(x_all, y_all, z_all, max_ind, conviter, cubSize, bgthr);
        for(int i=0; i<x_all.size(); i++){
            mask1D_v[x_all[i]+y_all[i]*sz_image[0]+z_all[i]*sz_image[0]*sz_image[1]]=0;
        }
        if(rsize>sizethr)
            seedMap.insert(std::pair<VISIOCYTELONG, VISIOCYTELONG>(rsize, max_ind) );

//        qDebug()<<"NeuronPicker: "<<max_val<<":"<<max_ind<<":"<<x_all.size();

        findMaxMinVal<unsigned char>(mask1D_v, page_size, max_ind, max_val, min_ind, min_val);
    }

    for(multimap<VISIOCYTELONG, VISIOCYTELONG, std::greater<VISIOCYTELONG> >::iterator iter=seedMap.begin();
        iter!=seedMap.end(); iter++){
        seeds.push_back(iter->second);
    }
    return seeds.size();
}

VISIOCYTELONG neuronPickerMain2::autoAll(QString fname_outbase, VISIOCYTEPluginCallback2 * callback , int cubSize, int conviter, int fgthr, double thr_bg[5], int sizethr, int margin_size, float sparsthr, float touchthr)
{
    float v=0;
    float * mask1D_v=neuronPickerMain::memory_allocate_float1D(page_size);
    float * mask1D_eng=neuronPickerMain::memory_allocate_float1D(page_size);
    unsigned char * data1D_out=neuronPickerMain::memory_allocate_uchar1D(page_size);
    for(int i=0; i<page_size; i++){
        v=data1Dc_float[i];
        for(int cid=1; cid<sz_image[3]; cid++){
            v=MAX(v,data1Dc_float[i+page_size*cid]);
        }
        mask1D_eng[i]=v;
    }
    VISIOCYTELONG x, y, z;
    int seedwin=2;
    for(x=0; x<sz_image[0]; x++){
        for(y=0; y<sz_image[1]; y++){
            for(z=0; z<sz_image[2]; z++){
                VISIOCYTELONG pos=neuronPickerMain::xyz2pos(x,y,z,sz_image[0],sz_image[0]*sz_image[1]);
                v=0;
                int accum=0;
                for(VISIOCYTELONG dx=MAX(x-seedwin,0); dx<MIN(x+seedwin+1,sz_image[0]); dx++){
                    for(VISIOCYTELONG dy=MAX(y-seedwin,0); dy<MIN(y+seedwin+1,sz_image[1]); dy++){
                        for(VISIOCYTELONG dz=MAX(z-seedwin,0); dz<MIN(z+seedwin+1,sz_image[2]); dz++){
                            v+=mask1D_eng[neuronPickerMain::xyz2pos(dx,dy,dz,sz_image[0],sz_image[0]*sz_image[1])];
                            accum++;
                        }
                    }
                }
                mask1D_v[pos]=MIN(v,mask1D_eng[pos]);
            }
        }
    }
    float max_val, min_val;
    VISIOCYTELONG max_ind, min_ind;
    vector<VISIOCYTELONG> x_all, y_all, z_all;
    VISIOCYTELONG rsize;
    int delta=cubSize/2;
    vector<float> color(sz_image[3]);
    float project;
    int neuronNum = 0;
    VISIOCYTELONG sz_out[4];
    sz_out[0]=sz_image[0];
    sz_out[1]=sz_image[1];
    sz_out[2]=sz_image[2];
    sz_out[3]=1;
    bool eligible;
    findMaxMinVal<float>(mask1D_v, page_size, max_ind, max_val, min_ind, min_val);
    while((int)max_val>fgthr){
        qDebug()<<"==========NeuronPicker: seed intensity "<<max_val;
//        rsize=extract(x_all, y_all, z_all, max_ind, conviter, cubSize, bgthr);
        rsize=extract_eng(x_all, y_all, z_all, mask1D_eng, mask1D_v, max_ind, conviter, cubSize, thr_bg);
        if(rsize>sizethr)
            eligible=checkEligibility(x_all, y_all, z_all, sparsthr, touchthr, cubSize);
        else
            eligible=false;
        if(eligible){ //save if it is an eligible neuron
            //set output
            memset(mask1D,0,page_size*sizeof(unsigned char));
            memset(data1D_out,0,page_size*sizeof(unsigned char));
            vector<float> dir = getProjectionDirection(max_ind, cubSize, (int)(thr_bg[1]), conviter);
            //find the region
            for(int i=0; i<x_all.size(); i++){
                x=x_all[i];
                y=y_all[i];
                z=z_all[i];
                for(VISIOCYTELONG dx=MAX(x-margin_size,0); dx<=MIN(sz_image[0]-1,x+margin_size); dx++){
                    for(VISIOCYTELONG dy=MAX(y-margin_size,0); dy<=MIN(sz_image[1]-1,y+margin_size); dy++){
                        for(VISIOCYTELONG dz=MAX(z-margin_size,0); dz<=MIN(sz_image[2]-1,z+margin_size); dz++){
                            mask1D[dx+dy*sz_image[0]+dz*sz_image[0]*sz_image[1]]=1;
                        }
                    }
                }
            }
            for(int i=0; i<x_all.size(); i++){
                x=x_all[i];
                y=y_all[i];
                z=z_all[i];
                for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
                    for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                        for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                            mask1D[dx+dy*sz_image[0]+dz*sz_image[0]*sz_image[1]]=2;
                        }
                    }
                }
            }
            //calculate projection
            for(int i=0; i<page_size; i++){
                if(mask1D[i]<1)
                    continue;
                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[i+cid*page_size];
                }
                project=getProjection(color, dir, conviter);
                if(mask1D[i]>1)
                    mask1D_eng[i]-=project;

                data1D_out[i]=(unsigned char)MIN(project*innerScale,255);
            }
            //save file
            qDebug()<<"NeuronPicker: outputing neuron #"<<neuronNum<<", seed intensity: "<<max_val<<", voxels:"<<x_all.size();
            QString fname_output=fname_outbase + "_" + QString::number(neuronNum) + ".visiocytepbd";
            if(!simple_saveimage_wrapper(*callback, qPrintable(fname_output),data1D_out,sz_out,1)){
                visiocyte_msg("Error: failed to save file to " + fname_output);
                continue;
            }else{
                qDebug()<<"NeuronPicker: "<<fname_output;
            }
            fname_output=fname_outbase + "_" + QString::number(neuronNum) + ".marker";
            neuronPickerMain::saveSingleMarker(max_ind,fname_output,sz_out);

            //record
            neuronNum++;
        }

        findMaxMinVal<float>(mask1D_v, page_size, max_ind, max_val, min_ind, min_val);
    }

    //save configuration
    QString fname_conf=fname_outbase+".conf";
    QFile file(fname_conf);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"cannot open "<<fname_conf<<" for write";
        return neuronNum;
    }
    QTextStream myfile(&file);
    myfile<<"cubSize: "<<cubSize<<endl;
    myfile<<"conviter: "<<conviter<<endl;
    myfile<<"fgthr: "<<fgthr<<endl;
    myfile<<"bgthr_v: "<<thr_bg[0]<<endl;
    myfile<<"bgthr_seed: "<<(int)thr_bg[1]<<endl;
    myfile<<"bgthr_r: "<<thr_bg[2]<<endl;
    myfile<<"bgthr_g: "<<thr_bg[3]<<endl;
    myfile<<"bgthr_b: "<<thr_bg[4]<<endl;
    myfile<<"sizethr: "<<sizethr<<endl;
    myfile<<"margin_size: "<<margin_size<<endl;
    myfile<<"sparsthr: "<<sparsthr<<endl;
    myfile<<"touchthr: "<<touchthr<<endl;
    myfile<<"seedwin: "<<seedwin<<endl;
    file.close();

    return neuronNum;
}

bool neuronPickerMain2::checkEligibility(vector<VISIOCYTELONG> x_all, vector<VISIOCYTELONG> y_all, vector<VISIOCYTELONG> z_all, float thr_sparse, float thr_touching, int cubeSize)
{
    memset(mask1D, 0, page_size*sizeof(unsigned char));
    VISIOCYTELONG nbox[6];
    nbox[0]=sz_image[0];
    nbox[1]=sz_image[1];
    nbox[2]=sz_image[2];
    nbox[3]=0;
    nbox[4]=0;
    nbox[5]=0;

    for(int i=0; i<x_all.size(); i++){
        mask1D[x_all[i]+y_all[i]*sz_image[0]+z_all[i]*sz_image[0]*sz_image[1]]=1;
        nbox[0]=MIN(nbox[0],x_all[i]);
        nbox[1]=MIN(nbox[1],y_all[i]);
        nbox[2]=MIN(nbox[2],z_all[i]);
        nbox[3]=MAX(nbox[3],x_all[i]);
        nbox[4]=MAX(nbox[4],y_all[i]);
        nbox[5]=MAX(nbox[5],z_all[i]);
    }

    //check boundary eligibility
    qDebug()<<"=======NeuronPicker: Check edge touching: ";
    if((nbox[0]<=1) && (nbox[3]<sz_image[0]*thr_touching)) return false;
    if((nbox[1]<=1) && (nbox[4]<sz_image[1]*thr_touching)) return false;
    if((nbox[2]<=1) && (nbox[5]<sz_image[2]*thr_touching)) return false;
    if((nbox[3]>sz_image[0]-2) && (sz_image[0]-nbox[0]<sz_image[0]*thr_touching)) return false;
    if((nbox[4]>sz_image[1]-2) && (sz_image[1]-nbox[1]<sz_image[1]*thr_touching)) return false;
    if((nbox[5]>sz_image[2]-2) && (sz_image[2]-nbox[2]<sz_image[2]*thr_touching)) return false;

    //calculate sparsness
    VISIOCYTELONG x,y,z;
    int delta=cubeSize/2;
    double sparsity=0;
    double neighborSize=(delta*2+1)*(delta*2+1)*(delta*2+1);
    double fgcount=0;
    for(int i=0; i<x_all.size(); i++){
        x=x_all[i];
        y=y_all[i];
        z=z_all[i];
        fgcount=0;
        //mask all regions founded
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_image[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_image[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_image[2]-1,z+delta); dz++){
                    if(mask1D[dx+dy*sz_image[0]+dz*sz_image[0]*sz_image[1]]==1)
                        fgcount++;
                }
            }
        }
        sparsity+=fgcount/neighborSize;
    }
    sparsity/=(double)x_all.size();
    qDebug()<<"=======NeuronPicker: Sparsity: "<<sparsity;
    if(sparsity>thr_sparse)
        return false;
    return true;
}

float neuronPickerMain2::getProjection(vector<float> vec, vector<float> dir, int convolute_iter)
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

vector<float> neuronPickerMain2::getProjectionDirection(VISIOCYTELONG seed_ind, int neighbor_size, int bg_thr, int convolute_iter)
{
    vector<VISIOCYTELONG> seed_coord = neuronPickerMain::pos2xyz(seed_ind, sz_image[0], sz_image[1]*sz_image[0]);
    int delta=neighbor_size/2;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    VISIOCYTELONG pos;

    int v_count=0;
    vector<float> dir(sz_image[3],0);
    vector<float> color(sz_image[3]);
    vector<float> color0(sz_image[3]);
    float norm=0;
    for(int cid=0; cid<sz_image[3]; cid++){
        color0[cid]=data1Dc_float[seed_ind+cid*page_size];
        norm+=color0[cid]*color0[cid];
    }
    norm=sqrt(norm);
    for(int cid=0; cid<sz_image[3]; cid++){
        color0[cid]/=norm;
    }
    for(VISIOCYTELONG dx=MAX(seed_coord[0]-delta,0); dx<=MIN(sz_image[0]-1,seed_coord[0]+delta); dx++){
        for(VISIOCYTELONG dy=MAX(seed_coord[1]-delta,0); dy<=MIN(sz_image[1]-1,seed_coord[1]+delta); dy++){
            for(VISIOCYTELONG dz=MAX(seed_coord[2]-delta,0); dz<=MIN(sz_image[2]-1,seed_coord[2]+delta); dz++){
                pos=neuronPickerMain::xyz2pos(dx,dy,dz,y_offset,z_offset);

                for(int cid=0; cid<sz_image[3]; cid++){
                    color[cid]=data1Dc_float[pos+cid*page_size];
                }

                float v=getProjection(color, color0, convolute_iter);

                if(v<bg_thr)    continue;

                for(VISIOCYTELONG cid=0; cid<sz_image[3]; cid++)
                    dir[cid]+=data1Dc_float[pos+page_size*cid]*v/255;
                v_count++;
            }
        }
    }
    if(v_count==0){
        qDebug()<<"Warning: NeuronPicker found empty seed area, please decrease the threshold or reselect an area!";
        dir.clear();
        return dir;
    }
    //normalize it
    norm=0;
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


unsigned char * neuronPickerMain::memory_allocate_uchar1D(const VISIOCYTELONG i_size)
{
    unsigned char *ptr_result;
    ptr_result=(unsigned char *) calloc(i_size, sizeof(unsigned char));
    return(ptr_result);
}
void neuronPickerMain::memory_free_uchar1D(unsigned char *ptr_input)
{
    free(ptr_input);
}
int * neuronPickerMain::memory_allocate_int1D(const VISIOCYTELONG i_size)
{
    int *ptr_result;
    ptr_result=(int *) calloc(i_size, sizeof(int));
    return(ptr_result);
}
void neuronPickerMain::memory_free_int1D(int *ptr_input)
{
    free(ptr_input);
}
float * neuronPickerMain::memory_allocate_float1D(const VISIOCYTELONG i_size)
{
    float *ptr_result;
    ptr_result=(float *) calloc(i_size, sizeof(float));
    return(ptr_result);
}
void neuronPickerMain::memory_free_float1D(float *ptr_input)
{
    free(ptr_input);
}
vector<VISIOCYTELONG> neuronPickerMain::landMarkList2poss(LandmarkList LandmarkList_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offest_Z)
{
    vector<VISIOCYTELONG> poss_result;
    VISIOCYTELONG count_landmark=LandmarkList_input.count();
    for (VISIOCYTELONG idx_input=0;idx_input<count_landmark;idx_input++)
    {
        poss_result.push_back(landMark2pos(LandmarkList_input.at(idx_input), _offset_Y, _offest_Z));
    }
    return poss_result;
}
VISIOCYTELONG neuronPickerMain::landMark2pos(LocationSimple Landmark_input, VISIOCYTELONG _offset_Y, VISIOCYTELONG _offset_Z)
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
vector<VISIOCYTELONG> neuronPickerMain::pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z)
{
    vector<VISIOCYTELONG> pos3_result (3, -1);
    pos3_result[2]=floor(_pos_input/(double)_offset_Z);
    pos3_result[1]=floor((_pos_input-pos3_result[2]*_offset_Z)/(double)_offset_Y);
    pos3_result[0]=_pos_input-pos3_result[2]*_offset_Z-pos3_result[1]*_offset_Y;
    return pos3_result;
}
VISIOCYTELONG neuronPickerMain::xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z)
{
    return _z*_offset_Z+_y*_offset_Y+_x;
}

void neuronPickerMain::initChannels_rgb(unsigned char *image1Dc, int *image1D_h,unsigned char *image1D_v, unsigned char *image1D_s, VISIOCYTELONG sz_img[4], const int bg_thr)
{
    memset(image1D_h, -1, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(int));
    memset(image1D_s, 0, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(unsigned char));
    VISIOCYTELONG page=sz_img[0]*sz_img[1]*sz_img[2];

    //rgb 2 hsv, obtain foreground and histogram
    for(VISIOCYTELONG idx=0; idx<page; idx++){
        unsigned char V=MAX(image1Dc[idx],image1Dc[idx+page]);
        V=MAX(V,image1Dc[idx+page*2]);
        image1D_v[idx]=V;
        //filter by value
        if(V<(unsigned char)bg_thr){
            continue;
        }
        //get hue
        image1D_h[idx] = rgb2hue(image1Dc[idx], image1Dc[idx+page], image1Dc[idx+page*2]);
        //rgb2hsv(image1Dc[idx], image1Dc[idx+page], image1Dc[idx+page*2], image1D_h[idx], image1D_s[idx], image1D_v[idx]);
    }
}

void neuronPickerMain::saveSingleMarker(VISIOCYTELONG pos_landmark, QString fname, VISIOCYTELONG sz_img[4])
{
    vector<VISIOCYTELONG> coord;
    coord=pos2xyz(pos_landmark, sz_img[0], sz_img[0]*(sz_img[1]));
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    myfile<<coord[0]<<", "<<coord[1]<<", "<<coord[2]<<",0, 1, , , 255, 0"<<endl;
    file.close();
}

int neuronPickerMain::huedis(int a, int b)
{
    int dis=MAX(a,b)-MIN(a,b);
    if(dis>180){
        dis=360-180;
    }
    return dis;
}

unsigned char neuronPickerMain::saturationdis(unsigned char a, unsigned char b)
{
    return MAX(a,b)-MIN(a,b);
}

void neuronPickerMain::findMaxVal(unsigned char *image1D_v, VISIOCYTELONG len, VISIOCYTELONG & maxIdx, unsigned char & maxVal)
{
    maxVal=0;
    maxIdx=0;
    for(VISIOCYTELONG i=0; i<len; i++){
        if(image1D_v[i]>maxVal){
            maxIdx=i;
            maxVal=image1D_v[i];
        }
    }
}

void neuronPickerMain::autoSeeds(int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, vector<VISIOCYTELONG>& seeds, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4], int fgthr, int sizethr)
{
    seeds.clear();
    multimap<VISIOCYTELONG, VISIOCYTELONG, std::greater<VISIOCYTELONG> > seedMap; //first:

    //mask will be used to track the progress (volexes > 0 will be processed)
    unsigned char* image1D_mask=memory_allocate_uchar1D(sz_img[0]*sz_img[1]*sz_img[2]);
    memset(image1D_mask, 0, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(unsigned char));
//    int* image1D_hbk=memory_allocate_int1D(sz_img[0]*sz_img[1]*sz_img[2]);

    for(VISIOCYTELONG i=0; i<sz_img[0]*sz_img[1]*sz_img[2]; i++){
        if(image1D_h[i]>=0){
            image1D_mask[i]=image1D_v[i];
        }
//        image1D_hbk[i]=image1D_h[i];
    }

    //cur will be used to obtain current growed region
    unsigned char* image1D_cur=memory_allocate_uchar1D(sz_img[0]*sz_img[1]*sz_img[2]);

    VISIOCYTELONG maxIdx;
    unsigned char maxVal;
    findMaxVal(image1D_mask, sz_img[0]*sz_img[1]*sz_img[2], maxIdx, maxVal);
    while(maxVal>fgthr){
        //region grow
        memset(image1D_cur, 0, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(unsigned char));
        VISIOCYTELONG curSize = extract(image1D_h, image1D_mask, image1D_s, image1D_cur, maxIdx, cubSize, colorSpan, sz_img);
        //get the size of the region
//        image1D_hbk[maxIdx]=-1;
        image1D_mask[maxIdx]=0;
        for(VISIOCYTELONG i=0; i<sz_img[0]*sz_img[1]*sz_img[2]; i++){
            if(image1D_cur[i]>0){
                image1D_mask[i]=0;
//                image1D_hbk[i]=-1;
            }
        }
        if(curSize>sizethr)
            seedMap.insert(std::pair<VISIOCYTELONG, VISIOCYTELONG>(curSize, maxIdx) );

        findMaxVal(image1D_mask, sz_img[0]*sz_img[1]*sz_img[2], maxIdx, maxVal);
        qDebug()<<"======NeuronPicker: max val:"<<maxVal<<"\tpos:"<<maxIdx;
    }

    for(multimap<VISIOCYTELONG, VISIOCYTELONG, std::greater<VISIOCYTELONG> >::iterator iter=seedMap.begin();
        iter!=seedMap.end(); iter++){
        seeds.push_back(iter->second);
    }

    qDebug()<<"NeuronPicker: identified "<<seeds.size()<<" regions";

    memory_free_uchar1D(image1D_mask);
    memory_free_uchar1D(image1D_cur);
//    memory_free_int1D(image1D_hbk);
}

VISIOCYTELONG neuronPickerMain::extract(int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, unsigned char *image1D_out, VISIOCYTELONG _pos_input, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4])
{
    vector<VISIOCYTELONG> seeds;
    VISIOCYTELONG delta=cubSize/2;
    VISIOCYTELONG h_mean=0, v_count=0, s_mean=0;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_img[0];
    VISIOCYTELONG z_offset=sz_img[0]*sz_img[1];
    vector<VISIOCYTELONG> coord;
    coord=pos2xyz(_pos_input, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];
    qDebug()<<"==========NeuronPicker: seed:"<<x<<":"<<y<<":"<<z<<":"<<delta;
    //find the average hue in seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                qDebug()<<"==========NeuronPicker: "<<dx<<":"<<dy<<":"<<dz<<":"<<image1D_h[pos];
                if(image1D_h[pos]<0)    continue;
                h_mean+=image1D_h[pos];
                s_mean+=(VISIOCYTELONG)image1D_s[pos];
                v_count++;
            }
        }
    }
    if(v_count<=0){
        qDebug()<<"==========NeuronPicker: an empty region!";
        return 0;
    }
    h_mean/=v_count;
    s_mean/=v_count;

    unsigned char* image1D_mask=memory_allocate_uchar1D(sz_img[0]*sz_img[1]*sz_img[2]);
    memset(image1D_mask, 0, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(unsigned char));

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                qDebug()<<"==========NeuronPicker: "<<dx<<":"<<dy<<":"<<dz<<":"<<image1D_h[pos]<<":"<<v_mean;
                if(image1D_h[pos]<0)    continue;
                if(huedis(image1D_h[pos],h_mean)>colorSpan) continue;
//                if(huedis(image1D_h[pos],h_mean)+saturationdis(image1D_s[pos],s_mean)>colorSpan) continue;
                image1D_out[pos]=image1D_v[pos];
                image1D_mask[pos]=1;
                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                    pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                    image1D_out[pos]=image1D_v[pos];
                    if(image1D_h[pos]<0)    continue;
                    if(image1D_mask[pos]>0) continue;
                    if(huedis(image1D_h[pos],h_mean)>colorSpan) continue;
//                    if(huedis(image1D_h[pos],h_mean)+saturationdis(image1D_s[pos],s_mean)>colorSpan) continue;
                    image1D_mask[pos]=1;
                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    memory_free_uchar1D(image1D_mask);
    return seeds.size();
}

//input is color matrix
VISIOCYTELONG neuronPickerMain::extract_color(int *image1D_h, unsigned char *image1Dc_in, unsigned char *image1D_s, unsigned char *image1Dc_out, VISIOCYTELONG _pos_input, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4])
{
    vector<VISIOCYTELONG> seeds;
    VISIOCYTELONG delta=cubSize/2;
    VISIOCYTELONG h_mean=0, v_count=0, s_mean=0;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_img[0];
    VISIOCYTELONG z_offset=sz_img[0]*sz_img[1];
    VISIOCYTELONG page=sz_img[0]*sz_img[1]*sz_img[2];
    vector<VISIOCYTELONG> coord;
    coord=pos2xyz(_pos_input, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];
    qDebug()<<"==========NeuronPicker: seed:"<<x<<":"<<y<<":"<<z<<":"<<delta;
    //find the average hue in seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                qDebug()<<"==========NeuronPicker: "<<dx<<":"<<dy<<":"<<dz<<":"<<image1D_h[pos];
                if(image1D_h[pos]<0)    continue;
                h_mean+=image1D_h[pos];
                s_mean+=(VISIOCYTELONG)image1D_s[pos];
                v_count++;
            }
        }
    }
    if(v_count<=0){
        qDebug()<<"==========NeuronPicker: an empty region!";
        return 0;
    }
    h_mean/=v_count;
    s_mean/=v_count;

    unsigned char* image1D_mask=memory_allocate_uchar1D(sz_img[0]*sz_img[1]*sz_img[2]);
    memset(image1D_mask, 0, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(unsigned char));

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                qDebug()<<"==========NeuronPicker: "<<dx<<":"<<dy<<":"<<dz<<":"<<image1D_h[pos]<<":"<<v_mean;
                if(image1D_h[pos]<0)    continue;
                if(huedis(image1D_h[pos],h_mean)>colorSpan) continue;
//                if(huedis(image1D_h[pos],h_mean)+saturationdis(image1D_s[pos],s_mean)>colorSpan) continue;
                image1Dc_out[pos]=image1Dc_in[pos];
                image1Dc_out[pos+page]=image1Dc_in[pos+page];
                image1Dc_out[pos+page*2]=image1Dc_in[pos+page*2];
                image1D_mask[pos]=1;
                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";
    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                    pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                    image1Dc_out[pos]=image1Dc_in[pos];
                    image1Dc_out[pos+page]=image1Dc_in[pos+page];
                    image1Dc_out[pos+page*2]=image1Dc_in[pos+page*2];
                    if(image1D_h[pos]<0)    continue;
                    if(image1D_mask[pos]>0) continue;
                    if(huedis(image1D_h[pos],h_mean)>colorSpan) continue;
//                    if(huedis(image1D_h[pos],h_mean)+saturationdis(image1D_s[pos],s_mean)>colorSpan) continue;
                    image1D_mask[pos]=1;
                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    memory_free_uchar1D(image1D_mask);
    return seeds.size();
}

//this function will extract less in comparison with extract();
VISIOCYTELONG neuronPickerMain::extract_mono(int *image1D_h, unsigned char *image1D_v, unsigned char *image1D_s, unsigned char *image1D_out, VISIOCYTELONG _pos_input, int cubSize, int colorSpan, VISIOCYTELONG sz_img[4])
{
    vector<VISIOCYTELONG> seeds;
    VISIOCYTELONG delta=cubSize/2;
    VISIOCYTELONG h_mean=0, v_count=0, s_mean=0;
    VISIOCYTELONG x,y,z,pos;
    VISIOCYTELONG y_offset=sz_img[0];
    VISIOCYTELONG z_offset=sz_img[0]*sz_img[1];
    vector<VISIOCYTELONG> coord;
    coord=pos2xyz(_pos_input, y_offset, z_offset);
    x=coord[0];
    y=coord[1];
    z=coord[2];
    qDebug()<<"==========NeuronPicker: seed:"<<x<<":"<<y<<":"<<z<<":"<<delta;
    //find the average hue in seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                qDebug()<<"==========NeuronPicker: "<<dx<<":"<<dy<<":"<<dz<<":"<<image1D_h[pos];
                if(image1D_h[pos]<0)    continue;
                h_mean+=image1D_h[pos];
                s_mean+=(VISIOCYTELONG)image1D_s[pos];
                v_count++;
            }
        }
    }
    if(v_count<=0){
        qDebug()<<"==========NeuronPicker: an empty region!";
        return 0;
    }
    h_mean/=v_count;
    s_mean/=v_count;

    unsigned char* image1D_mask=memory_allocate_uchar1D(sz_img[0]*sz_img[1]*sz_img[2]);
    memset(image1D_mask, 0, sz_img[0]*sz_img[1]*sz_img[2]*sizeof(unsigned char));

    //populate the init seed regions
    for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
        for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
            for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                qDebug()<<"==========NeuronPicker: "<<dx<<":"<<dy<<":"<<dz<<":"<<image1D_h[pos]<<":"<<v_mean;
                if(image1D_h[pos]<0)    continue;
                if(huedis(image1D_h[pos],h_mean)>colorSpan) continue;
//                if(huedis(image1D_h[pos],h_mean)+saturationdis(image1D_s[pos],s_mean)>colorSpan) continue;
                image1D_out[pos]=image1D_v[pos];
                image1D_mask[pos]=1;
                seeds.push_back(pos);
            }
        }
    }

    qDebug()<<"==========NeuronPicker: found "<<seeds.size()<<" initial seeds";

    //seed grow
    VISIOCYTELONG sid=0;
    while(sid<seeds.size()){
        coord=pos2xyz(seeds[sid], y_offset, z_offset);
        x=coord[0];
        y=coord[1];
        z=coord[2];
        //find the average hue in seed regions
        for(VISIOCYTELONG dx=MAX(x-delta,0); dx<=MIN(sz_img[0]-1,x+delta); dx++){
            for(VISIOCYTELONG dy=MAX(y-delta,0); dy<=MIN(sz_img[1]-1,y+delta); dy++){
                for(VISIOCYTELONG dz=MAX(z-delta,0); dz<=MIN(sz_img[2]-1,z+delta); dz++){
                    pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                    if(image1D_h[pos]<0)    continue;
                    if(image1D_mask[pos]>0) continue;
                    if(huedis(image1D_h[pos],h_mean)>colorSpan) continue;
//                    if(huedis(image1D_h[pos],h_mean)+saturationdis(image1D_s[pos],s_mean)>colorSpan) continue;

                    image1D_out[pos]=image1D_v[pos];
                    image1D_mask[pos]=1;
                    seeds.push_back(pos);
                }
            }
        }
        sid++;
    }

    qDebug()<<"=========NeuronPicker: finally found "<<seeds.size()<<" voxels";

    memory_free_uchar1D(image1D_mask);
    return seeds.size();
}

int neuronPickerMain::rgb2hue(const unsigned char R, const unsigned char G, const unsigned char B)
{
    float r = (float)R/255.0;
    float g = (float)G/255.0;
    float b = (float)B/255.0;
    float maxColor=-1, minColor=1e6;
    int maxChannel=-1;
    int hue=-1;
    if(maxColor<r){
        maxColor=r;
        maxChannel=0;
    }
    if(maxColor<g){
        maxColor=g;
        maxChannel=1;
    }
    if(maxColor<b){
        maxColor=b;
        maxChannel=2;
    }
    if(minColor>r){
        minColor=r;
    }
    if(minColor>g){
        minColor=g;
    }
    if(minColor>b){
        minColor=b;
    }

    if(maxChannel==0){
        float tmp=(g-b)/(maxColor-minColor);
        if(tmp<0)
            tmp=tmp+6;
        hue=60*tmp;
    }
    if(maxChannel==1){
        hue=60.0*(((b-r)/(maxColor-minColor))+2.0);
    }
    if(maxChannel==2){
        hue=60.0*(((r-g)/(maxColor-minColor))+4.0);
    }
    if(maxColor-minColor<1e-5)
        hue=-2;
    return hue;
}

void neuronPickerMain::rgb2hsv(const unsigned char R, const unsigned char G, const unsigned char B, int & h, unsigned char & s, unsigned char & v)
{
    float r = (float)R/255.0;
    float g = (float)G/255.0;
    float b = (float)B/255.0;
    float maxColor=-1, minColor=1e6;
    int maxChannel=-1;
    int hue=-1;
    if(maxColor<r){
        maxColor=r;
        maxChannel=0;
    }
    if(maxColor<g){
        maxColor=g;
        maxChannel=1;
    }
    if(maxColor<b){
        maxColor=b;
        maxChannel=2;
    }
    if(minColor>r){
        minColor=r;
    }
    if(minColor>g){
        minColor=g;
    }
    if(minColor>b){
        minColor=b;
    }

    if(maxChannel==0){
        float tmp=(g-b)/(maxColor-minColor);
        if(tmp<0)
            tmp=tmp+6;
        hue=60*tmp;
    }
    if(maxChannel==1){
        hue=60.0*(((b-r)/(maxColor-minColor))+2.0);
    }
    if(maxChannel==2){
        hue=60.0*(((r-g)/(maxColor-minColor))+4.0);
    }
    if(maxColor-minColor<1e-5)
        hue=-2;
    h=hue;
    v=MAX(MAX(R,G),B);
    if(maxColor<1e-5){
        s=0;
    }else{
        s=(maxColor-minColor)/maxColor*255;
    }
}
