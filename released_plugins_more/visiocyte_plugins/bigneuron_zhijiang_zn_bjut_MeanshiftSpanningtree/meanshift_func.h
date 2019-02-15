/* example_func.h
 * This is an example plugin, the header file of plugin functions.
 * 2012-02-10 : by Yinan Wan
 */
 
#ifndef __EXAMPLE_FUNC_H__
#define __EXAMPLE_FUNC_H__

#include <visiocyte_interface.h>
#include<Gradient.h>
#include "node.h"
struct input_PARA
{
    QString inimg_file;
    VISIOCYTELONG channel;
    double prim_distance;//the distance decide to delete covered nodes
    double threshold;//the threshold which used in determining noisy node
    double percentage;//same effect with threshold
};

int meanshift_plugin_vn4(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
void bf_vn3(QMap<int,Node* > roots,double **weight_result,unsigned char * &img1d,double average_dis,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void meanshift_vn5(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration);
Node getnode(Node *node);
void printf_path_between_roots(QMultiMap<VISIOCYTELONG,QMultiMap<VISIOCYTELONG,QList<Node> > > root_path,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
double Map_coordinate(Node current_center,Node target_node,VISIOCYTELONG relate_x,VISIOCYTELONG relate_y,VISIOCYTELONG relate_z);
QMap<VISIOCYTELONG,QList<Node> >  build_adj_matrix(QMultiMap<VISIOCYTELONG,QMultiMap<VISIOCYTELONG,QList<Node> > > path_between_each_roots,QMap<VISIOCYTELONG,QList<Node> >  node_con_node);
Node choose_region_vn4(unsigned char * &img1d,struct sphere_model_two_directions sphere_m,Node source_node,Node target_node,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,QList<Node> > > sphere_search(unsigned char * &img1d,QMap<int,Node* > cluster_root,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,double r,int iteration);
QMultiMap<VISIOCYTELONG,QMultiMap<VISIOCYTELONG,QList<Node> > > spanning_combined_bf_vn3(unsigned char * &img1d,QMap<int,Node* > cluster_root,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,double r,int iteration);
QMultiMap<VISIOCYTELONG,QMultiMap<VISIOCYTELONG,QList<Node> > > spanning_combined_bf_vn2(unsigned char * &img1d,QMap<int,Node* > cluster_root,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,double r,int iteration);
Node choose_region_vn3(unsigned char * &img1d,struct sphere_model_four_directions sphere_m,Node source_node,Node target_node,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
Node choose_region_vn2(unsigned char * &img1d,struct sphere_model sphere_m,Node source_node,Node target_node,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<Node> change_type_to_QList(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,Node> >  roots);
QList<NeuronSWC> fulfill_path_between_roots(QList<NeuronSWC> con_tree,QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,QList<Node> > > path_between_each_roots,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<NeuronSWC> spanning_tree_algorithm(double** markEdge,QList<Node> seeds);
QList <NeuronSWC > calculate_distance_between_roots(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,QList<Node> > > path_between_each_roots,QMap<int,Node* > roots,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
Node choose_region(struct sphere_model sphere_m,Node source_node,Node target_node);
QMultiMap<VISIOCYTELONG,QMultiMap<VISIOCYTELONG,QList<Node> > > spanning_combined_bf(unsigned char * &img1d,QMap<int,Node* > cluster_root,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,double r,int iteration);
QMap<VISIOCYTELONG,Node> merge_cluster_node(QMap<VISIOCYTELONG,Node> rootnodes,double distance);

QMap<VISIOCYTELONG,QList<Node> >  check_node_conection(QMultiMap<VISIOCYTELONG,QMultiMap<VISIOCYTELONG,QList<Node> > > path_between_each_roots,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QMap<int,Node*> delete_cluster_node(unsigned char * &img1d,QMap<VISIOCYTELONG,QList<Node> >  final_cluster,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG *in_sz,double prim_distance);
 QMap<VISIOCYTELONG,QList<Node> >  cluster2newroot(QMap<VISIOCYTELONG,QList<VISIOCYTELONG> >  covered,QMultiMap<VISIOCYTELONG,Node> cluster);
QList<NeuronSWC> spanning_without_bf(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,Node> >  roots);

//QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,Node> >  delete_cluster_node(unsigned char * &img1d,QMap<VISIOCYTELONG,QList<Node> >  final_cluster,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG *in_sz,double prim_distance);
 QMap<VISIOCYTELONG,QList<Node> >  cluster2newroot(QMap<VISIOCYTELONG,QList<VISIOCYTELONG> >  covered,QMultiMap<VISIOCYTELONG,Node> cluster);
QList<NeuronSWC> spanning_without_bf(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,Node> >  roots);

//QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,Node> > delete_cluster_node(unsigned char * &img1d,QMap<VISIOCYTELONG,QList<Node> > final_cluster,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG *in_sz,double prim_distance);
 QMap<VISIOCYTELONG,QList<Node> > cluster2newroot(QMap<VISIOCYTELONG,QList<VISIOCYTELONG> > covered,QMultiMap<VISIOCYTELONG,Node> cluster);
QList<NeuronSWC> spanning_without_bf(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,Node> > roots);

bool determine_noisy(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,double threshold,double percentage,int iter_step);
Node choose_next_node(unsigned char * &img1d,Node* pre,VISIOCYTELONG next_x,VISIOCYTELONG next_y,VISIOCYTELONG next_z,QList<Node> choose,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QMap<int,Node*> merge_rootnode_vn2(QMap<int,Node*> &rootnodes,unsigned char * &img1d,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,double distance);
long meanshift_vn6(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration,VISIOCYTELONG *in_sz);
long meanshift_vn7(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration,VISIOCYTELONG *in_sz);
bool found_final_vn3(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r);
QList<NeuronSWC> connect_shortest_path_vn5(unsigned char * &img1d,QList<Node> path,Node* begin,Node* end,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG *in_sz);
QList<NeuronSWC> connect_shortest_path_vn4(unsigned char * &img1d,QList<Node> path,Node* begin,Node* end,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<NeuronSWC> smooth_SWC_radius(QList<NeuronSWC> target,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
double fix_radius(QHash<VISIOCYTELONG,NeuronSWC> neuron,VISIOCYTELONG Parent,QList<VISIOCYTELONG> Childs );
QHash<VISIOCYTELONG,VISIOCYTELONG> Child_Parent_Wan(QList<NeuronSWC> target,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QHash<VISIOCYTELONG,VISIOCYTELONG> Parent_Child_Wan(QList<NeuronSWC> target,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void change_type(std::vector<NeuronSWC *> &target,QList<NeuronSWC> source);
QList<NeuronSWC> connect_shortest_path_vn3(unsigned char * &img1d,QList<Node> path,Node* begin,Node* end,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<NeuronSWC> connect_shortest_path_vn2(unsigned char * &img1d,QList<Node> path,Node* begin,Node* end,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<NeuronSWC> connect_shortest_path(unsigned char * &img1d,QList<Node> path,NeuronSWC begin,NeuronSWC end,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
double distance_calculate_vn2(unsigned char * &img1d,QList<Node> path,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<Node> found_path_vn3( QMap<VISIOCYTELONG,Node> path_map, Node* temp,Node* temp1,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<Node*> found_path_vn2( QMap<VISIOCYTELONG,Node*> path_map, Node* temp,Node* temp1,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void bf_vn2(QMap<int,Node* > roots,double **weight_result,unsigned char * &img1d,double average_dis,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
double distance_calculate(unsigned char * &img1d,QList<Node*> path,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void bf(QMap<int,Node* > roots,double **weight_result,unsigned char * &img1d,double average_dis,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
bool contain(QList<Node> *queue,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z);
int meanshift_plugin(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
int meanshift_plugin_vn2(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
QList<Node*> meanshift_vn2(unsigned char * &img1d,int *flag,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration);
QList<Node*> meanshift_vn3(unsigned char * &img1d,int *flag,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration);
void enlarge_radiusof_root_xy(unsigned char * &img1d,QList<Node*> &class_List,Node * &root,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
bool exist(VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,QList<Node*> List);
void meanshift_vn4(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r,int iteration);
void construct_tree(QMap<int,QList<Node*> > finalclass_node,unsigned char * &img1d,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
int meanshift_plugin_vn4(VISIOCYTEPluginCallback2 &callback, QWidget *parent,unsigned char* img1d,VISIOCYTELONG *in_sz, QString &image_name,bool bmenu,input_PARA &PARA);
int meanshift_plugin(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void printHelp();
QList <NeuronSWC> DFS_construct(double** markEdge,QList<Node> seeds,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void merge_spanning_tree(QList<QList <NeuronSWC> > &tree_part);
void bubble_sort(QMap<int,double> &root_r,int n);
void spanning_tree1(QList<Node*> &seeds);
void prepare_write(QList<NeuronSWC> marker_MST_sorted);
QList<Node*> enlarge_radiusof_allnodes_xy(unsigned char * &img1d,QList<Node*> &class_List,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
QList<Node*> trim_nodes(QList<Node*> seed,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
NeuronTree  post_process(NeuronTree nt);
void mark_nodeList(int mark,QList<Node*> &List,int s);
int found_finalnode_mark(VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,QList<Node*> final_List);
bool found_final(unsigned char * &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,VISIOCYTELONG r);
int ClassMark(QList<Node *> final_List);
void merge_rootnode(QMap<int,Node*> &rootnodes,unsigned char * &img1d,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void printSWCByMap_List(QMap<int,QList<Node*> >  List,char * filename);
QMap<VISIOCYTELONG,Node*> searchAndConnectByWs(unsigned char* &img1d,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,unsigned int ws);
VISIOCYTELONG meanshift(unsigned char* &img1d,VISIOCYTELONG x,VISIOCYTELONG y,VISIOCYTELONG z,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,unsigned int ws);
void printSwcByList(QList<Node*> nodeList,char* path);
void printSwcByMap(QMap<int,Node*> nodeMap, char *path);
void createTree(unsigned char* &img1d,Node* curNode,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,unsigned int ws);
bool checkConnect(unsigned char* &img1d,Node* begin,Node* end,unsigned int ws,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
void prundUnConnectNode(QMap<VISIOCYTELONG,Node*> nodeMap);
QList<Gradient*> gradient(QList<Node*> nodes,unsigned char * &img1d,VISIOCYTELONG sz_x,VISIOCYTELONG sz_y,VISIOCYTELONG sz_z);
#endif
