/* sort_func.h
 * 2012-02-01 : by Yinan Wan
 */
 
#ifndef __TRACING_FUNC_H__
#define __TRACING_FUNC_H__

#include <visiocyte_interface.h>
#include "../../../released_plugins/visiocyte_plugins/neurontracing_vn2/app2/my_surf_objs.h"
#include "../../../released_plugins/visiocyte_plugins/istitch/y_imglib.h"
enum tracingMethod {app1, app2, neutube,snake,most,mst, neurogpstree,rivulet2,tremap,gd,advantra,neuronchaser};

struct TRACE_LS_PARA
{
    int is_gsdt;
    int is_break_accept;
    int  bkg_thresh;
    double length_thresh;
    int  cnn_type;
    int  channel;
    double SR_ratio;
    int  b_256cube;
    int b_RadiusFrom2D;
    int block_size;
    int adap_win;
    int tracing_3D;
    int tracing_comb;
    int grid_trace;
    int global_name;
    int soma;

    VISIOCYTELONG in_sz[3];

    int  visible_thresh;//for APP1 use only

    int  seed_win; //for MOST use only
    int  slip_win; //for MOST use only

    tracingMethod  method;

    Image4DSimple* image;
    LandmarkList listLandmarks;
    QString tcfilename,inimg_file,rawfilename,markerfilename,swcfilename,inimg_file_2nd;
};


bool crawler_raw_app(VISIOCYTEPluginCallback2 &callback, QWidget *parent,TRACE_LS_PARA &p,bool bmenu);
bool app_tracing(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);

bool app_tracing_ada_win(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);
bool app_tracing_ada_win_3D(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);


bool crawler_raw_all(VISIOCYTEPluginCallback2 &callback, QWidget *parent,TRACE_LS_PARA &p,bool bmenu);
bool all_tracing(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);

bool all_tracing_ada_win(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);
bool all_tracing_ada_win_3D(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);

bool ada_win_finding(LandmarkList tips,LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList,Image4DSimple* total4DImage,int block_size,int direction);
bool ada_win_finding_3D(LandmarkList tips,LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList,Image4DSimple* total4DImage,int block_size,int direction);
bool ada_win_finding_3D_GD(LandmarkList tips,LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList,Image4DSimple* total4DImage,int block_size,int direction);

QList<LandmarkList> group_tips(LandmarkList tips,int block_size, int direction);
NeuronTree sort_eliminate_swc(NeuronTree nt,LandmarkList inputRootList,Image4DSimple* total4DImage);
LandmarkList eliminate_seed(NeuronTree nt,LandmarkList inputRootList,Image4DSimple* total4DImage);
bool combine_list2file(QList<NeuronSWC> & lN, QString fileSaveName);

void processSmartScan(VISIOCYTEPluginCallback2 &callback,list<string> & infostring,QString fileWithData);
void processSmartScan_3D(VISIOCYTEPluginCallback2 &callback,list<string> & infostring,QString fileWithData);
void processSmartScan_3D_wofuison(VISIOCYTEPluginCallback2 &callback,list<string> & infostring,QString fileWithData);

NeuronTree neuron_sub(NeuronTree nt_total, NeuronTree nt);

bool load_region_tc(VISIOCYTEPluginCallback2 &callback,QString &tcfile, Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim,unsigned char * & img,
                    VISIOCYTELONG startx, VISIOCYTELONG starty, VISIOCYTELONG startz, VISIOCYTELONG endx, VISIOCYTELONG endy, VISIOCYTELONG endz);


bool grid_raw_all(VISIOCYTEPluginCallback2 &callback, QWidget *parent,TRACE_LS_PARA &p,bool bmenu);
bool all_tracing_grid(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &P,VISIOCYTELONG ix, VISIOCYTELONG iy,VISIOCYTELONG iz);

bool combo_tracing_ada_win(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);
bool combo_tracing_ada_win_3D(VISIOCYTEPluginCallback2 &callback,TRACE_LS_PARA &p,LandmarkList inputRootList, LocationSimple tileLocation,LandmarkList *newTargetList,QList<LandmarkList> *newTipsList);

NeuronTree DL_eliminate_swc(NeuronTree nt,QList <ImageMarker> marklist);
NeuronTree pruning_cross_swc(NeuronTree nt);

bool extract_tips(VISIOCYTEPluginCallback2 &callback, QWidget *parent,TRACE_LS_PARA &P);
bool tracing_pair_app(VISIOCYTEPluginCallback2 &callback, QWidget *parent,TRACE_LS_PARA &p,bool bmenu);

vector<MyMarker> extract_branch_pts(VISIOCYTEPluginCallback2 &callback, const QString& filename,NeuronTree nt);
NeuronTree smartPrune(NeuronTree nt, double length);
void smartFuse(VISIOCYTEPluginCallback2 &callback,QString inputFolder, QString outputFile);

#endif

