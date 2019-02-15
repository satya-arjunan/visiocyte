#ifndef NEURON_TILE_DISPLAY_DIALOG_H
#define NEURON_TILE_DISPLAY_DIALOG_H
#include <QtGui>
#include <visiocyte_interface.h>
#include <vector>

using namespace std;

class NeuronTileDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    NeuronTileDisplayDialog(VISIOCYTEPluginCallback2 * callback, VisiocyteR_MainWindow* visiocytewin);
    void enterEvent(QEvent *e);

public slots:
    void tile();
    void tile(double dis);
    void slot_reset();
    void reject();

protected:
    void reset();
    void checkwindow();

public:
//    QCheckBox *check_boundbox;
//    QcheckBox *check_hide;
    QDoubleSpinBox *spin_x;
    QDoubleSpinBox *spin_y;
    QDoubleSpinBox *spin_z;
    QPushButton *btn_quit;
    QPushButton *btn_tile;
    QPushButton *btn_reset;

private:
    QList<NeuronTree> * ntList;
    double cur_x, cur_y, cur_z;
    int cur_num;
    VisiocyteR_MainWindow * visiocytewin;
    VISIOCYTEPluginCallback2 * callback;
};


class NeuronXYTileDialog : public QDialog
{
    Q_OBJECT

public:
    NeuronXYTileDialog(VISIOCYTEPluginCallback2 * callback, VisiocyteR_MainWindow* visiocytewin);
    void enterEvent(QEvent *e);

public slots:
    void tile();
    void slot_reset();
    void reject();

protected:
    void reset();
    void checkwindow();

public:
//    QCheckBox *check_boundbox;
//    QcheckBox *check_hide;
    QDoubleSpinBox *spin_x;
    QDoubleSpinBox *spin_y;
    QDoubleSpinBox *spin_ratial;
    QPushButton *btn_quit;
    QPushButton *btn_tile;
    QPushButton *btn_reset;

private:
    QList<NeuronTree> * ntList;
    vector<double> cur_x, cur_y;
    int cur_num;
    VisiocyteR_MainWindow * visiocytewin;
    VISIOCYTEPluginCallback2 * callback;
};


class NeuronTileGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    NeuronTileGroupsDialog(VISIOCYTEPluginCallback2 * callback, VisiocyteR_MainWindow* visiocytewin);
    void enterEvent(QEvent *e);

public slots:
    void tile();
    void slot_reset();
    void reject();

protected:
    void reset();
    void checkwindow();

public:
//    QCheckBox *check_boundbox;
//    QcheckBox *check_hide;
    QDoubleSpinBox *spin_x;
    QDoubleSpinBox *spin_y;
    QDoubleSpinBox *spin_z;
    QPushButton *btn_quit;
    QPushButton *btn_tile;
    QPushButton *btn_reset;
    QRadioButton *radio_x;
    QRadioButton *radio_y;
    QRadioButton *radio_z;

    void setAnoFileList(QStringList anofilelist){
        this->ano_file_list = anofilelist;
    }

    QStringList getAnoFileList(){
        return (this->ano_file_list);
    }

private:
    QList<NeuronTree> * ntList;
    QStringList  ano_file_list;

    vector<double> cur_x, cur_y;
    int cur_num;

    VisiocyteR_MainWindow * visiocytewin;
    VISIOCYTEPluginCallback2 * callback;
};






#endif // NEURON_TILE_DISPLAY_DIALOG_H
