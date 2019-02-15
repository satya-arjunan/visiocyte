#ifndef _POPULATE_NEURON_H_
#define _POPULATE_NEURON_H_



#include <visiocyte_interface.h>



struct Point3D{
    float x;
    float y;
    float z;
};

struct NeuronSize{
    float x;
    float y;
    float z;
};

struct MyBoundingBox{
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
};

VISIOCYTELONG randomPosition1D(VISIOCYTELONG range);
NeuronTree populate_transform (NeuronTree sampleNeuron,VISIOCYTELONG x, VISIOCYTELONG y, VISIOCYTELONG z, float rotation_z );

void removeSubtree(NeuronTree &nt, int nodeIndex);
void prune_by_boundingbox (NeuronTree &nt, VISIOCYTELONG siz_x,  VISIOCYTELONG siz_y,  VISIOCYTELONG siz_z);
//void removeSubTree( QMutableListIterator<NeuronSWC> iter, int &currentIdx );
QList<NeuronTree>  populate_neurons(NeuronTree sampleNeuron,int num_neurons,float maxRotation, VISIOCYTELONG siz_x,  VISIOCYTELONG siz_y,  VISIOCYTELONG siz_z);

QList<ImageMarker> detect_pairwise_contacts(const NeuronTree treeA, const NeuronTree treeB, int type1, int type2,
                                            float closeness,VISIOCYTEPluginCallback2 &callback);
QList<ImageMarker> detect_contacts(QList<NeuronTree> neuronTreeList, int type1, int type2,float closeness,
                                   VISIOCYTEPluginCallback2 &callback);

void label_neighborhood(unsigned char * img1d,VISIOCYTELONG tol_sz, VISIOCYTELONG sz_x, VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,
                         Point3D center, int typeValue, float closeness);
void label_image_by_type(unsigned char * img1d, VISIOCYTELONG tol_sz, VISIOCYTELONG sz_x, VISIOCYTELONG sz_y,VISIOCYTELONG sz_z,
                        Point3D offset, NeuronTree nt, int type,  int typeValue);


QList<ImageMarker> merge_contacts (const NeuronTree treeA, const NeuronTree treeB, QList<ImageMarker> detected_contacts,
                 float closeness,VISIOCYTEPluginCallback2 &callback);
#endif // _POPULATE_NEURON_H_



