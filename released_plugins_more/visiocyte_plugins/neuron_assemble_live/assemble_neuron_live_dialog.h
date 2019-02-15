#ifndef ASSEMBEL_NEURON_LIVE_DIALOG_H
#define ASSEMBEL_NEURON_LIVE_DIALOG_H

#ifndef __ALLOW_VR_FUNCS__
#define __ALLOW_VR_FUNCS__
#endif

#include <QDialog>
#include <QtGui>
#include "visiocyte_interface.h"
#include <map>
#include "../terastitcher/src/core/imagemanager/VirtualVolume.h"

#define WINNAME_ASSEM "assemble_swc_file"

using namespace std;
using namespace iim;

typedef struct NOI{
    VISIOCYTELONG n; //the id of noi, match with the n variable of node
    VISIOCYTELONG cid; //fragment component id
    int type; //type information
    float x, y, z;		// point coordinates
    union{
    float r;			// radius
    float radius;
    };
    QList<float> fea_val; //feature values
    QSet<NOI *> conn; //connections from NOI
    long seg_id;
    long level;
}NOI;

typedef QHash<VISIOCYTELONG, NOI*>::iterator NodeIter;

class assemble_neuron_live_dialog : public QDialog
{
    Q_OBJECT
public:
    assemble_neuron_live_dialog(VISIOCYTEPluginCallback2 * callback, QList<NeuronTree> &ntList, Image4DSimple * p_img4d,
                                LandmarkList marklist, QWidget *parent = 0);

    VISIOCYTEPluginCallback2 * callback;
    NeuronTree nt;
    NeuronTree nt_original;
    LandmarkList marklist;

    VISIOCYTELONG prev_root;
    QHash<VISIOCYTELONG, NOI*> nodes;
    int coffset;
    VISIOCYTELONG noffset;
    Image4DSimple * p_img4d;

    QString winname_main, winname_3d, winname_roi,terafly_folder;
    
    QPushButton * btn_link, *btn_loop, *btn_manuallink, *btn_deletelink, *btn_connect, *btn_connectall,
        *btn_syncmarker, *btn_break, * btn_save, * btn_quit, *btn_zoomin, *btn_syncmarkeronly,
        *btn_findtips, *btn_synctips, *btn_savetips, *btn_updatetips;
    QTabWidget * tab;
    QListWidget * list_edge, * list_link, *list_marker, *list_tips;
    QComboBox * cb_color;
    QCheckBox * check_loop, * check_zoomin;
    QSpinBox * spin_zoomin;
    VirtualVolume* dataTerafly;

    QStringList list_tips_information;

    VISIOCYTELONG x_min, x_max, y_min, y_max, z_min, z_max; //ROI window


signals:
    
public slots:
    void setColor(int i);
    void syncMarker();
    void syncMarkerOnly();
    void pairMarker();
    void delPair();
    void connectPair();
    void connectAll();
    void breakEdge();
    void searchPair();
    void searchLoop();
    void sortsaveSWC();
    void highlightPair();
    void highlightEdge();
    void zoomin();
    void findTips();
    void syncTips();
    void saveTips();
    void updateTips();


private:
    void creat(QWidget *parent);
    void initNeuron(QList<NeuronTree> &ntList_in);
    bool connectNode(VISIOCYTELONG pid1, VISIOCYTELONG pid2);
    bool breakNode(VISIOCYTELONG pid1, VISIOCYTELONG pid2);
    QSet<QPair<VISIOCYTELONG, VISIOCYTELONG> > searchConnection(double xscale, double yscale, double zscale,
                                                    double angThr, double disThr, int matchType,
                                                    bool b_minusradius);
    double getNeuronDiameter();
    LandmarkList * getMarkerList();
    void updateDisplay(); //deep refresh everything
    void update3DView(); //light refresh 3D window
    visiocytehandle getImageWindow(); //return the handle if window is open, otherwise 0
    visiocytehandle checkImageWindow(); //if window not opened, open it. return the handle
    visiocytehandle updateImageWindow(); //deep refresh image window
    VisiocyteR_MainWindow * get3DWindow(); //return the handle if window is open, otherwise 0
    VisiocyteR_MainWindow * check3DWindow(); //if window not opened, open it. return the handle
    VisiocyteR_MainWindow * update3DWindow(); //deep refresh 3D window
    void updateROIWindow(const QList<VISIOCYTELONG>& pids);
    void updateColor(); //update the neuron type, will sync neuron if changed
};

class pair_marker_dialog : public QDialog
{
    Q_OBJECT
public:
    pair_marker_dialog(LandmarkList * mList,QWidget *parent = 0);
    QListWidget * list1, * list2;
    QPushButton * btn_yes, *btn_no;
};

class connect_param_dialog : public QDialog
{
    Q_OBJECT

public:
    connect_param_dialog();
    ~connect_param_dialog();

private:
    void creat();
    void initDlg();

public slots:
    void myconfig();

public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *spin_zscale, *spin_xscale, *spin_yscale, *spin_ang, *spin_dis;
    QPushButton *btn_quit, *btn_run;
    QComboBox *cb_distanceType, *cb_matchType, *cb_conf;
    QTextEdit* text_info;
};

class sort_neuron_dialog : public QDialog
{
    Q_OBJECT
public:
    sort_neuron_dialog(LandmarkList * mList, VISIOCYTELONG prev_root, VISIOCYTELONG type1_root, QWidget *parent = 0);
    QComboBox * cb_marker;
    QPushButton * btn_yes, *btn_no;
    QRadioButton *radio_marker, *radio_type, *radio_prev;
};

#define NTDIS(a,b) (((a).x-(b).x)*((a).x-(b).x)+((a).y-(b).y)*((a).y-(b).y)+((a).z-(b).z)*((a).z-(b).z))
#define NTDOT(a,b) ((a).x*(b).x+(a).y*(b).y+(a).z*(b).z)
#define NTNORM(a) (sqrt((a).x*(a).x+(a).y*(a).y+(a).z*(a).z))

void update_marker_info(const LocationSimple& mk, QList<int>& info); //info[0]=point id, info[0]=matching point id
void update_marker_info(const LocationSimple& mk, QList<int>& info, int* color);
void set_marker_color(const LocationSimple& mk, RGB8 color);
bool get_marker_info(const LocationSimple& mk, QList<int>& info);
QList<NeuronSWC> generate_swc_typesort(QHash<VISIOCYTELONG, NOI*>& nodes, VISIOCYTELONG n_root);
bool export_list2file(const QList<NeuronSWC>& lN, QString fileSaveName);
QList<NOI *> search_loop(QHash<VISIOCYTELONG, NOI*>& nodes);

#endif // ASSEMBEL_NEURON_LIVE_DIALOG_H
