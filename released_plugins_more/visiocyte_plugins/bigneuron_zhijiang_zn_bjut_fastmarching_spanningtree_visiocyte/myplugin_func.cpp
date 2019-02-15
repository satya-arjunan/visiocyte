﻿#include <iostream>
#include <stdlib.h>
#include <map>
#include "myplugin_func.h"
#include "global.h"
#include "node.h"
#include "tree.h"
#include "graph.h"
#include "../../../bigneuron_ported/APP2_ported/heap.h"
#include "visiocyte_message.h"
#include "stackutil.h"
#include "tree.h"
#include "graph.h"
#include <visiocyte_interface.h>
#include <sstream>


const QString title="MyPlugin";
void printSWC(QMap<VISIOCYTELONG,Tree<Node*>*> treeMap,string fileName);
/**
 * @brief mst 这个方法用来实现将图转化为最小生成树（求得每个连通域的最小生成树）
 * @param map 输入的带权图
 * @param rootList 输出的树根节点集合
 * @param treeMap 输出的树节点id索引图
 */
template<typename T>
void mst(QMap<VISIOCYTELONG,Graph<T>*> map,QList<Tree<T>*> &rootList,QMap<VISIOCYTELONG,Tree<T>*> &treeMap)
{
    BasicHeap<HeapElem> heap;
    QList<Path*> pathList;
    QMap<VISIOCYTELONG,int> visitNodeMap;//表示1维空间中的点坐标与树中点id的映射
    while(true)
    {
      bool isBreak = false;
        //todo 这里图有可能是非连通图。接下来应该把每个连通子图都生成一个最小生成树。然后再想办法把他们连接起来
        if(heap.empty())
        {
        isBreak = true;
        typedef typename QMap<VISIOCYTELONG,Graph<T>* >::iterator iterator;
        iterator iter =  map.begin();
            for( ; iter != map.end(); iter++)
            {
                if(visitNodeMap.contains(iter.key())) continue;
                else
                {
                    VISIOCYTELONG ind = iter.key();
                    visitNodeMap[ind] = ind;//标记为已访问
                    Graph<T>* root = iter.value();//获取这棵树的根节点
                    Tree<T>* root_tree = new Tree<T>(root->node,ind,-1);//建立树中根节点
                    treeMap[root_tree->num] = root_tree;
                    rootList.append(root_tree);//将树的根节点保存到根队列
                    /*
                     * 接下来遍历根节点的边，将其边添加进堆中
                     * 这里不可能出现某条边所连接的点已经被访问的情况
                    */
            QMap<VISIOCYTELONG,Path*>::iterator pathIter = root->connect.begin();
                    for(; pathIter != root->connect.end(); pathIter++)
                    {
                        int id = pathList.size();
                        Path* path = new Path(ind,pathIter.key(),((Path*)pathIter.value())->w);
                        pathList.append(path);
                        HeapElem* elem = new HeapElem(id,path->w);
                        heap.insert(elem);
                    }
                    if(root->connect.size() == 0) continue;
                    else
                    {
                        isBreak = false;
                    }
                    break;
                }
            }
        }
        if(isBreak || visitNodeMap.size() == map.size()) break;
        HeapElem* min_elem = heap.delete_min();
        Path* path = pathList.at(min_elem->img_ind);
        if(visitNodeMap.contains(path->src_ind) && visitNodeMap.contains(path->dst_ind))
        {
            delete min_elem;
            continue;
        }

        VISIOCYTELONG new_ind = visitNodeMap.contains(path->src_ind) ? path->dst_ind : path->src_ind;
        VISIOCYTELONG parent_ind = visitNodeMap.contains(path->src_ind) ? path->src_ind : path->dst_ind;
        visitNodeMap[new_ind] = new_ind;
        Tree<T>* child =
                new Tree<T>(map[new_ind]->node
                                ,new_ind
                                ,parent_ind
                                ,treeMap[parent_ind]);
        treeMap[new_ind] = child;
    QMap<VISIOCYTELONG,Path*>::iterator pathIter = map[new_ind]->connect.begin();
        for(; pathIter != map[new_ind]->connect.end(); pathIter++)
        {
            int id = pathList.size();
            Path* childpath = new Path(new_ind,pathIter.key(),((Path*)pathIter.value())->w);
            if(visitNodeMap.contains(childpath->src_ind) && visitNodeMap.contains(childpath->dst_ind)) continue;
            pathList.append(childpath);
            HeapElem* elem = new HeapElem(id,childpath->w);
            heap.insert(elem);
        }
        delete min_elem;
    }
}
void findNode(unsigned char* img1d, QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap,double m=0,int n1=0)
{
    for(VISIOCYTELONG i = 0; i < sz_total; i++)//把所有点选出来
    {
        if(img1d[i] > bresh)
        {
            Graph<Node*>* graphNode = new Graph<Node*>(new Node(GET_X(i),GET_Y(i),GET_Z(i)));
            nodeMap[i] = graphNode;
			n1=n1+1;
			m=m+img1d[i];
        }
    }
	mean=m/n1;//计算所选点的像素均值	
}
void findNode_sec(unsigned char* img1d, QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap,double mean,int n1)
{
	for(VISIOCYTELONG i = 0; i < n1; i++)
    {
        if(img1d[i] >mean)//大于像素均值的点提取出来
        {
            Graph<Node*>* graphNode = new Graph<Node*>(new Node(GET_X(i),GET_Y(i),GET_Z(i)));
            nodeMap[i] = graphNode;
        }
    }
}
void initPath(unsigned char* img1d,QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
  QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin();
    for(; iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* graphNode = iter.value();
        VISIOCYTELONG src = iter.key();
        VISIOCYTELONG i,j,k;
    for(i = graphNode->node->x - 1; i <= graphNode->node->x + 1; i++)
        {
            if(i < 0 || i >= sz_x) continue;
        for(j = graphNode->node->y - 1; j <= graphNode->node->y + 1; j++)
            {
                if(j < 0 || j >= sz_y) continue;
        for(k = graphNode->node->z - 1; k <= graphNode->node->z + 1; k++)
                {
                    if(k < 0 || k >= sz_z) continue;
                    VISIOCYTELONG dst = GET_IND(i,j,k);
					if(dst == src) continue;
					if(img1d[dst] < 0.1*img1d[src]) continue;//所得六邻域点，若其阈值比中心点阈值的0.3倍小，则舍弃
                    if(nodeMap.contains(dst))
                    {
                        graphNode->connect[dst] = new Path(src,dst,0);
                    }
                }
            }
        }
    }
}
void calculateRadius(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    BasicHeap<HeapElem> heap;
    QMap<VISIOCYTELONG,HeapElem*> elemMap;
    //将边界点加入到最小堆
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* graphNode = iter.value();
        VISIOCYTELONG ind = GET_IND(graphNode->node->x,graphNode->node->y,graphNode->node->z);
        VISIOCYTELONG x = graphNode->node->x;
        VISIOCYTELONG y = graphNode->node->y;
        VISIOCYTELONG z = graphNode->node->z;
        for(VISIOCYTELONG i = x - 1; i <= x + 1; i++)
        {
            if(i < 0 || i >= sz_x) continue;
            for(VISIOCYTELONG j = y - 1; j <= y + 1; j++)
            {
                if(j < 0 || j >= sz_y) continue;
                VISIOCYTELONG nearby_ind = GET_IND(i,j,z);
                if(!nodeMap.contains(nearby_ind))
                {
                    HeapElem* elem = new HeapElem(ind,1);
                    heap.insert(elem);
                    elemMap[ind] = elem;
                    nodeMap[ind]->node->r = 1;
                }
            }
        }
    }
    //遍历所有亮点，计算到边界的最短距离
    while(!heap.empty())
    {
        HeapElem* min_elem = heap.delete_min();
        VISIOCYTELONG min_ind = min_elem->img_ind;
        VISIOCYTELONG x = GET_X(min_ind);
        VISIOCYTELONG y = GET_Y(min_ind);
        VISIOCYTELONG z = GET_Z(min_ind);

        for(VISIOCYTELONG i = x - 1; i <= x + 1; i++)
        {
            if(i < 0 || i >= sz_x) continue;
            for(VISIOCYTELONG j = y - 1; j <= y + 1; j++)
            {
                if(j < 0 || j >= sz_y) continue;
                VISIOCYTELONG nearby_ind = GET_IND(i,j,z);
                if(!nodeMap.contains(nearby_ind)) continue;
                double dis = min_elem->value + 1;
                if(elemMap.contains(nearby_ind))
                {
                    if(dis < elemMap[nearby_ind]->value)
                    {
                        heap.adjust(elemMap[nearby_ind]->heap_id,dis);
                        nodeMap[nearby_ind]->node->r = dis;
                    }
                }
                else
                {
                    HeapElem* elem = new HeapElem(nearby_ind,dis);
                    heap.insert(elem);
                    elemMap[nearby_ind] = elem;
                    nodeMap[nearby_ind]->node->r = dis;
                }
            }
        }
    }
    qDeleteAll(elemMap);
}
void deleteNode(Graph<Node*>* child,Graph<Node*>* node,QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    VISIOCYTELONG child_ind = GET_IND(child->node->x,child->node->y,child->node->z);
    VISIOCYTELONG node_ind = GET_IND(node->node->x,node->node->y,node->node->z);
    for(QMap<VISIOCYTELONG,Path*>::iterator iter = child->connect.begin(); iter != child->connect.end(); iter++)
    {
        VISIOCYTELONG dst = iter.key();
        if(!nodeMap.contains(dst))
        {
            continue;
        }
        Path* deletePath = nodeMap[dst]->connect[child_ind];
        delete deletePath;
        nodeMap[dst]->connect.remove(child_ind);
        if(dst != node_ind)
        {
            node->connect[dst] = new Path(node_ind,dst,0);
            nodeMap[dst]->connect[node_ind] = new Path(node_ind,dst,0);
        }
    }
   // qDeleteAll(child->connect);
    delete child;
    nodeMap.remove(child_ind);
}
void prundNodeByRadius(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    BasicHeap<HeapElem> heap;
    VISIOCYTELONG root_ind = 0;
    double max_r = -1;
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* node = iter.value();
        HeapElem* elem = new HeapElem(iter.key(),300 - node->node->r);
        heap.insert(elem);
    }//此处只删除完全包含于圆内的点    
    while(!heap.empty())
    {

        HeapElem* elem = heap.delete_min();
        Graph<Node*>* node = nodeMap[elem->img_ind];
        if(!nodeMap.contains(elem->img_ind))
        {
            delete elem;
            continue;
        }
        if(!node)
        {
            nodeMap.remove(elem->img_ind);
            delete elem;
            continue;
        }
        for(VISIOCYTELONG i = node->node->x - node->node->r; i <= node->node->x + node->node->r && i < sz_x; i++)
        {
            if(i < 0 || i >= sz_x) continue;
            for(VISIOCYTELONG j = node->node->y - node->node->r; j <= node->node->y + node->node->r && j < sz_y; j++)
            {
                if(j < 0 || j >= sz_y) continue;
                for(VISIOCYTELONG k = node->node->z - node->node->r; k <= node->node->z + node->node->r && k < sz_z; k++)
                {
                    if(k < 0 || k >= sz_z) continue;
                    VISIOCYTELONG child_ind = GET_IND(i,j,k);
                    if(elem->img_ind == child_ind) continue;
                    if(nodeMap.contains(child_ind))
                    {
                        Graph<Node*>* child = nodeMap[child_ind];
                        float dist = DISTANCE(child_ind,elem->img_ind);
                        if(dist > node->node->r) continue;
                        //这是删除完全的内嵌点，可以毫不犹豫的直接删除
                        if(node->node->r >= child->node->r + dist)
                        {
                            deleteNode(child,node,nodeMap);
                        }
                    }
                }
            }
        }
        delete elem;

    }
}
/**
 * 目前这个方法消耗了整个方案中最多的时间，考虑看看有没有其他更快的方法
 * @brief prundNodeByRadius 根据半径删除不必要的节点
 * @param nodeMap
 */

void prundNodeByRadius_old(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    //构造最小堆，按半径升序处理
    BasicHeap<HeapElem> heap;
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > centerListMap;//每个点的中心点队列
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > seedSubNodeList;//每个点的所属点队列
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        VISIOCYTELONG ind = iter.key();
        Graph<Node*>* graphNode = iter.value();
        HeapElem* elem = new HeapElem(ind,nodeMap[ind]->node->r);
        heap.insert(elem);
        centerListMap[ind][ind] = true;
        seedSubNodeList[ind][ind] = true;

        for(VISIOCYTELONG i = graphNode->node->x - graphNode->node->r; i <= graphNode->node->x + graphNode->node->r; i++)
        {
            if(i < 0 || i >= sz_x) continue;
            for(VISIOCYTELONG j = graphNode->node->y - graphNode->node->r; j <= graphNode->node->y + graphNode->node->r; j++)
            {
                if(j < 0 || j >= sz_y) continue;
                for(VISIOCYTELONG k = graphNode->node->z - graphNode->node->r; k <= graphNode->node->z + graphNode->node->r; k++)
                {
                    if(k < 0 || k >= sz_z) continue;
                    VISIOCYTELONG dst = GET_IND(i,j,k);
                    if(nodeMap.contains(dst))
                    {
                        seedSubNodeList[ind][dst] = true;
                        centerListMap[dst][ind] = true;
                    }
                }
            }
        }
		
    }
    //这里开始计算哪些点需要删除
    QList<VISIOCYTELONG> rmList;
    while(!heap.empty())
    {
        HeapElem* elem = heap.delete_min();
        VISIOCYTELONG min_ind = elem->img_ind;
        double total = 0;
        double repeat = 0;
        bool isPrund = false;

        //判断覆盖率
        if(!isPrund)
        {
            for(QMap<VISIOCYTELONG,bool>::iterator iter = seedSubNodeList[min_ind].begin(); iter != seedSubNodeList[min_ind].end(); iter++)
            {
                total++;
                repeat += centerListMap[iter.key()].size() > 1 ? 1 : 0;
            }
            nodeMap[min_ind]->node->cover = repeat / total;
            if(repeat / total >= coverRate)
            {
                isPrund = true;
            }
        }
        if(isPrund)
        {
            rmList.append(min_ind);
            for(QMap<VISIOCYTELONG,bool>::iterator iter = seedSubNodeList[min_ind].begin(); iter != seedSubNodeList[min_ind].end(); iter++)
            {
                centerListMap[iter.key()].remove(min_ind);
            }
        }
        delete elem;
    }
	
    for(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> >::iterator iter = seedSubNodeList.begin(); iter != seedSubNodeList.end(); iter++)
    ((QMap<VISIOCYTELONG,bool>)iter.value()).clear();
     seedSubNodeList.clear();
    //删除点，并合并边
    int count = 0;
    for(int i = 0; i < rmList.size(); i++)
    {
        VISIOCYTELONG rm_ind = rmList.at(i);
        Graph<Node*>* graphNode = nodeMap[rm_ind];
        if(graphNode->connect.size() == 1)
        {
            nodeMap[graphNode->connect.begin().key()]->connect.remove(rm_ind);
        }
        else if(centerListMap[rm_ind].size() > 0) //这个点的中心点队列还有中心点存在，则把这个点的所有边转移到这个中心点上
        {
            Graph<Node*>* src = nodeMap[centerListMap[rm_ind].begin().key()];
            for(QMap<VISIOCYTELONG,Path*>::iterator iter = graphNode->connect.begin(); iter != graphNode->connect.end(); iter++)
            {
                VISIOCYTELONG dst_ind = iter.key();
                nodeMap[dst_ind]->connect.remove(rm_ind);
                VISIOCYTELONG src_ind = GET_IND(src->node->x,src->node->y,src->node->z);
                if(src_ind == dst_ind) continue;
                src->connect[dst_ind] = new Path(dst_ind,src_ind,0);
                nodeMap[dst_ind]->connect[src_ind] = src->connect[dst_ind];
            }
			
        }
		
        else //这个点不属于任何的点，则把这个点通向的点全连接
        {
            for(QMap<VISIOCYTELONG,Path*>::iterator iter1 = graphNode->connect.begin(); iter1 != graphNode->connect.end(); iter1++)
            {
                for(QMap<VISIOCYTELONG,Path*>::iterator iter2 = graphNode->connect.begin(); iter2 != graphNode->connect.end(); iter2++)
                {
                    VISIOCYTELONG src_ind = iter1.key();
                    VISIOCYTELONG dst_ind = iter2.key();
                    if(src_ind == dst_ind) continue;
                    Path* path = new Path(src_ind,dst_ind,0);
                    if(nodeMap.contains(src_ind)) nodeMap[src_ind]->connect.remove(rm_ind);
                    if(nodeMap.contains(dst_ind)) nodeMap[dst_ind]->connect.remove(rm_ind);
                    nodeMap[src_ind]->connect[dst_ind] = path;
                    nodeMap[dst_ind]->connect[src_ind] = path;
                }
            }
        }
        delete graphNode;
        nodeMap.remove(rm_ind);
    }
    rmList.clear();
    for(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> >::iterator iter = centerListMap.begin(); iter != centerListMap.end(); iter++)
        ((QMap<VISIOCYTELONG,bool>)iter.value()).clear();
    centerListMap.clear();
}

void calculateWeight(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap,QMap<VISIOCYTELONG,Node*> nodeMapCopy)
{
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* src = iter.value();
        for(QMap<VISIOCYTELONG,Path*>::iterator pathIter = src->connect.begin(); pathIter != src->connect.end(); pathIter++)
        {
            VISIOCYTELONG src_ind = iter.key();
            VISIOCYTELONG dst_ind = pathIter.key();

            VISIOCYTELONG src_x = GET_X(src_ind);
            VISIOCYTELONG src_y = GET_Y(src_ind);
            VISIOCYTELONG src_z = GET_Z(src_ind);
            VISIOCYTELONG dst_x = GET_X(dst_ind);
            VISIOCYTELONG dst_y = GET_Y(dst_ind);
            VISIOCYTELONG dst_z = GET_Z(dst_ind);

            VISIOCYTELONG begin_x = min(src_x,dst_x);
            VISIOCYTELONG end_x = max(src_x,dst_x);
            VISIOCYTELONG begin_y = min(src_y,dst_y);
            VISIOCYTELONG end_y = max(src_y,dst_y);
            VISIOCYTELONG begin_z = min(src_z,dst_z);
            VISIOCYTELONG end_z = max(src_z,dst_z);

            BasicHeap<HeapElem> heap;
            HeapElem* src_elem = new HeapElem(src_ind,0);
            heap.insert(src_elem);
            QMap<VISIOCYTELONG,HeapElem*> elemMap;
            elemMap[src_ind] = src_elem;

            bool isBreak = false;
            while(!heap.empty())
            {
                HeapElem* min_elem = heap.delete_min();
                VISIOCYTELONG min_ind = min_elem->img_ind;

                VISIOCYTELONG min_x = GET_X(min_ind);
                VISIOCYTELONG min_y = GET_Y(min_ind);
                VISIOCYTELONG min_z = GET_Z(min_ind);
				
                for(VISIOCYTELONG i = min_x - 1; i <= min_x + 1; i++)
                {
                    if(i < begin_x || i > end_x) continue;
                    for(VISIOCYTELONG j = min_y - 1; j <= min_y + 1; j++)
                    {
                        if(j < begin_y || j > end_y) continue;
                        for(VISIOCYTELONG k = min_z - 1; k <= min_z + 1; k++)
                        {
                            if(k < begin_z || k > end_z) continue;
                            VISIOCYTELONG ind = GET_IND(i,j,k);
                            double w = min_elem->value + (nodeMapCopy[ind] && nodeMapCopy[ind]->r > 0 ? 1 / nodeMapCopy[ind]->r : 1024);

							if(elemMap.contains(ind))
                            {
                                if(elemMap[ind]->value > w)
                                {
                                    heap.adjust(elemMap[ind]->heap_id,w);
                                }
                            }
                            else
                            {
                                HeapElem* elem = new HeapElem(ind,w);
                                elemMap[ind] = elem;
                                heap.insert(elem);
                            }
							
                            if(ind == dst_ind)
                            {
                                src->connect[dst_ind]->w = elemMap[ind]->value;
                                nodeMap[dst_ind]->connect[src_ind]->w = elemMap[ind]->value;
                                isBreak = true;
                            }
                        }
                    }
                }
                if(isBreak) break;
            }
            qDeleteAll(elemMap);			
        }
		
    }

}
/**
 * @brief findLeaf 计算树的叶子节点
 * @param curNode 当前遍历到的树节点
 * @param leafList 返回的叶子节点队列
 */
template<class T>
void findLeaf(Tree<T>* curNode,QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > &nodeListMap,int rootNum)
{
    if(curNode->child.size() == 0)
    {
        nodeListMap[rootNum][curNode->num] = true;
        return;
    }
    for(int i = 0; i < curNode->child.size(); i++)
    {
        Tree<T>* child = curNode->child.at(i);
        findLeaf(child,nodeListMap,rootNum);
    }
    return;
}
void findPath(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > nodeListMap,QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> &rootMap,VISIOCYTELONG src_ind,VISIOCYTELONG dst_ind)
{
    VISIOCYTELONG min_src = -1;
    VISIOCYTELONG min_dst = -1;
    double min = 100000;

    for(QMap<VISIOCYTELONG,bool>::iterator iteri = nodeListMap[src_ind].begin(); iteri != nodeListMap[src_ind].end(); iteri++)
    {
        for(QMap<VISIOCYTELONG,bool>::iterator iterj = nodeListMap[dst_ind].begin(); iterj != nodeListMap[dst_ind].end(); iterj++)
        {
            VISIOCYTELONG src_node = iteri.key();
            VISIOCYTELONG dst_node = iterj.key();

            double distance = DISTANCE(src_node,dst_node);
            if(distance < min)
            {
                min = distance;
                min_src = src_node;
                min_dst = dst_node;
            }
        }
    }
    if(min_src < 0 || min_dst < 0) return;
    Path* path = new Path(min_src,min_dst,min);
    rootMap[src_ind]->connect[dst_ind] = path;
    rootMap[dst_ind]->connect[src_ind] = path;
}

/**
 * @brief calculateTreeWeight 建立树之间的带权边，权值为欧氏距离
 * @param treeMap 树的节点映射
 * @param rootList 树的根节点队列
 * @param rootMap 树的根节点带权图
 * @param pathMap 树间边对应的原图真实边的映射
 */
void calculateTreeWeight(QList<Tree<Node*>*> rootList,QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> &rootMap)
{
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > nodeListMap;//每棵树的连接点点集//todo 需要找到每棵树的根节点和叶子节点
    for(int i = 0; i < rootList.size(); i++)
    {
        Tree<Node*>* curNode = rootList.at(i);
        findLeaf(curNode,nodeListMap,curNode->num);
        nodeListMap[curNode->num][curNode->num] = true;
        rootMap[curNode->num] = new Graph<Tree<Node*>*>(curNode);
    }

    //todo 需要对每两棵树，找到这两棵树中最近的一对连接点
    for(int i = 0; i < rootList.size() - 1; i++)
    {
        for(int j = i + 1; j < rootList.size(); j++)
        {
            findPath(nodeListMap,rootMap,rootList.at(i)->num,rootList.at(j)->num);
        }
    }
}

void reCreateTree(Tree<Node*>* curNode, QMap<VISIOCYTELONG, Tree<Node *> *> &treeMap)
{
    curNode->num = treeMap.size() + 1;
    treeMap[curNode->num] = curNode;
    for(int i = 0; i < curNode->child.size(); i++)
    {
        Tree<Node*>* child = curNode->child.at(i);
        child->parent = curNode->num;
        child->child.removeOne(curNode);
        reCreateTree(child,treeMap);
    }
    return;
}
/**
 * @brief reCreate 用于重新创建树
 * @param rootMap 树的图
 * @param rootList 树的队列
 * @param treeMap 原图树节点表
 */
void reCreate(QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> rootMap,QMap<VISIOCYTELONG,Tree<Tree<Node*>*>*> treeRootMap,QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap)
{
    Tree<Node*>* root;
    double max = -1;
    //把树重新断连接变成图
    for(QMap<VISIOCYTELONG,Tree<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
    {
        Tree<Node*>* node = iter.value();
        if(node->node->r > max)
        {
            max = node->node->r;
            root = node;
        }
        if(node->parent > 0)
        {
            node->child.append(treeMap[node->parent]);
            node->parent = -2;
        }
    }
    for(QMap<VISIOCYTELONG,Tree<Tree<Node*>*>*>::iterator iter = treeRootMap.begin(); iter != treeRootMap.end(); iter++)
    {
        Tree<Tree<Node*>*>* node = iter.value();
        for(int i = 0; i < node->child.size(); i++)
        {
            Path* path = rootMap[node->num]->connect[node->child.at(i)->num];
            treeMap[path->src_ind]->child.append(treeMap[path->dst_ind]);
            treeMap[path->dst_ind]->child.append(treeMap[path->src_ind]);
        }
    }

    //把图重新变为树
    if(!root)
    {
        printf("root is null\n");
        return;
    }
    root->parent = -1;
    treeMap.clear();
    printf("reCreateTree begin\n");
    reCreateTree(root,treeMap);
    printf("reCreateTree over\n");
}
double getArea(Tree<Node*>* treeNode)
{
    double area = treeNode->node->r;
    for(int i = 0; i < treeNode->child.size(); i++)
    {
        area += getArea(treeNode->child.at(i));
    }
    return area;
}
void updateTime()
{
    totalTimeCost = timer.elapsed();
    printf("totalTimeCost:%d\n",totalTimeCost);
    timeCost = totalTimeCost - lastTimeCost;
    lastTimeCost = totalTimeCost;
}
//APP2的平滑，搬来用用
bool smooth_curve_and_radius(std::vector<Tree<Node*>*> & mCoord, int winsize)
{
    //std::cout<<" smooth_curve ";
    if (winsize<2) return true;

    std::vector<Tree<Node*>*> mC = mCoord; // a copy
    VISIOCYTELONG N = mCoord.size();
    int halfwin = winsize/2;

    for (int i=1; i<N-1; i++) // don't move start & end point
    {
        std::vector<Tree<Node*>*> winC;
        std::vector<double> winW;
        winC.clear();
        winW.clear();

        winC.push_back( mC[i] );
        winW.push_back( 1.+halfwin );
        for (int j=1; j<=halfwin; j++)
        {
            int k1 = i+j;   if(k1<0) k1=0;  if(k1>N-1) k1=N-1;
            int k2 = i-j;   if(k2<0) k2=0;  if(k2>N-1) k2=N-1;
            winC.push_back( mC[k1] );
            winC.push_back( mC[k2] );
            winW.push_back( 1.+halfwin-j );
            winW.push_back( 1.+halfwin-j );
        }
        //std::cout<<"winC.size = "<<winC.size()<<"\n";

        double s, x,y,z,r;
        s = x = y = z = r = 0;
        for (int ii=0; ii<winC.size(); ii++)
        {
            x += winW[ii]* winC[ii]->node->x;
            y += winW[ii]* winC[ii]->node->y;
            z += winW[ii]* winC[ii]->node->z;
            r += winW[ii]* winC[ii]->node->r;
            s += winW[ii];
        }

        if (s)
        {
            x /= s;
            y /= s;
            z /= s;
            r /= s;
        }

        mCoord[i]->node->x = x; // output
        mCoord[i]->node->y = y; // output
        mCoord[i]->node->z = z; // output
        mCoord[i]->node->r = r; // output
    }
    return true;
}
//删除短小的seg
void prundSeg(Tree<Node*>* root,QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap)
{
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > leafMap;//叶子集合
    findLeaf(root,leafMap,root->num);
    QMap<VISIOCYTELONG,bool> leafs = leafMap[root->num];
    for(QMap<VISIOCYTELONG,bool>::iterator iter = leafs.begin(); iter != leafs.end(); iter++)
    {
        Tree<Node*>* node = treeMap[iter.key()];
        int count = 0;
        double cover = 0;
        double sumW = 0;
        while(node->child.size() <= 1)
        {
            count++;
            double w = node->node->r * node->node->r;
            cover += node->node->cover * w;
            sumW += w;
            node = treeMap[node->parent];
        }
        cover /= sumW;
        if(count <= 5 && cover >= 0.6)
        {
            node = treeMap[iter.key()];
            while(node->child.size() <= 1)
            {
                treeMap.remove(node->num);
                int parent = node->parent;
                delete node;
                node = treeMap[node->parent];
            }
        }
        else
        {
            node = treeMap[iter.key()];
            while(node->child.size() <= 1)
            {
                node = treeMap[node->parent];
            }
        }

    }
}
//转换成seg的数据格式，然后调用app2的平滑算法
void smooth(Tree<Node*>* root,QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap)
{
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > leafMap;//叶子集合
    findLeaf(root,leafMap,root->num);
    QMap<VISIOCYTELONG,bool> leafs = leafMap[root->num];
    QMap<int,std::vector<Tree<Node*>*> > segListMap;
    BasicHeap<HeapElem> heap;
    for(QMap<VISIOCYTELONG,bool>::iterator iter = leafs.begin(); iter != leafs.end(); iter++)
    {
        Tree<Node*>* node = treeMap[iter.key()];
        VISIOCYTELONG ind = node->num;
        do
        {
            segListMap[ind].push_back(node);
            if(node->parent < 0) break;
            node = treeMap[node->parent];
        }
        while(1);
        HeapElem* elem = new HeapElem(ind,300 - segListMap[ind].size());
        heap.insert(elem);
    }
    QMap<VISIOCYTELONG,bool> visitMap;
    int type = 2;
    while(!heap.empty())
    {
        HeapElem* min_elem = heap.delete_min();
        VISIOCYTELONG min_ind = min_elem->img_ind;
        int breakPoint = 0;
        for(int i = 0; i < segListMap[min_ind].size(); i++)
        {
            Tree<Node*>* node = segListMap[min_ind].at(i);
            if(!visitMap.contains(node->num))
            {
                visitMap[node->num] = true;
                node->node->type = type;
            }
            else
            {
                breakPoint = i;
                break;
            }
        }
        for(int i = segListMap[min_ind].size() - 1; i >= breakPoint; i--)
        {
            segListMap[min_ind].pop_back();
        }
        smooth_curve_and_radius(segListMap[min_ind],5);
        type++;
    }
}
void copyNode(QMap<VISIOCYTELONG,Graph<Node*>*> nodeMap,QMap<VISIOCYTELONG,Node*> nodeMapCopy)
{
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* node = iter.value();
        nodeMapCopy[iter.key()] = node->node;
    }
}
void prundNodeBySingle(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* node = iter.value();
        if(node->connect.size() == 0 || (node->node->r <= 1 && node->connect.size() < 2))
        {
            for(QMap<VISIOCYTELONG,Path*>::iterator iter1 = node->connect.begin(); iter1 != node->connect.end(); iter1++)
            {
                VISIOCYTELONG dst = iter1.key();
                if(!nodeMap.contains(dst))
                {
                    continue;
                }
                Path* deletePath = nodeMap[dst]->connect[iter.key()];
                delete deletePath;
                nodeMap[dst]->connect.remove(iter.key());
            }
            if(!node) delete node;
            nodeMap.remove(iter.key());
        }
    }
}
void myplugin_proc(unsigned char* img1d)
{
    printf("begin\n");//todo 预处理操作 //todo 获取前景像素点
    timer.start();
    QMap<VISIOCYTELONG,Graph<Node*>*> nodeMap;
    printf("findNode begin\n");
    findNode(img1d,nodeMap);
	findNode_sec(img1d,nodeMap,mean,n1);
    printf("findNode over\n");
    updateTime();
    //todo 将点集合编织成图
    printf("initPath begin\n");
	
	initPath(img1d,nodeMap);
    //initPath(nodeMap);
    printf("initPath over %d\n",nodeMap.size());
    updateTime();
    //todo 计算半径
    printf("calculateRadius begin\n");
    //calculateRadius_old(nodeMap)  //原来的计算半径方法，相比较而言慢很多
    calculateRadius(nodeMap);
    QMap<VISIOCYTELONG,Node*> nodeMapCopy;
    copyNode(nodeMap,nodeMapCopy);
    printf("calculateRadius over\n",nodeMap.size());
    updateTime();
    //todo 删除多余点
	printf("prundNodeByRadius begin\n");
    prundNodeByRadius(nodeMap);
    prundNodeByRadius_old(nodeMap);//2015.12.31
    prundNodeBySingle(nodeMap);
    printf("prundNodeByRadius over %d\n",nodeMap.size());
    updateTime();


    //todo 计算边权值
    printf("calculateWeight begin\n");
    calculateWeight(nodeMap,nodeMapCopy);
    printf("calculateWeight over\n");
    updateTime();
    //todo 生成每个连通域的最小生成树
    QList<Tree<Node*>*> rootList;//保存树根节点的队列
    QMap<VISIOCYTELONG,Tree<Node*>*> treeMap;//保存树上节点的图
    printf("mst begin\n");
    mst(nodeMap,rootList,treeMap);
    printf("mst over %d %d\n",rootList.size(),treeMap.size());
    updateTime();
    QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> rootMap;//存放树连通关系的图
    //todo 计算树间权值
    printf("calculateTreeWeight begin\n");
    calculateTreeWeight(rootList,rootMap);
    printf("calculateTreeWeight over\n");
    updateTime();
    //todo 生成最小生成树的最小生成树
    QList<Tree<Tree<Node*>*>*> treeRootList;
    QMap<VISIOCYTELONG,Tree<Tree<Node*>*>*> treeRootMap;
    printf("mst begin\n");
    mst(rootMap,treeRootList,treeRootMap);
    printf("mst over\n");
    updateTime();
    //todo 对最小生成树的最小生成树减枝
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > childMap;
    findLeaf(treeRootList.at(0),childMap,treeRootList.at(0)->num);
    for(QMap<VISIOCYTELONG,bool>::iterator childIter = childMap[treeRootList.at(0)->num].begin();
        childIter != childMap[treeRootList.at(0)->num].end();
        childIter++)
    {
        VISIOCYTELONG root_ind = childIter.key();
        double area = getArea(treeRootMap[root_ind]->node);
        if(area < 2)
        {
            treeRootMap[treeRootMap[root_ind]->parent]->child.removeOne(treeRootMap[root_ind]);
        }
    }
    if(treeRootList.at(0)->child.size() == 1)
    {
        if(getArea(treeRootList.at(0)->node) < 2)
        {
            treeRootList.at(0)->child.at(0)->parent = -1;
            treeRootMap.remove(treeRootList.at(0)->num);
            treeRootList.removeFirst();
        }
    }
    childMap.clear();

    //todo 以半径最大节点起，重新构建树
    printf("reCreate begin\n");
    reCreate(rootMap,treeRootMap,treeMap);
    printf("reCreate over\n");
    updateTime();
    //todo 减去毛刺
    findLeaf(treeMap[1],childMap,treeMap[1]->num);
    for(QMap<VISIOCYTELONG,bool>::iterator childIter = childMap[treeMap[1]->num].begin();
        childIter != childMap[treeMap[1]->num].end();
        childIter++)
    {
        VISIOCYTELONG ind = childIter.key();
        Tree<Node*>* node = treeMap[ind];
        Tree<Node*>* parent = treeMap[node->parent];
        for(int i = 0; i < parent->child.size(); i++)
        {
            if(parent->child.at(i)->num != node->num)
            {
                if(parent->child.at(i)->child.size() > 1)
                {
                    parent->child.removeOne(node);
                    break;
                }
            }
        }
    }
    
    Tree<Node*>* root = treeMap[1];
    treeMap.clear();
    reCreateTree(root,treeMap);
	
    //todo 做平滑
    smooth(root,treeMap);
    prundSeg(root,treeMap);
	printf("reconstruction over\n");
    //todo 输出swc
    printSWC(treeMap,fileName + "_fastmarching_spanningtree.swc");
    printf("over\n");
}
void printSWC(QMap<VISIOCYTELONG,Tree<Node*>*> treeMap,string fileName)
{
    FILE* swc = fopen(fileName.data(),"wt");

    fprintf(swc, "#name\n");
    fprintf(swc, "#comment\n");
    fprintf(swc,"##timecost:%d\n",totalTimeCost);
    fprintf(swc, "##n,type,x,y,z,radius,parent\n");
	
	map <int, int> m1;//生成一个字典，存储每个节点的序号
	int j=1;//行数
    for(QMap<VISIOCYTELONG,Tree<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
    {
        Tree<Node*>* node = iter.value();
		m1[node->num]=j;	
		j=j+1;
    }
	//fprintf(swc,"%ld \n",m1[366]);
          
	
    int s;
    int i=1;//每个节点的序号变为0,1,2,3...，根节点序号根据字典来查找
    for(QMap<VISIOCYTELONG,Tree<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
	{ 
		Tree<Node*>* node = iter.value();
		node->num=i;
		++i;
		for ( map<int ,int >::iterator iter = m1.begin( ); iter != m1.end( ); iter++ ) 
		   //if(node->parent<0)continue;
		   s = m1[node->parent]; 
		   node->parent=s;
		
				        
	}
	for(QMap<VISIOCYTELONG,Tree<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
	{
		Tree<Node*>* node = iter.value();
		if(node->parent<=0)node->parent=-1;	
				        
	}
			
	for(QMap<VISIOCYTELONG,Tree<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
	{
		Tree<Node*>* node = iter.value();
		fprintf(swc,"%ld %d %5.3f %5.3f %5.3f %5.3f %1d \n",
                   node->num,
                   node->node->type,
                   node->node->x,
                   node->node->y,
                   node->node->z,
                   node->node->r - 0.5,
                   node->parent
		           );      
	 }

    fclose(swc);
    return;
}
bool proc(VISIOCYTEPluginCallback2 &callback,QWidget* parent,unsigned char* data1d,VISIOCYTELONG* in_sz,QString inimg_file)
{

    sz_x = in_sz[0];
    sz_y = in_sz[1];
    sz_z = in_sz[2];

    sz_xy = sz_x * sz_y;
    sz_total = sz_xy * sz_z;

    
    coverRate = 1;
    fileName = inimg_file.toStdString();
    myplugin_proc(data1d);
    return 1;
}
