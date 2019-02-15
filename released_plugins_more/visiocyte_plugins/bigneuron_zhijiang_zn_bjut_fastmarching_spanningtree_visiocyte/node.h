#ifndef NODE_H
#define NODE_H
#include <visiocyte_interface.h>

class Node
{
public:
    double x;
    double y;
    double z;
    double r;
    double cover;
    int type;
public:
    Node(double x,double y,double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->r = 0;
        this->type = 0;
        this->cover = 0;
    }
};


#endif // NODE_H
