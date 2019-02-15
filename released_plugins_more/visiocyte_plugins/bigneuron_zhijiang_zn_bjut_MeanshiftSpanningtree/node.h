#ifndef NODE_H
#define NODE_H
#include <visiocyte_interface.h>
class Node
{
public:
    VISIOCYTELONG x;//x轴坐标
    VISIOCYTELONG y;//y轴坐标
    VISIOCYTELONG z;//z轴坐标
    VISIOCYTELONG ind;//在hashmap中的key
    VISIOCYTELONG parent;//父亲节点
    VISIOCYTELONG number;//swc中编号
	int containByroot;//flag用于判断是否被其他的根节点包含
	double intensity;
    VISIOCYTELONG class_mark;//类别标记,用自己的序号做标记
    QMap<VISIOCYTELONG,Node*> connect;//连接点集合
    VISIOCYTELONG count;
    double r;
public:
    Node();


	Node(VISIOCYTELONG i,VISIOCYTELONG j,VISIOCYTELONG k)
	{
		x=i;
		y=j;
		z=k;
	};
};

#endif // NODE_H


