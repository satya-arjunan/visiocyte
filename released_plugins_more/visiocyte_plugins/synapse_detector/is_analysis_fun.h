#ifndef IS_ANALYSIS_FUN_H
#define IS_ANALYSIS_FUN_H

#include <QtGui>
#include <QDialog>
#include "visiocyte_interface.h"
#include "vector"
#include "map"

using namespace std;

struct is_parameters{
    int spine_bgthr; //background spine
    int is_bgthr;
    int max_dis;
    int min_voxel;
    int max_voxel;
    int spine_channel;
    int is_channel;
    int halfwindowsize;
};


struct GOI{
    vector<VISIOCYTELONG> voxel;
    int label_id;
    int nearest_node;
    bool on_dendrite;
    GOI(){label_id=-1;nearest_node=-1; on_dendrite=false;}

    GOI(vector<VISIOCYTELONG> _group, int _nearest_node,int _id,bool _ondendrite)
    {
        voxel=_group;
        nearest_node=_nearest_node;
        label_id=_id;
        on_dendrite=_ondendrite;
    }
    GOI(vector<VISIOCYTELONG> one_group,int id)
    {
        voxel=one_group;
        nearest_node=-1;
        label_id=id;
        on_dendrite=false;
    }
};

//typedef vector<GOI *> GIS;

class is_analysis_fun : public QDialog
{
    Q_OBJECT

public:
    explicit is_analysis_fun(VISIOCYTEPluginCallback2 *cb,QStringList name_list, QVector<int> para_list);
    void run();
    inline LandmarkList getLList() {return LList_out;}
    inline vector<GOI> getVoxelGroup() {return voxel_groups;}

private:
    VISIOCYTEPluginCallback2 * callback;
    VISIOCYTELONG sz_img[4],mask_sz[4],sz[4];
    NeuronTree nt,nt_copy;
    unsigned char * p_img1D, *mask,*image_trun;
    //unsigned char ***ppp_img3D;
    int type_img;
    is_parameters param;
    vector<GOI> voxel_groups;
    map<VISIOCYTELONG,bool> voxels_map;
    LandmarkList LList_out,LList_adj;
    QDialog *mydialog;
    QListWidget *list_markers;
    QPlainTextEdit *edit_box;
    int x_start,y_start,z_start,x_end,y_end,z_end;
    visiocytehandle main_win,curwin;
    QString basedir,swc_name,image_name,csv_out_name,eswc_out_name;

private:
    void obtain_mask();
    void connected_components();
    void stat_generate();
    void visualize_image();

    //bool eswc_generate(); not used

public slots:


public:
    bool pushImageData(unsigned char * data1Dc_in, VISIOCYTELONG new_sz_img[4])
    {
        //qDebug()<<"in push imageData";
//        if(ppp_img3D!=0){
//            delete [] ppp_img3D; ppp_img3D=0;
//        }
        sz_img[0]=new_sz_img[0];
        sz_img[1]=new_sz_img[1];
        sz_img[2]=new_sz_img[2];
        sz_img[3]=new_sz_img[3];

        //we know which channel to select
        //need to check this channel whether there are voxels

        VISIOCYTELONG size_tmp=sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3];
        p_img1D = new unsigned char [size_tmp];
        for(VISIOCYTELONG i=0; i<size_tmp; i++){
            p_img1D[i] = data1Dc_in[i];
        }
        //arrange the image into 3D
//        ppp_img3D=new unsigned char ** [sz_img[2]];
//        for(VISIOCYTELONG z=0; z<sz_img[2]; z++){
//            ppp_img3D[z]=new unsigned char * [sz_img[1]];
//            for(VISIOCYTELONG y=0; y<sz_img[1]; y++){
//                ppp_img3D[z][y]=p_img1D+y*sz_img[0]+z*sz_img[0]*sz_img[1];
//            }
//        }
        qDebug()<<"in pushnewdata:"<<sz_img[0]<<":"<<sz_img[1]<<":"<<sz_img[2];
        return true;
    }

    void pushSWCData(NeuronTree neuron)
    {
        nt.copy(neuron);
        qDebug()<<"nt size:"<<nt.listNeuron.size();
    }
};

#endif // IS_ANALYSIS_FUN_H
