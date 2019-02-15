#ifndef SOMA_H
#define SOMA_H
#include "ineurondataobject.h"
#include <vector>
#define nullptr 0 
struct Cell{
    Cell(){}
    ~Cell(){}
    Cell(int i, double xx, double yy, double zz, double rr=1, double aa=1, double bb=1, double mrr=1,
         double ss=1, double vv=1):ID(i), x(xx), y(yy), z(zz), r(rr), a(aa), b(bb), mr(mrr), s(ss),
        v(vv){}
    int ID;
    double x, y, z, r, a, b, mr, s, v;//λ��x,y,z���뾶r������a������b, ƽ���뾶�����s�����v
};

class Soma : public INeuronDataObject
{
public:
    Soma(INeuronProcessObject* p = nullptr);
    bool IsEmpty()const;
//    void SetProcessObject(INeuronProcessObject*);
//    void ReleaseProcessObject();
    void push_back(const Cell&);
    void clear(){m_Source.clear();}
    const Cell& GetCell(int idx)const{return m_Source[idx];}
    //bool empty()const{return m_Source.empty();}
    int size()const {return m_Source.size();}
private:
    std::vector<Cell> m_Source;
    Soma(const Soma&);//uncopyable
    Soma& operator=(const Soma&);
};

#endif // SOMA_H
