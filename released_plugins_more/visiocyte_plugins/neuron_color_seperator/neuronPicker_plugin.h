/* neuronPicker_plugin.h
 * 
 * 2014-12-01 : by Xiang Li, Hanbo Chen
 */
 
#ifndef __NEURONPICKER_PLUGIN_H__
#define __NEURONPICKER_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>
#include <vector>
#include "neuronpicker_func.h"

using namespace std;

#define NAME_INWIN "Input_Neuron_Picker"
#define NAME_OUTWIN "Output_Neuron_Picker"

class neuronPicker : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	bool interface_run(VISIOCYTEPluginCallback2 &_VISIOCYTEPluginCallback2_currentCallback, QWidget *_QWidget_parent);
	

    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

private:
    void printHelp();
};


class neuronPickerDialog : public QDialog
{
    Q_OBJECT

public:
    neuronPickerDialog(VISIOCYTEPluginCallback2 * cb);
    ~neuronPickerDialog();
    void shiftTwoBits2UINT8(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
    void convert2UINT8(unsigned short *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);
    void convert2UINT8(float *pre1d, unsigned char *pPost, VISIOCYTELONG imsz);

private:
    VISIOCYTEPluginCallback2 * callback;

    neuronPickerMain2 pickerObj;
    QString fname_previnput;
    unsigned char *image1Dc_in;
    unsigned char *image1Dc_out;
    VISIOCYTELONG sz_img[4];
    VISIOCYTELONG sz_out[4];
    int intype;
    LandmarkList LList;
    vector<VISIOCYTELONG> poss_landmark;
    VISIOCYTELONG pos_out;

private:
    void creat();
    void checkButtons();
    void initDlg();
    void updateInputWindow();
    void updateOutputWindow();

public slots:
    bool load();
    void output();
    int loadMarkers();
    void extract();
    void syncMarkers(int c);
    void saveFile();
    void skip();
    void reject();
    void runall();
    void autoSeeds();

public:
    QComboBox *cb_marker;
    QPushButton *btn_update, *btn_extract, *btn_save, *btn_next, *btn_quit, *btn_load, *btn_output, *btn_runall, *btn_autoMarkers;
    QSpinBox *spin_distance, *spin_bgthr, *spin_conviter, *spin_fgthr, *spin_sizethr, *spin_sizemargin;
    QLineEdit *edit_load, *edit_output;
    QDoubleSpinBox *spin_rate;
};


#endif

