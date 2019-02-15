#include <iostream>
#include <stdlib.h>
#include <map>
#include "myplugin_func.h"
#include "global.h"
#include "node.h"
#include "tree.h"
#include "graph.h"
#include "../../APP2_ported/heap.h"
#include "visiocyte_message.h"
#include "stackutil.h"
#include "tree.h"
#include "graph.h"
#include <visiocyte_interface.h>

const QString title="MyPlugin";


void printSWC(QMap<VISIOCYTELONG,Tree<Node*>*> treeMap,string fileName)
{
    FILE* swc = fopen(fileName.data(),"wt");

    fprintf(swc, "#name\n");
    fprintf(swc, "#comment\n");
    fprintf(swc,"##timecost:%d\n",totalTimeCost);
    fprintf(swc, "##n,type,x,y,z,radius,parent\n");

    for(QMap<VISIOCYTELONG,Tree<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
    {
        Tree<Node*>* node = iter.value();
        fprintf(swc,"%ld %d %5.3f %5.3f %5.3f %5.3f %ld\n",
                node->num,
                (int)(node->node->r + 0.5),
                node->node->x,
                node->node->y,
                node->node->z,
                node->node->r - 0.5,
                node->parent);
    }
    fclose(swc);
    return;
}
void printSWC(QMap<VISIOCYTELONG,Graph<Node*>*> treeMap,string fileName)
{
    FILE* swc = fopen(fileName.data(),"wt");

    fprintf(swc, "#name\n");
    fprintf(swc, "#comment\n");
    fprintf(swc,"##timecost:%d\n",totalTimeCost);
    fprintf(swc, "##n,type,x,y,z,radius,parent\n");
    int i = 0;
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = treeMap.begin(); iter != treeMap.end(); iter++)
    {
        Graph<Node*>* node = iter.value();
        fprintf(swc,"%ld %d %5.3f %5.3f %5.3f %5.3f %ld\n",
                ++i,
                (int)(node->node->r + 0.5),
                node->node->x,
                node->node->y,
                node->node->z,
                node->node->r > 0 ? node->node->r : 0.5,
                -1);
    }
    fclose(swc);
    return;
}
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


void findNode(unsigned char* img1d, QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    for(VISIOCYTELONG i = 0; i < sz_total; i++)
    {
        if(img1d[i] > bresh)
        {
            Graph<Node*>* graphNode = new Graph<Node*>(new Node(GET_X(i),GET_Y(i),GET_Z(i)));
            nodeMap[i] = graphNode;
        }
    }
}
void initPath(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
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

void calculateRadius_old(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
{
    for(QMap<VISIOCYTELONG,Graph<Node*>*>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++)
    {
        Graph<Node*>* node = iter.value();
        VISIOCYTELONG key = iter.key();

        BasicHeap<HeapElem> heap;
        HeapElem* nodeElem = new HeapElem(key,0);
        QMap<VISIOCYTELONG,HeapElem*> elemMap;
        elemMap[key] = nodeElem;
        heap.insert(nodeElem);

        double total = 1;
        double bg = 0;
        while(!heap.empty())
        {
            HeapElem* elem = heap.delete_min();
            VISIOCYTELONG min_ind = elem->img_ind;
            if(node->node->r != 0 && elem->value > node->node->r - 1) break;
            VISIOCYTELONG min_x = GET_X(min_ind);
            VISIOCYTELONG min_y = GET_Y(min_ind);
            VISIOCYTELONG min_z = GET_Z(min_ind);
            bool isBreak = false;
            for(VISIOCYTELONG i = min_x - 1; i <= min_x + 1; i++)
            {
                if(i < 0 || i >= sz_x || isBreak) continue;
                for(VISIOCYTELONG j = min_y - 1; j <= min_y + 1; j++)
                {
                    if(j < 0 || j >= sz_y || isBreak) continue;
                    int offset = ABS(i - min_x) + ABS(j - min_y);
                    if(offset >= 2 || offset == 0) continue;
                    VISIOCYTELONG nearby_ind = GET_IND(i,j,min_z);
                    if(elemMap.contains(nearby_ind))//是否已经在堆中，或者已经遍历到
                    {
                        HeapElem* nearby_elem = elemMap[nearby_ind];
                        if(nearby_elem->value > elem->value + 1)
                        {
                            heap.adjust(nearby_elem->heap_id,elem->value + 1);
                        }
                    }
                    else
                    {
                        total++;
                        bg += nodeMap.contains(nearby_ind) ? 0 : 1;
                        HeapElem* nearby_elem = new HeapElem(nearby_ind,elem->value + 1);
                        heap.insert(nearby_elem);
                        elemMap[nearby_ind] = nearby_elem;
                        if(bg > 0)
                        {
                            node->node->r = elem->value + 1;
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
/**
 * 目前这个方法消耗了整个方案中最多的时间，考虑看看有没有其他更快的方法
 * @brief prundNodeByRadius 根据半径删除不必要的节点
 * @param nodeMap
 */
void prundNodeByRadius(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
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
        //在这之前，可以先判断其周围是否有能完全包含他的节点，若有则可以直接跳过覆盖率判断
/*
        Graph<Node*>* node = nodeMap[min_ind];
        for(QMap<VISIOCYTELONG,Path*>::iterator iter = node->connect.begin(); iter != node->connect.end(); iter++)
        {
            Path* path = iter.value();
            VISIOCYTELONG dst_ind = path->getDst(min_ind);
            Graph<Node*>* otherNode = nodeMap[dst_ind];
            if(otherNode->node->r > node->node->r + 1)
            {
                isPrund = true;
                break;
            }
        }
*/
        //判断覆盖率
        if(!isPrund)
        {
            for(QMap<VISIOCYTELONG,bool>::iterator iter = seedSubNodeList[min_ind].begin(); iter != seedSubNodeList[min_ind].end(); iter++)
            {
                total++;
                repeat += centerListMap[iter.key()].size() > 1 ? 1 : 0;
            }
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
void calculateWeight(QMap<VISIOCYTELONG,Graph<Node*>*> &nodeMap)
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
                            double w = (nodeMap.contains(ind) ? 0 : 1024) + min_elem->value + 1;
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
void findPath(QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > nodeListMap
              ,QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> &rootMap,
              VISIOCYTELONG src_ind,VISIOCYTELONG dst_ind)
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
void calculateTreeWeight(QList<Tree<Node*>*> rootList
                         ,QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> &rootMap)
{
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > nodeListMap;//每棵树的连接点点集
    //todo 需要找到每棵树的根节点和叶子节点
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
void reCreate(QMap<VISIOCYTELONG,Graph<Tree<Node*>*>*> rootMap
                ,QMap<VISIOCYTELONG,Tree<Tree<Node*>*>*> treeRootMap
                ,QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap)
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
//转换成seg的数据格式，然后调用app2的平滑算法
void smooth(Tree<Node*>* root,QMap<VISIOCYTELONG,Tree<Node*>*> &treeMap)
{
    QMap<VISIOCYTELONG,QMap<VISIOCYTELONG,bool> > leafMap;//叶子集合
    findLeaf(root,leafMap,root->num);
    QMap<VISIOCYTELONG,bool> leafs = leafMap[root->num];
    QMap<VISIOCYTELONG,std::vector<Tree<Node*>*> > segList;
    BasicHeap<HeapElem> heap;
    for(QMap<VISIOCYTELONG,bool>::iterator iter = leafs.begin(); iter != leafs.end(); iter++)
    {
        VISIOCYTELONG ind = iter.key();
        Tree<Node*>* node = treeMap[ind];
        while(node->parent > 0)
        {
            segList[ind].push_back(node);
            node = treeMap[node->parent];
        }
        HeapElem* elem = new HeapElem(ind,1000 - segList[ind].size());
        heap.insert(elem);
    }
    QMap<VISIOCYTELONG,bool> isVisit;
    while(!heap.empty())
    {
        HeapElem* elem = heap.delete_min();
        VISIOCYTELONG ind = elem->img_ind;
        for(int i = segList[ind].size() - 1; i >= 0; i--)
        {
            VISIOCYTELONG num = segList[ind].at(i)->num;
            if(isVisit.contains(num)) segList[ind].pop_back();
            else isVisit[i] = true;
        }
        smooth_curve_and_radius(segList[ind],5);
    }
}
void myplugin_proc(unsigned char* img1d)
{
    printf("begin\n");
    //todo 预处理操作
    //todo 获取前景像素点
    timer.start();
    QMap<VISIOCYTELONG,Graph<Node*>*> nodeMap;
    printf("findNode begin\n");
    findNode(img1d,nodeMap);
    printf("findNode over\n");
    updateTime();
    printSWC(nodeMap,fileName + "_findNode.swc");
    //todo 将点集合编织成图
    printf("initPath begin\n");
    initPath(nodeMap);
    printf("initPath over %d\n",nodeMap.size());
    updateTime();
    //todo 计算半径
    printf("calculateRadius begin\n");
    //calculateRadius_old(nodeMap)  //原来的计算半径方法，相比较而言慢很多
    calculateRadius(nodeMap);
    printf("calculateRadius over\n",nodeMap.size());
    updateTime();
    printSWC(nodeMap,fileName + "_calculateRadius.swc");
    //todo 删除多余点
    printf("prundNodeByRadius begin\n");
    prundNodeByRadius(nodeMap);
    printf("prundNodeByRadius over %d\n",nodeMap.size());
    updateTime();
    printSWC(nodeMap,fileName + "_prundByRadius.swc");
    //todo 计算边权值
    printf("calculateWeight begin\n");
    calculateWeight(nodeMap);
    printf("calculateWeight over\n");
    updateTime();
    //todo 生成每个连通域的最小生成树
    QList<Tree<Node*>*> rootList;//保存树根节点的队列
    QMap<VISIOCYTELONG,Tree<Node*>*> treeMap;//保存树上节点的图
    printf("mst begin\n");
    mst(nodeMap,rootList,treeMap);
    printf("mst over %d %d\n",rootList.size(),treeMap.size());
    updateTime();
    printSWC(treeMap,fileName + "_init.swc");
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
    printSWC(treeMap,fileName + "_beforeSmooth.swc");
    //todo 做平滑
    smooth(root,treeMap);
    //todo 输出swc
    printSWC(treeMap,fileName + "_finish.swc");
    printf("over\n");

}
int proc(VISIOCYTEPluginCallback2 &callback,QWidget* parent)
{

    visiocytehandle curwin = callback.currentImageWindow();

    if(!curwin)
    {
            QMessageBox::information(0, title, QObject::tr("No image is open."));
            return -1;
    }
    Image4DSimple *p4d = callback.getImage(curwin);

    unsigned char* img1d = p4d->getRawDataAtChannel(0);
    sz_x = p4d->getXDim();
    sz_y = p4d->getYDim();
    sz_z = p4d->getZDim();
    sz_xy = sz_x * sz_y;
    sz_total = sz_xy * sz_z;
    bresh = 0;
    coverRate = 1;
    fileName = p4d->getFileName();
    fileName = fileName.substr(fileName.find_last_of("/") + 1, fileName.size());

    myplugin_proc(img1d);
}



