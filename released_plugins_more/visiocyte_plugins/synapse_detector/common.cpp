#include "common.h"
#include <math.h>

int calc_nearest_node(NeuronTree neuron,float center_x,float center_y,float center_z)
{
    float distance=1e6;
    int nearest_node_id=0;
    for (int i=0;i<neuron.listNeuron.size();i++)
    {
        float tmp_dis=(center_x-neuron.listNeuron[i].x)*(center_x-neuron.listNeuron[i].x)+
           (center_y-neuron.listNeuron[i].y)*(center_y-neuron.listNeuron[i].y)+
                (center_z-neuron.listNeuron[i].z)*(center_z-neuron.listNeuron[i].z);
        if (tmp_dis<distance)
        {
            distance=tmp_dis;
            nearest_node_id=i;
        }
    }
    return nearest_node_id;
}

void backupNeuron(NeuronTree & source, NeuronTree & backup)
{
    NeuronTree *np = (NeuronTree *)(&backup);
    np->n=source.n; np->on=source.on; np->selected=source.selected; np->name=source.name; np->comment=source.comment;
    np->color.r=source.color.r; np->color.g=source.color.g; np->color.b=source.color.b; np->color.a=source.color.a;
    np->listNeuron.clear();

    for(VISIOCYTELONG i=0; i<source.listNeuron.size(); i++)
    {
        NeuronSWC S;
        S.n = source.listNeuron[i].n;
        S.type = source.listNeuron[i].type;
        S.x = source.listNeuron[i].x;
        S.y = source.listNeuron[i].y;
        S.z = source.listNeuron[i].z;
        S.r = source.listNeuron[i].r;
        S.pn = source.listNeuron[i].pn;
        //S.seg_id = source.listNeuron[i].seg_id;
        //S.level = source.listNeuron[i].level;
        //S.fea_val = source.listNeuron[i].fea_val;
        S.fea_val.clear();
        for (int j=0;j<5;j++)
        {
            S.fea_val.append(-1);
        }
        np->listNeuron.append(S);
    }
    np->hashNeuron = source.hashNeuron;
    //np->file     = source.file;
    np->editable = source.editable;
    np->linemode = source.linemode;
}

bool write_marker_file(QString filename,LandmarkList listmarkers)
{
    FILE * fp = fopen(filename.toAscii(), "w");
    if (!fp)
    {
        qDebug()<<"cannot open the file to save the landmark points.\n";
        return false;
    }
    if (listmarkers.size()==0)
        return false;
    fprintf(fp, "##x,y,z,radius,shape,name,comment,color_r,color_g,color_b,category\n");
    for (int i=0;i<listmarkers.size();i++)
    {
        fprintf(fp,"%d,%d,%d,0,0,%s,%s,%d,%d,%d,%d\n",(int)listmarkers[i].x,(int)listmarkers[i].y,
                (int)listmarkers[i].z,listmarkers.at(i).name.c_str(),listmarkers.at(i).comments.c_str(),
                listmarkers[i].color.r,listmarkers[i].color.g,listmarkers[i].color.b,listmarkers[i].category);
    }
    fclose(fp);
    qDebug()<<"marker file succesfully written";
    return true;
}

bool save_project_results(VISIOCYTEPluginCallback2 *callback,VISIOCYTELONG sz_img[4],vector<GOV> label_group,QString folder_output,
      QString input_swc_name,QString input_image_name,bool eswc_flag, NeuronTree neuron,LandmarkList LList,int sel_channel,int bg_thr,int max_dis
        ,int seg_id,int marker_id,QString output_label_name,QString output_marker_name,QString output_csv_name,QString output_eswc_name)

{
    //qDebug()<<"saving profiles " <<"eswc flag:"<<eswc_flag;
    //save label image
    VISIOCYTELONG size_page=sz_img[0]*sz_img[1]*sz_img[2];
    VISIOCYTELONG label_sz[4];
    label_sz[0]=sz_img[0];
    label_sz[1]=sz_img[1];
    label_sz[2]=sz_img[2];
    label_sz[3]=1;
    unsigned short *tmp_label = new unsigned short[size_page];
    memset(tmp_label,0,size_page*sizeof(unsigned short));

    for(int i=0; i<label_group.size(); i++)
    {
        GOV tmp = label_group[i];
        for (int j=0; j<tmp.size(); j++)
        {
            tmp_label[tmp.at(j)->pos]=tmp.at(j)->intensity_label;
            //tmp_label[tmp.at(j)->pos+size_page]=tmp.at(j)->dst;
        }
    }
    QString fname_output = QDir(folder_output).filePath(output_label_name);
    if (!simple_saveimage_wrapper(*callback, fname_output.toStdString().c_str(), (unsigned char *)tmp_label, label_sz, 2))
        return false;
    if (tmp_label!=0){
        delete[] tmp_label; tmp_label=0;}
    qDebug()<<"finish saving label";

    //save markerfile
    QString marker_name=QDir(folder_output).filePath(output_marker_name);
    if (!write_marker_file(marker_name,LList))
        return false;
    qDebug()<<"finish saving markerfile";

    //save eswc file
    QString eswc_file=QDir(folder_output).filePath(output_eswc_name);
    NeuronTree nt_copy;
    //backupNeuron(neuron,nt_copy);

    //save csv file
    QString csv_file=QDir(folder_output).filePath(output_csv_name);
    FILE *fp2=fopen(csv_file.toAscii(),"w");
    if (!fp2)
        return false;
    if (0) //eswc_flag)
        fprintf(fp2,"##marker_id,auto_detect_id,volume,max_dis,min_dis,center_dis,head_width,center_x,center_y,center_z,skel_node,skel_type,skel_node_seg,skel_node_branch,dis_to_root,tree_id\n");
    else
        fprintf(fp2,"##marker_id,auto_detect_id,volume,max_dis,min_dis,center_dis,head_width,center_x,center_y,center_z,skel_node\n");

    for (int i=0;i<LList.size();i++)
    {
        QString tmp_comment;
        tmp_comment=QString::fromStdString(LList[i].comments);
        if (tmp_comment.contains("1"))// || tmp_comment.contains("0"))
        {
            GOV tmp=label_group[i];
            if (tmp.size()<=0) continue;
            std::sort(tmp.begin(),tmp.end(),sortfunc_dst);
            int group_id=tmp.front()->intensity_label;
            int max_dis=tmp.front()->dst;
            int min_dis=tmp.back()->dst;
            int volume=tmp.size();

            float sum_dis=0;

            for (int j=0;j<tmp.size();j++)
                sum_dis+=(float)tmp[j]->dst;

            int center_dis=sum_dis/tmp.size()+0.5;
            int skel_id=calc_nearest_node(neuron,LList.at(i).x-1,LList.at(i).y-1,LList.at(i).z-1);
            //qDebug()<<"skel_id_size:"<<skel_id_size<<" skel_id:"<<skel_id;
            float head_width=calc_head_width(tmp,center_dis,LList[i].x-1.,LList[i].y-1.,LList[i].z-1.);

            //construct eswc file
//            if (nt_copy.listNeuron[skel_id].fea_val[0]==-1)
//            {
//                nt_copy.listNeuron[skel_id].fea_val[0]=volume;
//                nt_copy.listNeuron[skel_id].fea_val[1]=center_dis;
//                nt_copy.listNeuron[skel_id].fea_val[2]=head_width;
//            }
//            else
//            {
//                NeuronSWC S;
//                S.n = nt_copy.listNeuron.length()+1;
//                qDebug()<<"~~~~~~~~~~~~~New node add:"<<S.n<<"ori node: "<<nt_copy.listNeuron[skel_id].n
//                       <<" ori node parent:"<<nt_copy.listNeuron[skel_id].parent;
//                S.type = nt_copy.listNeuron[skel_id].type;
//                S.x = nt_copy.listNeuron[skel_id].x;
//                S.y = nt_copy.listNeuron[skel_id].y;
//                S.z = nt_copy.listNeuron[skel_id].z;
//                S.r = nt_copy.listNeuron[skel_id].r;
//                S.parent = nt_copy.listNeuron[skel_id].parent;
//                S.fea_val.clear();
//                S.fea_val.push_back(volume);
//                S.fea_val.push_back(center_dis);
//                S.fea_val.push_back(head_width);
//                for (int jj=0;jj<2;jj++)
//                    S.fea_val.push_back(-1);
//                nt_copy.listNeuron[skel_id].parent=S.n;
//                nt_copy.listNeuron.append(S);
//            }
            //done with eswc file

            if (0) //eswc_flag)
            {
                fprintf(fp2,"%d,%d,%d,%d,%d,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%.2f,%d\n",i+1,group_id,volume,max_dis,
                        min_dis,center_dis,head_width,(int)LList.at(i).x-1,(int)LList.at(i).y-1,(int)LList.at(i).z-1,skel_id,
                        neuron.listNeuron.at(skel_id).type,(int)neuron.listNeuron.at(skel_id).seg_id,
                        (int)neuron.listNeuron.at(skel_id).level, neuron.listNeuron.at(skel_id).fea_val[1],
                        (int)neuron.listNeuron.at(skel_id).fea_val[0]);
            }
            else
                fprintf(fp2,"%d,%d,%d,%d,%d,%d,%.2f,%d,%d,%d,%d\n",i+1,group_id,volume,max_dis,min_dis
                        ,center_dis,head_width,(int)LList.at(i).x-1,(int)LList.at(i).y-1,(int)LList.at(i).z-1,skel_id);
        }
    }
    fclose(fp2);
    qDebug()<<"finish saving csv";

    //complete saving eswc file
    //writeESWC_file(eswc_file,nt_copy);

    //get the project profile txt
    QString tmp_name="project.txt";
    QString project_name=QDir(folder_output).filePath(tmp_name);
    QFile qfile(project_name);
    if (!qfile.open(QIODevice::WriteOnly))
    {
        //visiocyte_msg("Cannot open txt file for writing!");
        return false;
    }
    QTextStream out(&qfile);
    out<<"RAWIMG="<<input_image_name<<endl;
    out<<"SWCFILE="<<input_swc_name<<endl;
    out<<"MASKIMG="<<fname_output<<endl;
    out<<"MARKER="<<marker_name<<endl;
    out<<"CSV="<<csv_file<<endl;
    out<<"PARA_BG="<<bg_thr<<endl;
    out<<"PARA_MAXDIS="<<max_dis<<endl;
    out<<"PARA_CHANNEL="<<sel_channel<<endl;
    //which segment it is at now
    out<<"SEG_ID="<<seg_id<<endl; //for by spine view seg_id=-1
    //which marker it is at
    out<<"MAKRER_ID="<<marker_id<<endl;

    qfile.close();
    qDebug()<<"file complete wrriting";
    return true;
}

float calc_head_width(GOV cur_group, int center_dis, float center_x, float center_y, float center_z)
{
    //cur_group is sorted by dst //decending
    //qDebug()<<"In calc head width:"<<"center width:"<<center_dis;
    int minNumberofvoxels=8;
    int vid_begin,vid_end;
    vid_begin=vid_end=0;

    while(vid_begin<cur_group.size() && cur_group[vid_begin]->dst>center_dis){
        //qDebug()<<"cur_group[vid_begin]:"<<cur_group[vid_begin]->dst;
        vid_begin++;
        continue;
    }

    vid_end=vid_begin;
    while(vid_end<cur_group.size()&&cur_group[vid_end]->dst==center_dis)
    {
        vid_end++;
        continue;
    }
    if (vid_end-vid_begin<minNumberofvoxels) //include two more layers
    {
        while(vid_end<cur_group.size() && cur_group[vid_end]->dst>=center_dis-1){
            vid_end++;
            continue;
        }
        vid_begin=0;
        while(vid_begin<cur_group.size() && cur_group[vid_begin]->dst>center_dis+1){
            vid_begin++;
            continue;
        }
    }
    //qDebug()<<":"<<vid_end-vid_begin;
//    float sum_x,sum_y,sum_z;
//    sum_x=sum_y=sum_z=0;
//    for (int i=vid_begin;i<vid_end;i++)
//    {
//        sum_x+=cur_layer[i]->x;
//        sum_y+=cur_layer[i]->y;
//        sum_z+=cur_layer[i]->z;
//    }

//    center_x=sum_x/cur_layer.size();
//    center_y=sum_y/cur_layer.size();
//    center_z=sum_z/cur_layer.size();

    float max_dis=0;
    for (int k=vid_begin;k<vid_end;k++)
    {
        float dis=(cur_group[k]->x-center_x)*(cur_group[k]->x-center_x)+
                (cur_group[k]->y-center_y)*(cur_group[k]->y-center_y)+
                (cur_group[k]->z-center_z)*(cur_group[k]->z-center_z);
        if (dis>max_dis)
            max_dis=dis;
    }
    max_dis=2*sqrt(max_dis);
    return max_dis;
}

void convert2UINT8(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz)
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

void convert2UINT8(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz)
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

void GetColorRGB(int* rgb, int idx)
{
    idx=idx+1;
    if(idx>=0){
    idx = idx > 0 ? idx % 128 : 128 - abs(idx % 128);
    int colorLib[128][3] = {
            {39,16,2}, //{55, 173, 188},
            {3, 91, 61},
            {237, 195, 97},
            {175, 178, 151},
            {245, 199, 137},
            {24, 226, 66},
            {118, 84, 193},
            {205, 205, 224},
            {22, 170, 128},
            {86, 150, 90},
            {53, 67, 25},
            {38, 82, 152},
            {55,173,188}, //{39, 16, 2},
            {197, 174, 31},
            {183, 41, 31},
            {174, 37, 15},
            {34, 4, 48},
            {18, 10, 79},
            {132, 100, 254},
            {112, 39, 198},
            {189, 22, 120},
            {75, 104, 110},
            {222, 180, 4},
            {6, 60, 153},
            {236, 85, 113},
            {25, 182, 184},
            {200, 240, 12},
            {204, 119, 71},
            {11, 201, 229},
            {198, 214, 88},
            {49, 97, 65},
            {5, 75, 61},
            {52, 89, 137},
            {32, 49, 125},
            {104, 187, 76},
            {206, 233, 195},
            {85, 183, 9},
            {157, 29, 13},
            {14, 7, 244},
            {134, 193, 150},
            {188, 81, 216},
            {126, 29, 207},
            {170, 97, 183},
            {107, 14, 149},
            {76, 104, 149},
            {80, 38, 253},
            {27, 85, 10},
            {235, 95, 252},
            {139, 144, 48},
            {55, 124, 56},
            {239, 71, 243},
            {208, 89, 6},
            {87, 98, 24},
            {247, 48, 42},
            {129, 130, 13},
            {94, 149, 254},
            {111, 177, 4},
            {39, 229, 12},
            {7, 146, 87},
            {56, 231, 174},
            {95, 102, 52},
            {61, 226, 235},
            {199, 62, 149},
            {51, 32, 175},
            {232, 191, 210},
            {57, 99, 107},
            {239, 27, 135},
            {158, 71, 50},
            {104, 92, 41},
            {228, 112, 171},
            {54, 120, 13},
            {126, 69, 174},
            {191, 100, 143},
            {187, 156, 148},
            {18, 95, 9},
            {104, 168, 147},
            {249, 113, 198},
            {145, 5, 131},
            {104, 56, 59},
            {112, 235, 81},
            {73, 93, 127},
            {207, 60, 6},
            {77, 76, 211},
            {35, 208, 220},
            {141, 5, 129},
            {182, 178, 228},
            {179, 239, 3},
            {209, 9, 132},
            {167, 192, 71},
            {154, 227, 164},
            {200, 125, 103},
            {224, 181, 211},
            {3, 24, 139},
            {218, 67, 198},
            {163, 185, 228},
            {196, 53, 11},
            {39, 183, 33},
            {56, 116, 119},
            {23, 129, 20},
            {42, 191, 85},
            {78, 209, 250},
            {247, 147, 60},
            {74, 172, 146},
            {51, 244, 86},
            {38, 84, 29},
            {197, 202, 150},
            {20, 49, 198},
            {90, 214, 34},
            {178, 49, 101},
            {86, 235, 214},
            {161, 221, 55},
            {17, 173, 136},
            {132, 65, 217},
            {210, 4, 121},
            {241, 117, 217},
            {137, 111, 6},
            {129, 224, 232},
            {73, 34, 0},
            {81, 135, 211},
            {172, 233, 193},
            {43, 246, 89},
            {153, 187, 222},
            {85, 118, 43},
            {119, 116, 33},
            {163, 229, 109},
            {45, 75, 15},
            {15, 7, 140},
            {144, 78, 192}
    };
    for(int k = 0; k < 3; k++)
        rgb[k] = colorLib[idx][k];
    }
    else{
        rgb[0]=0;
        rgb[1]=0;
        rgb[2]=0;
    }
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

//for manual proofreading segment view
vector<vector<int> > build_parent_LUT(NeuronTree *neuron_tmp)
{
    int size=neuron_tmp->listNeuron.size();
    vector<vector<int> > parent_LUT;
    parent_LUT.clear();
    parent_LUT.resize(size);
    for (int i=0;i<size;i++)
    {
        int parent=neuron_tmp->listNeuron.at(i).parent;
        if (parent==-1)
        {
            continue;
        }
        parent_LUT[neuron_tmp->hashNeuron.value(parent)].push_back(i);
    }

    qDebug()<<"building new parnet LUT";
//    for (int i=0;i<parent_LUT.size();i++)
//    {
//        qDebug()<<"parent_LUT size:"<<i<<":"<<parent_LUT[i].size();
//    }
    return parent_LUT;
}


vector<vector<int> > neurontree_divide_swc(NeuronTree *neuron_tmp,float distance_thresh)
{
    //float distance_thresh=all_para.max_dis*5;
    qDebug()<<"in swc divide"<<distance_thresh;
    vector<int> leaf_nodes_id;
    vector<vector <int> > parent_LUT = build_parent_LUT(neuron_tmp);
    for (int i=0;i<neuron_tmp->listNeuron.size();i++)
    {
        if (parent_LUT[i].size()==0)
        {
           leaf_nodes_id.push_back(i);
        }
    }
    qDebug()<<"leaf nodes:"<<leaf_nodes_id.size();
    map<int,bool> used_flag; //use the idex starting from 0
    vector<vector<int> > nt_seg;

    for (int i=0;i<leaf_nodes_id.size();i++)
    {
        //qDebug()<<"i:"<<i;

        float between_distance,accu_distance;
        int start_node,parent_node,parent,next_parent_node;

        start_node=leaf_nodes_id[i];
        parent=neuron_tmp->listNeuron[start_node].parent;
        parent_node=neuron_tmp->hashNeuron.value(parent);
        used_flag[start_node]=1;
        vector<int> one_nt;
        one_nt.push_back(start_node);
        accu_distance=between_distance=calc_between_dis(neuron_tmp,start_node,parent_node);

        while (true)
        {
            while(accu_distance<distance_thresh && parent!=-1 && used_flag[parent_node]<=0)
            {
                one_nt.push_back(parent_node);
                used_flag[parent_node]=1;
                parent=neuron_tmp->listNeuron[parent_node].parent;
                next_parent_node=neuron_tmp->hashNeuron.value(parent);
                between_distance=calc_between_dis(neuron_tmp,parent_node,next_parent_node);
                accu_distance+=between_distance;
                parent_node=next_parent_node;

//                if (nt_seg.size()==5)
//                    qDebug()<<"5:accu_distance:"<<accu_distance<<"start_node:"<<start_node
//                     <<"x:"<<neuron.listNeuron[start_node].x<<"parent node:"<<parent_node<<"x:"<<neuron.listNeuron[parent_node].x;
            }
            if (parent==-1||used_flag[parent_node]>0)
            {
               nt_seg.push_back(one_nt);
               break;
            }
            else
            {
               nt_seg.push_back(one_nt);
               one_nt.clear();
               start_node=parent_node;
               one_nt.push_back(start_node);
               used_flag[start_node]=1;
               parent=neuron_tmp->listNeuron[start_node].parent;
               parent_node=neuron_tmp->hashNeuron.value(parent);
               accu_distance=between_distance=calc_between_dis(neuron_tmp,start_node,parent_node);
            }
        }
           //qDebug()<<"nt_seg size:"<<nt_seg.size();
    }
    qDebug()<<"After division swc. We have "<<nt_seg.size() <<" windows!";
//    for (int i=0;i<nt_seg.size();i++)
//    {
//        qDebug()<<"size:"<<nt_seg[i].size()<<" start:"<<nt_seg[i].front()<<"x:"<<neuron_tmp.listNeuron.at(nt_seg[i].front()).x-neuron_tmp.listNeuron.at(nt_seg[i].front()).r<<
//                 " end:"<<nt_seg[i].back()<<"x:"<<neuron_tmp.listNeuron.at(nt_seg[i].back()).x+neuron_tmp.listNeuron.at(nt_seg[i].back()).r;
//        vector<VISIOCYTELONG> coord(6,0);
//        coord=image_seg_plan(nt_seg[i],neuron_tmp);
//        qDebug()<<"xyz min:"<<coord[0]<<":"<<coord[1]<<":"<<coord[2]<<"xyz max:"<<coord[3]<<":"
//               <<coord[4]<<":"<<coord[5];
//    }
    return nt_seg;
}

NeuronTree* check_neuron_tree(NeuronTree neuron,VISIOCYTELONG sz_img[4])
{
    qDebug()<<"check neuron tree";
    map<int,bool> outlier_flag;
    QList<NeuronSWC> tmp_list;
    QHash <int, int>  hashNeuron;
    tmp_list.clear();

    for (int i=0;i<neuron.listNeuron.size();i++)
    {
        if (outlier_flag[i]>0)
            continue;

        if (neuron.listNeuron[i].x<0 ||neuron.listNeuron[i].x>sz_img[0]-1 ||
                neuron.listNeuron[i].y<0 ||neuron.listNeuron[i].y>sz_img[1]-1
                ||neuron.listNeuron[i].z<0 || neuron.listNeuron[i].z>sz_img[2]-1)
        {
            outlier_flag[i]=1;
            continue;
        }
        NeuronSWC tmp_swc;
        tmp_swc.x=neuron.listNeuron[i].x;
        tmp_swc.y=neuron.listNeuron[i].y;
        tmp_swc.z=neuron.listNeuron[i].z;
        tmp_swc.n=neuron.listNeuron[i].n;

        int parent=neuron.listNeuron[i].parent;
        int parent_node=neuron.hashNeuron.value(parent);

        if (neuron.listNeuron[parent_node].x<0 || neuron.listNeuron[parent_node].x>sz_img[0]-1
           || neuron.listNeuron[parent_node].y<0 ||neuron.listNeuron[parent_node].y>sz_img[1]-1
           || neuron.listNeuron[parent_node].z<0||neuron.listNeuron[parent_node].z>sz_img[2]-1)
        {
            tmp_swc.parent=-1;
            outlier_flag[parent_node]=1;
        }
        else
            tmp_swc.parent=parent;

        tmp_list.append(tmp_swc);
        hashNeuron.insert(tmp_swc.n, tmp_list.size()-1);
    }
    NeuronTree *checked_neuron=new NeuronTree;
    checked_neuron->listNeuron=tmp_list;
    checked_neuron->hashNeuron=hashNeuron;
    //writeSWC_file("test.swc",checked_neuron);
    //qDebug()<<"before check:"<<neuron.listNeuron.size()<<"after check:"<<tmp_list.size();
    return checked_neuron;
}

float calc_between_dis(NeuronTree *neuron,int node1_id,int node2_id)
{
    float distance=0;
    distance=(neuron->listNeuron[node1_id].x-neuron->listNeuron[node2_id].x)*(neuron->listNeuron[node1_id].x-neuron->listNeuron[node2_id].x)
            +(neuron->listNeuron[node1_id].y-neuron->listNeuron[node2_id].y)*(neuron->listNeuron[node1_id].y-neuron->listNeuron[node2_id].y)
            +(neuron->listNeuron[node1_id].z-neuron->listNeuron[node2_id].z)*(neuron->listNeuron[node1_id].z-neuron->listNeuron[node2_id].z);
    return (sqrt(distance));
}

void open_triview_window(QString win_name,VISIOCYTEPluginCallback2 *callback,
                       unsigned char *image,visiocytehandle &main_win,VISIOCYTELONG sz_img[4])  //image1dc_spine
{
    qDebug()<<"open triview window general format";
    bool window_open_flag=false;
    visiocytehandleList list_triwin = callback->getImageWindowList();
    for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
        if(callback->getImageName(list_triwin.at(i)).contains(win_name))
        {
            window_open_flag=true;
            main_win=list_triwin[i];
            return;
        }
    }
    if(!window_open_flag)
    {
        main_win = callback->newImageWindow(win_name);
        unsigned char *image_input=new unsigned char [sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]];
        memcpy(image_input,image,sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]);
        Image4DSimple image_main;
        image_main.setData(image_input,sz_img[0],sz_img[1],sz_img[2],sz_img[3],VISIOCYTE_UINT8);
        callback->setImage(main_win,&image_main);
        callback->updateImageWindow(main_win);
        qDebug()<<"opening a new window!!";
    }
}
