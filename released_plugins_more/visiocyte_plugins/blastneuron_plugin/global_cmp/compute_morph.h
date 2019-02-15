#ifndef COMPUTE_H
#define COMPUTE_H

#include <QtGlobal>
#include "basic_surf_objs.h" 

void computeFeature(const NeuronTree & nt, double * features);
QVector<VISIOCYTELONG> getRemoteChild(int t);
void computeLinear(const NeuronTree & nt);
void computeTree(const NeuronTree & nt);
#endif
