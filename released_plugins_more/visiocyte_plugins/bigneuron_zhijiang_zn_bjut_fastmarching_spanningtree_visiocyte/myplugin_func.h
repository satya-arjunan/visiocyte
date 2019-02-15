/* example_func.h
 * This is an example plugin, the header file of plugin functions.
 * 2012-02-10 : by Yinan Wan
 */

#ifndef __MY_FUNC_H__
#define __MY_FUNC_H__

#include <visiocyte_interface.h>
#include "node.h"
#include "tree.h"
#include "graph.h"
#include "visiocyte_message.h"
#include "basic_surf_objs.h"

bool proc(VISIOCYTEPluginCallback2 &callback,QWidget* parent,unsigned char* data1d,VISIOCYTELONG* in_sz,QString inimg_file);
template<class T>
void mst(QMap<VISIOCYTELONG,Graph<T>*> map,QList<Tree<T>*> &rootList,QMap<VISIOCYTELONG,Tree<T>*> &treeMap);//最小生成树
//void findNode(unsigned char* img1d, QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap);//获取前景色点
//void findNode(unsigned char* img1d, QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap,double m=0,int n1=0);
//void findNode_sec(unsigned char* img1d, QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap,double mean,int n1);
void initPath(unsigned char* img1d,QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap);//初始化图结构
void calculateRadius_old(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap);//抛弃使用的计算半径方法
void calculateRadius(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap);//计算半径
void prundNodeByRadius(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap);//根据半径合并点
void calculateWeight(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap,QMap<VISIOCYTELONG,Node*> nodeMapCopy);//计算边权重
void calculateTreeWeight(QList<Tree<Node*>*> rootList
                         , QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> &rootMap);
void reCreate(QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> rootMap
                , QMap<VISIOCYTELONG, Tree<Tree<Node *> *> *> treeRootMap
                , QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap);
void reCreateTree(Tree<Node*>* curNode,QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap);
void myplugin_proc(unsigned char* img1d);//算法入口
#endif
