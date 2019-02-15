#ifndef FILTER_DIALOG_H
#define FILTER_DIALOG_H

#include <QtGui>
#include <visiocyte_interface.h>
#include <vector>

void ComputemaskImage(NeuronTree neurons,unsigned char* pImMask,VISIOCYTELONG sx,VISIOCYTELONG sy,VISIOCYTELONG sz, double margin=0);
void BoundNeuronCoordinates(NeuronTree & neuron, double & output_xmin,double & output_xmax,
                            double & output_ymin,double & output_ymax,double & output_zmin,double & output_zmax);
QHash<VISIOCYTELONG, VISIOCYTELONG> NeuronNextPn(const NeuronTree &neurons);
void convert2UINT8(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
void convert2UINT8(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);

class filter_dialog:public QDialog
{
    Q_OBJECT

public:
    filter_dialog(VISIOCYTEPluginCallback2 *cb);
    VISIOCYTEPluginCallback2 *callback;
    void swc_filter_image();

private:
    void create();
    VISIOCYTELONG sz_img[4];
    int intype;
    unsigned char *image_data;
    NeuronTree neuron;
    QGridLayout *mygridLayout;
    QLineEdit *edit_load,*edit_swc;

public slots:
    void load_swc();
    void loadImage();
    void dialoguefinish(int);

};

#endif // FILTER_DIALOG_H
