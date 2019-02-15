#ifndef __NEURON_RETRIEVE_H__
#define __NEURON_RETRIEVE_H__

#include "basic_surf_objs.h"
#include <math.h>

using namespace std;

bool neuron_retrieve(NeuronTree query, QList<double*> & feature_list, vector<VISIOCYTELONG>  & result, VISIOCYTELONG cand, int method_code, int norm_code);
bool print_result(vector<VISIOCYTELONG> result, const char* file_name, QStringList name_list, const char* database_name, const char* query_name);
bool compute_intersect(vector<vector<VISIOCYTELONG> > & l1,  vector<VISIOCYTELONG> & result, int thres, int rej_thres);
bool compute_intersect(vector<VISIOCYTELONG> l1, vector<VISIOCYTELONG> l2, vector<VISIOCYTELONG> & result, double thres);
bool read_matrix(const char* file_name, QList<double*> & matrix, VISIOCYTELONG line, VISIOCYTELONG width);
bool loadFeatureFile(QString file_name, QList<double*>&  morph_list, QList<double*> & gmi_list, QStringList & nameList);
#endif
