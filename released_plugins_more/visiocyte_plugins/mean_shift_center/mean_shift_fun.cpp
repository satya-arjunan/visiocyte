#include "mean_shift_fun.h"

mean_shift_fun::mean_shift_fun()
{
    data1Dc_float=0;
    sz_image[0]=sz_image[1]=sz_image[2]=sz_image[3]=0;
    page_size=0;
}

mean_shift_fun::~mean_shift_fun()
{
    if(data1Dc_float!=0)
        memory_free_float1D(data1Dc_float);
}

vector<float> mean_shift_fun::mean_shift_center(VISIOCYTELONG ind, int windowradius)
{
    int methodcode=0; //if methodcode is 2,there are constrains.
    vector<float> center=calc_mean_shift_center(ind,windowradius,data1Dc_float,sz_image,methodcode);
    return center;
}

vector<float> mean_shift_fun::mean_shift_center_mass(VISIOCYTELONG ind, int windowradius)
{
    int methodcode=0; //if methodcode is 2,there are constrains.
    vector<float> center=calc_mean_shift_center_mass(ind,windowradius,data1Dc_float,sz_image,methodcode);
    return center;
}

vector<float> mean_shift_fun::mean_shift_with_constraint(VISIOCYTELONG ind, int windowradius)
{
    int methodcode=2; //if methodcode is 2,there are constrains.
    vector<float> center=calc_mean_shift_center(ind,windowradius,data1Dc_float,sz_image,methodcode);
    return center;
}

vector<float> mean_shift_fun::ray_shoot_center(VISIOCYTELONG ind,int bg_thr,int j)
{
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];

    VISIOCYTELONG pos,pos_prev;
    vector<VISIOCYTELONG> coord;
    float dir_vec[48][3]={{0.6667,0.6667,0.3333},{0.2852,0.6886,0.6667},{0.6886,0.2852,0.6667},
                        {0.2826,0.2826,0.9167},{-0.6667,0.6667,0.3333},{-0.6886,0.2852,0.6667},
                        {-0.2852,0.6886,0.6667},{-0.2826,0.2826,0.9167},{-0.6667,-0.6667,0.3333},
                        {-0.2852,-0.6886,0.6667},{-0.6886,-0.2852,0.6667},{-0.2826,-0.2826,0.9167},
                        {0.6667,-0.6667,0.3333},{0.6886,-0.2852,0.6667},{0.2852,-0.6886,0.6667},
                        {0.2826,-0.2826,0.9167},{0.9428,0.0000,-0.3333},{0.9239,0.3827,0.0000},
                        {0.9239,-0.3827,0.0000},{0.9428,0.0000,0.3333},{0.0000,0.9428,-0.3333},
                        {-0.3827,0.9239,0.0000},{0.3827,0.9239,0.0000},{0.0000,0.9428,0.3333},
                        {-0.9428,0.0000,-0.3333},{-0.9239,-0.3827,0.0000},{-0.9239,0.3827,0.0000},
                        {-0.9428,0.0000,0.3333},{0.0000,-0.9428,-0.3333},{0.3827,-0.9239,0.0000},
                        {-0.3827,-0.9239,0.0000},{0.0000,-0.9428,0.3333},{0.2826,0.2826,-0.9167},
                        {0.2852,0.6886,-0.6667},{0.6886,0.2852,-0.6667},{0.6667,0.6667,-0.3333},
                        {-0.2826,0.2826,-0.9167},{-0.6886,0.2852,-0.6667},{-0.2852,0.6886,-0.6667},
                        {-0.6667,0.6667,-0.3333},{-0.2826,-0.2826,-0.9167},{-0.2852,-0.6886,-0.6667},
                        {-0.6886,-0.2852,-0.6667},{-0.6667,-0.6667,-0.3333},{0.2826,-0.2826,-0.9167},
                        {0.6886,-0.2852,-0.6667},{0.2852,-0.6886,-0.6667},{0.6667,-0.6667,-0.3333}};

    int dir_vec_size=48;

    vector<float> center_float(3,0);
    vector<float> intensity_vec;
    float v_prev,v_new,x,y,z,x_new,y_new,z_new,sum_x,sum_y,sum_z,x_prev,y_prev,z_prev,dx_sum,dy_sum,dz_sum;
    float center_dis=1;
    float bound[48][3];
    coord=pos2xyz(ind, y_offset, z_offset);
    x=(float)coord[0];y=(float)coord[1];z=(float)coord[2];
    //qDebug()<<"x,y,z:"<<x<<":"<<y<<":"<<z;

    dx_sum=dy_sum=dz_sum=0;
    for (int k=0;k<dir_vec_size;k++)
    {
        dx_sum=dx_sum+ABS(dir_vec[k][0]);
        dy_sum=dy_sum+ABS(dir_vec[k][1]);
        dz_sum=dz_sum+ABS(dir_vec[k][2]);
    }

    //find out the channel with the maximum intensity for the marker
    v_prev=data1Dc_float[ind];
    int channel=0;
    for (int j=1;j<sz_image[3];j++)
    {
        if (data1Dc_float[ind+page_size*j]>v_prev)
        {
            v_prev=data1Dc_float[ind+page_size*j];
            channel=j;
        }
    }
//    QString myfile="record.txt";
//    FILE * fp1 = fopen(myfile.toLatin1(), "wt");
//    fprintf(fp1, "j=: %ld\n",j);
//    fprintf(fp1,"##dir_vect,x,y,z,intensity\n");

    float converge_dis=0.5;
    while (center_dis>converge_dis)
    {
        sum_x=sum_y=sum_z=0;

        for (int i=0;i<dir_vec_size;i++)
        {
            //qDebug()<<"new round";
            intensity_vec.clear();
            intensity_vec.push_back(v_prev);
            float dx=dir_vec[i][0];
            float dy=dir_vec[i][1];
            float dz=dir_vec[i][2];
            //qDebug()<<"dx,dy,dz:"<<dx<<":"<<dy<<":"<<dz;
            pos_prev=ind;
            x_prev=x;
            y_prev=y;
            z_prev=z;
            v_new=v_prev;

            while (v_new>bg_thr)
            {
                x_new=x_prev+dx;
                if (x_new<0 || x_new>sz_image[0]-1)
                {
                    y_new=y_prev+dy;
                    z_new=z_prev+dz;
                    //qDebug()<<"x touches edge";
                    break;
                }
                y_new=y_prev+dy;
                if (y_new<0 || y_new>sz_image[1]-1)
                {
                    z_new=z_prev+dz;
                    //qDebug()<<"y touches edge";
                    break;
                }
                z_new=z_prev+dz;
                if (z_new<0 || z_new>sz_image[2]-1)
                {
                    //qDebug()<<"z touches edge";
                    break;
                }
                pos=xyz2pos(x_new+0.5,y_new+0.5,z_new+0.5,y_offset,z_offset); //float to int
                v_new=data1Dc_float[pos+channel*page_size];
                x_prev=x_new;
                y_prev=y_new;
                z_prev=z_new;
                if (pos==pos_prev) continue;
                pos_prev=pos;
                intensity_vec.push_back(v_new);
                //qDebug()<<"dir i:"<<i<<" x,y,z:"<<x_prev<<":"<<y_prev<<":"<<":"<<z_prev<<":"<<"intensity:"<<v_new;

                //fprintf(fp1,"%1d %5.3f %5.3f %5.3f %4.1f \n",i,x_prev,y_prev,z_prev,v_new);
            }
            //qDebug()<<"intensity_vec size:"<<intensity_vec.size();
            //fprintf(fp1,"___________intensity_vec_size: %1d\n",intensity_vec.size());
            if (intensity_vec.size()>4)
            {
                int id;
                bool reverse=false;
                for (id=2;id<intensity_vec.size()-2;id++)
                {
                    if ((intensity_vec[id]<intensity_vec[id-1])&&(intensity_vec[id]<=intensity_vec[id-2]-9)
                      &&intensity_vec[id]<intensity_vec[id+1]&&intensity_vec[id]<=intensity_vec[id+2]-9)
                    {
                        //qDebug()<<"reversed order: "<<id;
                        bound[i][0]=x+id*dx;
                        bound[i][1]=y+id*dy;
                        bound[i][2]=z+id*dz;
                        sum_x=sum_x+ABS(dx)*bound[i][0];
                        sum_y=sum_y+ABS(dy)*bound[i][1];
                        sum_z=sum_z+ABS(dz)*bound[i][2];
                        reverse=true;
                        //fprintf(fp1,"reverse order %1d\n",id);
                        break;

                    }
                }
                if (!reverse)
                {
                    bound[i][0]=x_new-dx;
                    bound[i][1]=y_new-dy;
                    bound[i][2]=z_new-dz;
                    sum_x=sum_x+ABS(dx)*(x_new-dx);
                    sum_y=sum_y+ABS(dy)*(y_new-dy);
                    sum_z=sum_z+ABS(dz)*(z_new-dz);
                }

            }
            else
            {
                bound[i][0]=x_new-dx;
                bound[i][1]=y_new-dy;
                bound[i][2]=z_new-dz;
                //qDebug()<<"Out of loop";
                sum_x=sum_x+ABS(dx)*(x_new-dx);
                sum_y=sum_y+ABS(dy)*(y_new-dy);
                sum_z=sum_z+ABS(dz)*(z_new-dz);
            }

        }
        //qDebug()<<"bounds:"<<bound[0][0]<<":"<<bound[0][1]<<":"<<bound[0][2];
//        qDebug()<<"dx_sum,y,z:"<<dx_sum<<":"<<dy_sum<<":"<<dz_sum;
//        qDebug()<<"sum_x,y,z:"<<sum_x<<":"<<sum_y<<":"<<sum_z;
        if (sum_x<1e-5||sum_y<1e-5||sum_z<1e-5) //a very dark marker.
        {
            visiocyte_msg("marker "+QString::number(j+1)+ " is below background threshold. Ray shoot will not move"
                      "the location");
            center_float[0]=x;
            center_float[1]=y;
            center_float[2]=z;
            return center_float;
        }
        center_float[0]=(float)sum_x/dx_sum;
        center_float[1]=(float)sum_y/dy_sum;
        center_float[2]=(float)sum_z/dz_sum;


        float tmp_dis=(center_float[0]-x)*(center_float[0]-x)+(center_float[1]-y)*(center_float[1]-y)
                    +(center_float[2]-z)*(center_float[2]-z);
        center_dis=sqrt(tmp_dis);

        //qDebug()<<"new_center:"<<center_float[0]<<":"<<center_float[1]<<":"<<center_float[2];;
        //qDebug()<<"center distance:"<<center_dis;

        //fprintf(fp1,"center distance: %5.3f\n",center_dis);
        x=center_float[0]; y=center_float[1]; z=center_float[2];
        VISIOCYTELONG tmp_ind=xyz2pos((int)(center_float[0]+0.5),(int)(center_float[1]+0.5),(int)(center_float[2]+0.5),
                y_offset,z_offset);
        v_prev=data1Dc_float[tmp_ind+channel*page_size];

    }
    //fclose(fp1);
    //return center_float;
//    QString curFile = "result.swc";
//    qDebug()<<"writing file";
//    FILE * fp = fopen(curFile.toLatin1(), "a");
//    if (!fp)
//    {
//        visiocyte_msg("Could not open the file to save the neuron.");
//    }

//    fprintf(fp, "#name %s\n", qPrintable(QString("no name")));
//    fprintf(fp, "#comment %s\n", qPrintable(QString("no comment")));
//    fprintf(fp, "##n,type,x,y,z,radius,parent\n");
//    fprintf(fp, "%ld %d %5.3f %5.3f %5.3f %5.3f %ld\n",
//            1+j*49, 1, x, y, z, 1, -1);

//    for (int m=0+j*49;m<j*49+dir_vec_size; m++)
//    {
//        fprintf(fp, "%ld %d %5.3f %5.3f %5.3f %5.3f %ld\n",
//                m+2, 1, bound[m-j*49][0], bound[m-j*49][1], bound[m-j*49][2], 1, 1+j*49);
//    }

//    fclose(fp);
    return center_float;
}


vector<float> mean_shift_fun::gradient_transform(float *&outimg1d,VISIOCYTELONG ind,
                            int bg_thr, int cnn_type,int z_thickness,int halfwindowsize, int search_radius)
{
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];

    VISIOCYTELONG pos,pos1;
    vector<VISIOCYTELONG> coord;

    float v_prev,x,y,z;
    vector<float> center_float(3,0),center_float_tmp(3,0);

    coord=pos2xyz(ind, y_offset, z_offset);
    x=(float)coord[0];y=(float)coord[1];z=(float)coord[2];

    //find out the channel with the maximum intensity for the marker
    v_prev=data1Dc_float[ind];
    int channel=0;
    for (int j=1;j<sz_image[3];j++)
    {
        if (data1Dc_float[ind+page_size*j]>v_prev)
        {
            v_prev=data1Dc_float[ind+page_size*j];
            channel=j;
        }
    }

    int sz0,sz1,sz2,x_start,y_start,z_start;

    if (x<halfwindowsize) sz0=x+halfwindowsize+1;
    else if (x+halfwindowsize>sz_image[0]) sz0=sz_image[0]-x+halfwindowsize;
    else sz0=2*halfwindowsize+1;

    if (y<halfwindowsize) sz1=y+halfwindowsize+1;
    else if (y+halfwindowsize>sz_image[1]) sz1=sz_image[1]-y+halfwindowsize;
    else sz1=2*halfwindowsize+1;

    if (z<halfwindowsize) sz2=z+halfwindowsize+1;
    else if (z+halfwindowsize>sz_image[2]) sz2=sz_image[2]-z+halfwindowsize;
    else sz2=2*halfwindowsize+1;

//    qDebug()<<"halfsize:"<<halfwindowsize<<" x,y,z:"<<x<<":"<<y<<":"<<z;
//    qDebug()<<"sz0,sz1,sz2:"<<sz0<<":"<<sz1<<":"<<sz2;
    unsigned char *image_trun=new unsigned char[sz0*sz1*sz2];
    //memset(image_trun,0,size_page*sizeof(unsigned char));

    //Store the area around the marker in image_trun
    x_start=MAX(0,x-halfwindowsize);
    y_start=MAX(0,y-halfwindowsize);
    z_start=MAX(0,z-halfwindowsize);
    for (VISIOCYTELONG dx=x_start;dx<MIN(sz_image[0],x+halfwindowsize+1);dx++){
        for (VISIOCYTELONG dy=y_start;dy<MIN(sz_image[1],y+halfwindowsize+1);dy++){
            for (VISIOCYTELONG dz=z_start;dz<MIN(sz_image[2],z+halfwindowsize+1);dz++){
                pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                pos1=xyz2pos(dx-x_start,dy-y_start,dz-z_start,sz0,sz0*sz1);
                image_trun[pos1]=data1Dc_float[pos+channel*page_size];
            }
        }
    }

    //put into image_trun gradient distance transform
    float *phi=0;
    fastmarching_dt(image_trun, phi,sz0, sz1, sz2, cnn_type, bg_thr, z_thickness);

    float min_val = phi[0], max_val = phi[0];

    float *outimg1d_tmp = new float[sz0*sz1*sz2*sizeof(float)];
    outimg1d=new float[sz_image[0]*sz_image[1]*sz_image[2]*sizeof(float)];

    for(VISIOCYTELONG i = 0; i < sz0*sz1*sz2; i++)
    {
        if(phi[i] == INF) continue;
        min_val = MIN(min_val, phi[i]);
        max_val = MAX(max_val, phi[i]);
    }
    //cout<<"min_val = "<<min_val<<" max_val = "<<max_val<<endl;
    max_val -= min_val; if(max_val == 0.0) max_val = 0.00001;

    for(VISIOCYTELONG i = 0; i < sz0*sz1*sz2; i++)
    {
        if(phi[i] == INF) outimg1d_tmp[i] = 0;
        else if(phi[i] ==0) outimg1d_tmp[i] = 0;
        else
        {
            outimg1d_tmp[i] = (phi[i] - min_val)/max_val * 255 + 0.5;
            outimg1d_tmp[i] = MAX(outimg1d_tmp[i], 1);
        }
    }

    memset(outimg1d,0,sz_image[0]*sz_image[1]*sz_image[2]*sizeof(float));
    for (int k=0;k<sz0;k++){
        for (int j=0;j<sz1;j++){
            for (int l=0;l<sz2;l++){
                outimg1d[k+x_start+(j+y_start)*y_offset+(l+z_start)*z_offset]=outimg1d_tmp[k+j*sz0+l*sz0*sz1];
            }
        }
    }
    VISIOCYTELONG sz[4]={sz0,sz1,sz2,1};
    VISIOCYTELONG ind_tr=xyz2pos(x-x_start,y-y_start,z-z_start,sz0,sz0*sz1);
    center_float_tmp=calc_mean_shift_center(ind_tr,search_radius,outimg1d_tmp,sz,2);
    center_float[0]=center_float_tmp[0]+x_start;
    center_float[1]=center_float_tmp[1]+y_start;
    center_float[2]=center_float_tmp[2]+z_start;
    //qDebug()<<"center_float:"<<center_float[0]<<":"<<center_float[1]<<":"<<center_float[2];
    return center_float;
}

vector<float> calc_mean_shift_center(VISIOCYTELONG ind, int windowradius,float *data1Dc_float,
                                     VISIOCYTELONG sz_image[],int methodcode)
{
    //qDebug()<<"methodcode:"<<methodcode;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    VISIOCYTELONG page_size=sz_image[0]*sz_image[1]*sz_image[2];

    VISIOCYTELONG N = sz_image[0];
    VISIOCYTELONG M = sz_image[1];
    VISIOCYTELONG P = sz_image[2];

    VISIOCYTELONG pos;
    vector<VISIOCYTELONG> coord;

    float total_x,total_y,total_z,v_color,sum_v,v_prev,x,y,z;
    float center_dis=1;
    vector<float> center_float(3,0);

    coord=pos2xyz(ind, y_offset, z_offset);
    x=(float)coord[0];y=(float)coord[1];z=(float)coord[2];
    //qDebug()<<"x,y,z:"<<x<<":"<<y<<":"<<z<<"ind:"<<ind;

    //find out the channel with the maximum intensity for the marker
    v_prev=data1Dc_float[ind];
    int channel=0;
    for (int j=1;j<sz_image[3];j++)
    {
        if (data1Dc_float[ind+page_size*j]>v_prev)
        {
            v_prev=data1Dc_float[ind+page_size*j];
            channel=j;
        }
    }
    //qDebug()<<"v_Prev:"<<v_prev;
    int testCount=0;
    int testCount1=0;

    while (center_dis>=0.5 && testCount<50)
    {
        total_x=total_y=total_z=sum_v=0;
        testCount++;
        testCount=testCount1=0;

        for(VISIOCYTELONG dx=MAX(x+0.5-windowradius,0); dx<=MIN(sz_image[0]-1,x+0.5+windowradius); dx++){
            for(VISIOCYTELONG dy=MAX(y+0.5-windowradius,0); dy<=MIN(sz_image[1]-1,y+0.5+windowradius); dy++){
                for(VISIOCYTELONG dz=MAX(z+0.5-windowradius,0); dz<=MIN(sz_image[2]-1,z+0.5+windowradius); dz++){
                    pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                    double tmp=(dx-x)*(dx-x)+(dy-y)*(dy-y)
                         +(dz-z)*(dz-z);
                    double distance=sqrt(tmp);
                    if (distance>windowradius) continue;
                    v_color=data1Dc_float[pos+page_size*channel];

                    total_x=v_color*(float)dx+total_x;
                    total_y=v_color*(float)dy+total_y;
                    total_z=v_color*(float)dz+total_z;
                    sum_v=sum_v+v_color;

                    testCount++;
                    if(v_color>100)
                        testCount1++;
                 }
             }
         }


//        qDebug()<<"windowradius:"<<windowradius;
//        qDebug()<<"total xyz:"<<total_x<<":"<<total_y<<":"<<total_z<<":"<<sum_v<<":"<<sum_v/testCount<<":"<<testCount<<":"<<testCount1;

        center_float[0]=total_x/sum_v;
        center_float[1]=total_y/sum_v;
        center_float[2]=total_z/sum_v;
        qDebug()<<"new_center:"<<center_float[0]<<":"<<center_float[1]<<":"<<center_float[2];

        if (total_x<1e-5||total_y<1e-5||total_z<1e-5) //a very dark marker.
        {

            visiocyte_msg("Sphere surrounding the marker is zero. Mean-shift cannot happen. Marker location will not move",0);
            center_float[0]=x;
            center_float[1]=y;
            center_float[2]=z;
            return center_float;
        }

        VISIOCYTELONG prev_ind=xyz2pos((int)(x+0.5),(int)(y+0.5),(int)(z+0.5),y_offset,z_offset);
        VISIOCYTELONG tmp_ind=xyz2pos((int)(center_float[0]+0.5),(int)(center_float[1]+0.5),(int)(center_float[2]+0.5),
                y_offset,z_offset);

        if (methodcode==2)
        {
            if (data1Dc_float[tmp_ind+channel*page_size]<data1Dc_float[prev_ind+channel*page_size]&& windowradius>=2) // && windowradius>2)
            {
            //qDebug()<<methodcode<<" window too large"<<windowradius;
            windowradius--;
            center_dis=1;
            continue;
            }
         }

        float tmp_1=(center_float[0]-x)*(center_float[0]-x)+(center_float[1]-y)*(center_float[1]-y)
                    +(center_float[2]-z)*(center_float[2]-z);
        center_dis=sqrt(tmp_1);

//        qDebug()<<"new_center:"<<center_float[0]<<":"<<center_float[1]<<":"<<center_float[2]<<" intensity:"<<data1Dc_float[tmp_ind+channel*page_size];
//        qDebug()<<"center distance:"<<center_dis;
        x=center_float[0]; y=center_float[1]; z=center_float[2];
    }

    return center_float;
}

vector<float> calc_mean_shift_center_mass(VISIOCYTELONG ind, int windowradius,float *data1Dc_float,
                                     VISIOCYTELONG sz_image[],int methodcode)
{
    //qDebug()<<"methodcode:"<<methodcode;
    VISIOCYTELONG y_offset=sz_image[0];
    VISIOCYTELONG z_offset=sz_image[0]*sz_image[1];
    VISIOCYTELONG page_size=sz_image[0]*sz_image[1]*sz_image[2];

    VISIOCYTELONG N = sz_image[0];
    VISIOCYTELONG M = sz_image[1];
    VISIOCYTELONG P = sz_image[2];

    VISIOCYTELONG pos;
    vector<VISIOCYTELONG> coord;

    float total_x,total_y,total_z,v_color,sum_v,v_prev,x,y,z;
    float center_dis=1;
    vector<float> center_float(3,0);

    coord=pos2xyz(ind, y_offset, z_offset);
    x=(float)coord[0];y=(float)coord[1];z=(float)coord[2];
    //qDebug()<<"x,y,z:"<<x<<":"<<y<<":"<<z<<"ind:"<<ind;

    //find out the channel with the maximum intensity for the marker
    v_prev=data1Dc_float[ind];
    int channel=0;
    for (int j=1;j<sz_image[3];j++)
    {
        if (data1Dc_float[ind+page_size*j]>v_prev)
        {
            v_prev=data1Dc_float[ind+page_size*j];
            channel=j;
        }
    }
    //qDebug()<<"v_Prev:"<<v_prev;
    int testCount=0;
//    int testCount1=0;

    while (center_dis>=0.5 && testCount<50)
    {
        total_x=total_y=total_z=sum_v=0;
        testCount++;
     //   testCount=testCount1=0;

//        for(VISIOCYTELONG dx=MAX(x+0.5-windowradius,0); dx<=MIN(sz_image[0]-1,x+0.5+windowradius); dx++){
//            for(VISIOCYTELONG dy=MAX(y+0.5-windowradius,0); dy<=MIN(sz_image[1]-1,y+0.5+windowradius); dy++){
//                for(VISIOCYTELONG dz=MAX(z+0.5-windowradius,0); dz<=MIN(sz_image[2]-1,z+0.5+windowradius); dz++){
//                    pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
//                    double tmp=(dx-x)*(dx-x)+(dy-y)*(dy-y)
//                         +(dz-z)*(dz-z);
//                    double distance=sqrt(tmp);
//                    if (distance>windowradius) continue;
//                    v_color=data1Dc_float[pos+page_size*channel];

//                    total_x=v_color*(float)dx+total_x;
//                    total_y=v_color*(float)dy+total_y;
//                    total_z=v_color*(float)dz+total_z;
//                    sum_v=sum_v+v_color;

//                    testCount++;
//                    if(v_color>100)
//                        testCount1++;
//                 }
//             }
//         }


        VISIOCYTELONG xb = MAX(x+0.5-windowradius,0);
        VISIOCYTELONG xe = MIN(sz_image[0]-1,x+0.5+windowradius);
        VISIOCYTELONG yb = MAX(y+0.5-windowradius,0);
        VISIOCYTELONG ye = MIN(sz_image[1]-1,y+0.5+windowradius);
        VISIOCYTELONG zb = MAX(z+0.5-windowradius,0);
        VISIOCYTELONG ze = MIN(sz_image[2]-1,z+0.5+windowradius);

        VISIOCYTELONG i,j,k;
        double w;

        //first get the average value
        double x2, y2, z2;
        double rx2 = double(windowradius+1)*(windowradius+1);
        double ry2 = rx2, rz2 = rx2;
        double tmpd;
        double xm=0,ym=0,zm=0, s=0, mv=0, n=0;

        s = 0; n = 0;
        for(k=zb; k<=ze; k++)
        {
            VISIOCYTELONG offsetkl = k*M*N;
            z2 = k-z; z2*=z2;
            for(j=yb; j<=ye; j++)
            {
                VISIOCYTELONG offsetjl = j*N;
                y2 = j-y; y2*=y2;
                tmpd = y2/ry2 + z2/rz2;
                if (tmpd>1.0)
                    continue;

                for(i=xb; i<=xe; i++)
                {
                    x2 = i-x; x2*=x2;
                    if (x2/rx2 + tmpd > 1.0)
                        continue;

                    s += double(data1Dc_float[offsetkl + offsetjl + i]);
                    n = n+1;
                }
            }
        }
        if (n!=0)
            mv = s/n;
        else
            mv = 0;

        //now get the center of mass
        s = 0; n=0;
        for(k=zb; k<=ze; k++)
        {
            VISIOCYTELONG offsetkl = k*M*N;
            z2 = k-z; z2*=z2;
            for(j=yb; j<=ye; j++)
            {
                VISIOCYTELONG offsetjl = j*N;
                y2 = j-y; y2*=y2;
                tmpd = y2/ry2 + z2/rz2;
                if (tmpd>1.0)
                    continue;

                for(i=xb; i<=xe; i++)
                {
                    x2 = i-x; x2*=x2;
                    if (x2/rx2 + tmpd > 1.0)
                        continue;

                    w = double(data1Dc_float[offsetkl + offsetjl + i]) - mv;
                    if (w>0)
                    {
                    xm += w*i;
                    ym += w*j;
                    zm += w*k;
                    s += w;
                    n = n+1;
                    }
                }
            }
        }

        if(n>0)
        {
            xm /= s; ym /=s; zm /=s;
        }
        else
        {
            xm = x; ym=y; zm=z;
        }

        center_float[0]=xm;
        center_float[1]=ym;
        center_float[2]=zm;

//        qDebug()<<"windowradius:"<<windowradius;
//        qDebug()<<"total xyz:"<<total_x<<":"<<total_y<<":"<<total_z<<":"<<sum_v<<":"<<sum_v/testCount<<":"<<testCount<<":"<<testCount1;

//        center_float[0]=total_x/sum_v;
//        center_float[1]=total_y/sum_v;
//        center_float[2]=total_z/sum_v;
//        qDebug()<<"new_center:"<<center_float[0]<<":"<<center_float[1]<<":"<<center_float[2];

//        if (total_x<1e-5||total_y<1e-5||total_z<1e-5) //a very dark marker.
//        {

//            visiocyte_msg("Sphere surrounding the marker is zero. Mean-shift cannot happen. Marker location will not move",0);
//            center_float[0]=x;
//            center_float[1]=y;
//            center_float[2]=z;
//            return center_float;
//        }

        VISIOCYTELONG prev_ind=xyz2pos((int)(x+0.5),(int)(y+0.5),(int)(z+0.5),y_offset,z_offset);
        VISIOCYTELONG tmp_ind=xyz2pos((int)(center_float[0]+0.5),(int)(center_float[1]+0.5),(int)(center_float[2]+0.5),
                y_offset,z_offset);

        if (methodcode==2)
        {
            if (data1Dc_float[tmp_ind+channel*page_size]<data1Dc_float[prev_ind+channel*page_size]&& windowradius>=2) // && windowradius>2)
            {
            //qDebug()<<methodcode<<" window too large"<<windowradius;
            windowradius--;
            center_dis=1;
            continue;
            }
         }

        float tmp_1=(center_float[0]-x)*(center_float[0]-x)+(center_float[1]-y)*(center_float[1]-y)
                    +(center_float[2]-z)*(center_float[2]-z);
        center_dis=sqrt(tmp_1);

//        qDebug()<<"new_center:"<<center_float[0]<<":"<<center_float[1]<<":"<<center_float[2]<<" intensity:"<<data1Dc_float[tmp_ind+channel*page_size];
//        qDebug()<<"center distance:"<<center_dis;
        x=center_float[0]; y=center_float[1]; z=center_float[2];
    }

    return center_float;
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

void convert2UINT8_meanshift(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz)
{
    unsigned short* pPre = (unsigned short*)pre1d;
    unsigned short max_v=0, min_v = 255;
    for(VISIOCYTELONG i=0; i<imsz; i++)
    {
        if(max_v<pPre[i]) max_v = pPre[i];
        if(min_v>pPre[i]) min_v = pPre[i];
    }
    max_v -= min_v;
    if(max_v>0)
    {
        for(VISIOCYTELONG i=0; i<imsz; i++)
        {
            pPost[i] = (unsigned char) 255*(double)(pPre[i] - min_v)/max_v;
        }
    }
    else
    {
        for(VISIOCYTELONG i=0; i<imsz; i++)
        {
            pPost[i] = (unsigned char) pPre[i];
        }
    }
}

void convert2UINT8_meanshift(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz)
{
    float* pPre = (float*)pre1d;
    float max_v=0, min_v = 65535;
    for(VISIOCYTELONG i=0; i<imsz; i++)
    {
        if(max_v<pPre[i]) max_v = pPre[i];
        if(min_v>pPre[i]) min_v = pPre[i];
    }
    max_v -= min_v;
    if(max_v>0)
    {
        for(VISIOCYTELONG i=0; i<imsz; i++)
        {
            pPost[i] = (unsigned char) 255*(double)(pPre[i] - min_v)/max_v;
        }
    }
    else
    {
        for(VISIOCYTELONG i=0; i<imsz; i++)
        {
            pPost[i] = (unsigned char) pPre[i];
        }
    }
}


bool load_data(VISIOCYTEPluginCallback2 *cb,unsigned char * & image1Dc_in,LandmarkList &LList,ImagePixelType &pixeltype,
               VISIOCYTELONG sz_img[4],visiocytehandle &curwin)
{
    VISIOCYTEPluginCallback2 *callback=cb;
    visiocytehandleList visiocytehandleList_current=callback->getImageWindowList();
    QList <VisiocyteR_MainWindow *> cur_list_3dviewer = callback->getListAll3DViewers();
    LandmarkList LList_in;
    QList<NeuronTree> * mTreeList = 0;
    if (visiocytehandleList_current.size()==0){
        visiocyte_msg("Please open image and select markers");
        return false;
    }
    else if (visiocytehandleList_current.size()==1)  //One window open
    {
        //get markers and check markers
        LList_in.clear();
        if(callback->getLandmark(visiocytehandleList_current[0]).size()>0)
        {
            LList_in = callback->getLandmark(visiocytehandleList_current[0]);
        }else if (callback->getListAll3DViewers().size()>0)
        {
            mTreeList = callback->getHandleNeuronTrees_3DGlobalViewer(visiocytehandleList_current[0]);
            if(mTreeList->size() == 0)
            {
                visiocyte_msg("Please load markers or the swc file");
                return false;
            }
        }else
        {
            visiocyte_msg("Please load markers or the swc file");
            return false;
        }


        curwin=visiocytehandleList_current[0];
    }
    else if (visiocytehandleList_current.size()>1)
    {
        QStringList items;
        int i;
        for (i=0; i<visiocytehandleList_current.size(); i++)
            items << callback->getImageName(visiocytehandleList_current[i]);

        for (i=0; i<cur_list_3dviewer.count(); i++)
        {
            QString curname = callback->getImageName(cur_list_3dviewer[i]).remove("3D View [").remove("]");
            bool b_found=false;
            for (int j=0; j<visiocytehandleList_current.size(); j++)
                if (curname==callback->getImageName(visiocytehandleList_current[j]))
                {
                    b_found=true;
                    break;
                }

            if (!b_found)
                items << callback->getImageName(cur_list_3dviewer[i]);
        }
        //qDebug()<<"Number of items:"<<items.size();

        QDialog *mydialog=new QDialog;
        QComboBox *combo=new QComboBox;
        combo->insertItems(0,items);
        QLabel *label_win=new QLabel;
        label_win->setText("You have multiple windows open, please select one image:");
        QGridLayout *layout= new QGridLayout;
        layout->addWidget(label_win,0,0,1,1);
        layout->addWidget(combo,1,0,4,1);
        QPushButton *button_d_ok=new QPushButton("Ok");
        button_d_ok->setFixedWidth(100);
        QPushButton *button_d_cancel=new QPushButton("Cancel");
        button_d_cancel->setFixedWidth(100);
        QHBoxLayout *box=new QHBoxLayout;
        box->addWidget(button_d_ok,Qt::AlignCenter);
        box->addWidget(button_d_cancel,Qt::AlignCenter);
        layout->addLayout(box,5,0,1,1);
        QDialog::connect(button_d_ok,SIGNAL(clicked()),mydialog,SLOT(accept()));
        QDialog::connect(button_d_cancel,SIGNAL(clicked()),mydialog,SLOT(reject()));

        mydialog->setLayout(layout);
        mydialog->exec();
        if (mydialog->result()==QDialog::Accepted)
        {
            int tmp=combo->currentIndex();
            curwin=visiocytehandleList_current[tmp];
        }
        else
        {
            visiocyte_msg("You have not selected a window");
            return false;
        }
        //get markers and check markers
        LList_in.clear();
        if(callback->getLandmark(visiocytehandleList_current[0]).size()>0)
        {
            LList_in = callback->getLandmark(visiocytehandleList_current[0]);
        }else if (callback->getListAll3DViewers().size()>0)
        {
            mTreeList = callback->getHandleNeuronTrees_3DGlobalViewer(visiocytehandleList_current[0]);
            if(mTreeList->size() == 0)
            {
                visiocyte_msg("Please load markers or the swc file");
                return false;
            }
        }else
        {
            visiocyte_msg("Please load markers or the swc file");
            return false;
        }
    }

    //Get the image info
    Image4DSimple* p4DImage = callback->getImage(curwin);
    if (!p4DImage){
        QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
        return false;
    }

    sz_img[0]=p4DImage->getXDim();
    sz_img[1]=p4DImage->getYDim();
    sz_img[2]=p4DImage->getZDim();
    sz_img[3]=p4DImage->getCDim();

    if (sz_img[3]>3){
        sz_img[3]=3;
        QMessageBox::information(0,"","More than 3 channels were loaded."
                                 "The first 3 channel will be applied for analysis.");
    }

    VISIOCYTELONG size_tmp=sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3];
    pixeltype = p4DImage->getDatatype();
    image1Dc_in=memory_allocate_uchar1D(size_tmp);
    unsigned char * ptmp=p4DImage->getRawData();
    memcpy(image1Dc_in,ptmp,size_tmp*pixeltype);

    LList.clear();
    if(LList_in.size() > 0)
    {
        for(int i=0; i<LList_in.size(); i++){
            LList.append(LList_in.at(i));
            LList[i].color.r=196;
            LList[i].color.g=LList[i].color.b=0;
        }
    }else
    {
        for(int i=0; i<mTreeList->at(0).listNeuron.size(); i++)
        {
            LocationSimple swc_node;
            swc_node.x = mTreeList->at(0).listNeuron.at(i).x + 1;
            swc_node.y = mTreeList->at(0).listNeuron.at(i).y + 1;
            swc_node.z = mTreeList->at(0).listNeuron.at(i).z + 1;
            swc_node.radius = mTreeList->at(0).listNeuron.at(i).r;
            LList.append(swc_node);
            LList[i].color.r=196;
            LList[i].color.g=LList[i].color.b=0;
        }
    }

    return true;
}
