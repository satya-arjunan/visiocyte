#ifndef COMMON_H
#define COMMON_H

//#include "visiocyte_interface.h"
#include "spine_fun.h"

using namespace std;

int calc_nearest_node(NeuronTree neuron,float center_x,float center_y,float center_z);
void backupNeuron(NeuronTree &source, NeuronTree &backup);
bool write_marker_file(QString filename,LandmarkList listmarkers);
bool save_project_results(VISIOCYTEPluginCallback2 *callback, VISIOCYTELONG sz_img[4], vector<GOV> label_group, QString folder_output,
      QString input_swc_name, QString input_image_name, bool eswc_flag, NeuronTree neuron, LandmarkList LList, int sel_channel,
      int bg_thr, int max_dis, int seg_id, int marker_id, QString output_label_name, QString output_marker_name, QString output_csv_name, QString output_eswc_name);
void convert2UINT8(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
void convert2UINT8(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
void GetColorRGB(int* rgb, int idx);
vector<VISIOCYTELONG> pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);
VISIOCYTELONG xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);

vector<vector<int> > build_parent_LUT(NeuronTree *neuron_tmp);
vector<vector<int> > neurontree_divide_swc(NeuronTree *neuron_tmp, float distance_thresh);
NeuronTree *check_neuron_tree(NeuronTree neuron, VISIOCYTELONG sz_img[4]);
float calc_between_dis(NeuronTree *neuron,int node1_id,int node2_id);

void open_triview_window(QString main_win_name, VISIOCYTEPluginCallback2 *callback,
                       unsigned char *image, visiocytehandle &main_win, VISIOCYTELONG sz_img[4]);
float calc_head_width(GOV cur_group, int center_dis, float center_x, float center_y, float center_z);


#endif // COMMON_H
