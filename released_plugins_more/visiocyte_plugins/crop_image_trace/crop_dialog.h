#ifndef CROP_DIALOG_H
#define CROP_DIALOG_H

#include <QtGui>
#include "visiocyte_interface.h"
#include "vector"

using namespace std;

bool load_data(VISIOCYTEPluginCallback2 *cb, unsigned char * & image1Dc_in, ImagePixelType &pixeltype,
               VISIOCYTELONG sz_img[4], NeuronTree &neuron, visiocytehandle &curwin);
void backupNeuron(const NeuronTree & source, const NeuronTree & backup);
vector<VISIOCYTELONG> pos2xyz(const VISIOCYTELONG _pos_input, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);
VISIOCYTELONG xyz2pos(const VISIOCYTELONG _x, const VISIOCYTELONG _y, const VISIOCYTELONG _z, const VISIOCYTELONG _offset_Y, const VISIOCYTELONG _offset_Z);

class crop_dialog:public QDialog
{
     Q_OBJECT
public:
    crop_dialog(VISIOCYTEPluginCallback2 *cb);
    ~crop_dialog();
    void getData();

private:
    bool check_channel();
    bool check_button();
    void initDlg();

public slots:
    void crop();
    bool csv_out();

private:
    VISIOCYTEPluginCallback2 *callback;
    unsigned char *image1Dc_in;
    VISIOCYTELONG sz_img[4];
    visiocytehandle curwin;
    NeuronTree neuron;
    QString folder_output;
    QDialog *mydialog2;
    QComboBox *channel_menu;
    QPushButton *ok;
    QLineEdit *edit_csv;
    QSpinBox *spin_x_min,*spin_y_min,*spin_z_min,*spin_x_max,*spin_y_max,*spin_z_max;
};

#endif // CROP_DIALOG_H
