/****************************************************************************
**
** neuron_match_clique.cpp
** by Hanbo Chen 2014.10.16
**
****************************************************************************/

#include "neuron_match_clique.h"
#include "../../../visiocyte_main/neuron_editing/neuron_xforms.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "marker_match_dialog.h"
#include "performance_timer.h"

#define CANDMS_ENTRY(a,b) (candMS[(a)+(b)*MS_x])
#define _TESTFLAG 0
#define WINNAME_LOCAL "neuron_stitcher_zoom_view"

#define _checkwindow() \
    if (!callback){ \
        this->hide(); \
        return; \
    }\
    bool isclosed = true;\
    QList <VisiocyteR_MainWindow *> allWindowList = callback->getListAll3DViewers(); \
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++) \
    {\
        if(allWindowList.at(i)==visiocytewin){\
            isclosed = false;\
            break;\
        }\
    }\
    if(isclosed){\
        this->hide();\
        return;\
    }

int COLOR_ACTIVE0=4;
int COLOR_ACTIVE1=5;
int COLOR_ORG0=2;
int COLOR_ORG1=7;
int COLOR_STITCHED0=12;
int COLOR_STITCHED1=12;


NeuronMatchOnlyDialog::NeuronMatchOnlyDialog(NeuronTree *nt_in0, NeuronTree *nt_in1, LandmarkList *mList_in, double affineParam[7])
{
    nt1=nt_in1;
    nt0=nt_in0;
    mList=mList_in;
    affine=affineParam;

    creat();
}

void NeuronMatchOnlyDialog::creat()
{
    gridLayout = new QGridLayout();

    //matching zone
    cb_dir = new QComboBox(); cb_dir->addItem("x"); cb_dir->addItem("y"); cb_dir->addItem("z");
    cb_dir->setCurrentIndex(2);
    spin_zscale = new QDoubleSpinBox();
    spin_zscale->setRange(0,100000); spin_zscale->setValue(1);
    spin_ang = new QDoubleSpinBox();
    spin_ang->setRange(0,180); spin_ang->setValue(60);
    spin_matchdis = new QDoubleSpinBox();
    spin_matchdis->setRange(0,100000); spin_matchdis->setValue(100);
    spin_searchspan = new QDoubleSpinBox();
    spin_searchspan->setRange(0,100000); spin_searchspan->setValue(20);
    spin_cmatchdis = new QDoubleSpinBox();
    spin_cmatchdis->setRange(0,100000); spin_cmatchdis->setValue(100);
    spin_segthr = new QDoubleSpinBox();
    spin_segthr->setRange(0,100000); spin_segthr->setValue(0);
    spin_maxcnum = new QSpinBox();
    spin_maxcnum->setRange(0,1e10); spin_maxcnum->setValue(1000);
    spin_spineLen = new QSpinBox();
    spin_spineLen->setRange(0,100000); spin_spineLen->setValue(5); spin_spineLen->setEnabled(false);
    spin_spineAng = new QDoubleSpinBox();
    spin_spineAng->setRange(0,180); spin_spineAng->setValue(30); spin_spineAng->setEnabled(false);
    spin_spineRadius = new QDoubleSpinBox();
    spin_spineRadius->setRange(0,100000); spin_spineRadius->setValue(3); spin_spineRadius->setEnabled(false);
    check_spine = new QCheckBox("filter spines when matching:");
    check_spine -> setChecked(false);
    connect(check_spine, SIGNAL(stateChanged(int)), this, SLOT(spineCheck(int)));
    QLabel* label_0 = new QLabel("stacking direction: ");
    gridLayout->addWidget(label_0,9,0,1,2);
    gridLayout->addWidget(cb_dir,9,2,1,1);
    QLabel* label_1 = new QLabel("resacle stacking direction: ");
    gridLayout->addWidget(label_1,9,3,1,2);
    gridLayout->addWidget(spin_zscale,9,5,1,1);
    QLabel* label_2 = new QLabel("Max angular to match points (0~180): ");
    gridLayout->addWidget(label_2,10,0,1,2);
    gridLayout->addWidget(spin_ang,10,2,1,1);
    QLabel* label_3 = new QLabel("Max distance to match points: ");
    gridLayout->addWidget(label_3,10,3,1,2);
    gridLayout->addWidget(spin_matchdis,10,5,1,1);
    QLabel* label_4 = new QLabel("match candidates searching span: ");
    gridLayout->addWidget(label_4,11,0,1,2);
    gridLayout->addWidget(spin_searchspan,11,2,1,1);
    QLabel* label_5 = new QLabel("Max distance to match 3-clique: ");
    gridLayout->addWidget(label_5,11,3,1,2);
    gridLayout->addWidget(spin_cmatchdis,11,5,1,1);
    QLabel* label_6 = new QLabel("fragment filter (0=keep all): ");
    gridLayout->addWidget(label_6,12,0,1,2);
    gridLayout->addWidget(spin_segthr,12,2,1,1);
    QLabel* label_61 = new QLabel("Max number of triangles (smaller, faster): ");
    gridLayout->addWidget(label_61,12,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_maxcnum,12,5,1,1);
    gridLayout->addWidget(check_spine,13,0,1,2);
    QLabel* label_7 = new QLabel("segment point #:");
    gridLayout->addWidget(label_7,14,0,1,1,Qt::AlignRight);
    gridLayout->addWidget(spin_spineLen,14,1,1,1);
    QLabel* label_8 = new QLabel("turning angle:");
    gridLayout->addWidget(label_8,14,2,1,1,Qt::AlignRight);
    gridLayout->addWidget(spin_spineAng,14,3,1,1);
    QLabel* label_9 = new QLabel("radius:");
    gridLayout->addWidget(label_9,14,4,1,1,Qt::AlignRight);
    gridLayout->addWidget(spin_spineRadius,14,5,1,1);

    btn_match = new QPushButton("Match"); btn_match->setAutoDefault(false);
    connect(btn_match, SIGNAL(clicked()), this, SLOT(match()));
    gridLayout->addWidget(btn_match,15,5,1,1);

    setLayout(gridLayout);
}

void NeuronMatchOnlyDialog::match()
{
    gridLayout->setEnabled(false);
    NeuronTree nt0_run,nt1_run;
    backupNeuron(*nt0,nt0_run);
    backupNeuron(*nt1,nt1_run);
    //init matching function
    neuron_match_clique matchfunc(&nt0_run,&nt1_run);

    //parameters
    matchfunc.direction=cb_dir->currentIndex();
    matchfunc.zscale=spin_zscale->value();
    matchfunc.angThr_match=cos(spin_ang->value()/180*M_PI);
    matchfunc.pmatchThr = spin_matchdis->value();
    matchfunc.spanCand = spin_searchspan->value();
    matchfunc.cmatchThr = spin_cmatchdis->value();
    matchfunc.segmentThr = spin_segthr->value();
    matchfunc.maxClique3Num = spin_maxcnum->value();
    if(check_spine->isChecked()){
        matchfunc.spineLengthThr = spin_spineLen->value();
        matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
        matchfunc.spineRadiusThr = spin_spineRadius->value();
    }else{
        matchfunc.spineLengthThr = 0;
        matchfunc.spineRadiusThr = 0;
    }

    //init clique and candidate
    matchfunc.init();

    //global match
    qDebug()<<"start global search";
    matchfunc.globalmatch();

    //get parameter
    affine[0]=matchfunc.shift_x;
    affine[1]=matchfunc.shift_y;
    affine[2]=matchfunc.shift_z;
    affine[3]=matchfunc.rotation_ang;
    affine[4]=matchfunc.rotation_cx;
    affine[5]=matchfunc.rotation_cy;
    affine[6]=matchfunc.rotation_cz;

    //populate markers
    matchfunc.update_matchedPoints_to_Markers(mList);

    //quit
    accept();
}

void NeuronMatchOnlyDialog::spineCheck(int c)
{
    if(check_spine->isChecked()){
        spin_spineLen->setEnabled(true);
        spin_spineAng->setEnabled(true);
        spin_spineRadius->setEnabled(true);
    }else{
        spin_spineLen->setEnabled(false);
        spin_spineAng->setEnabled(false);
        spin_spineRadius->setEnabled(false);
    }
}

NeuronLiveMatchDialog::NeuronLiveMatchDialog(VISIOCYTEPluginCallback2 * cb, VisiocyteR_MainWindow* inwin)
{
    if(!inwin)
        return;
    visiocytewin=inwin;
    callback = cb;
    visiocytecontrol = callback->getView3DControl_Any3DViewer(visiocytewin);

    p_img0=0;
    p_img1=0;

    ntList=cb->getHandleNeuronTrees_Any3DViewer(visiocytewin);
    if(ntList->size()!=2){ //this should not happen
        visiocyte_msg("The number of neurons in the window is not 2");
        return;
    }

    //back up type
    backup_swc_type();

    //back up
    for(int i=0; i<2; i++){
        NeuronTree nt_tmp;
        backupNeuron(ntList->at(i),nt_tmp);
        ntList_bk.append(nt_tmp);
    }

    //creat dialog
    creat();

    mList = cb->getHandleLandmarkList_Any3DViewer(visiocytewin);

    matchfunc = new neuron_match_clique((NeuronTree*)(&ntList->at(0)),(NeuronTree*)(&ntList->at(1)));
    matchfunc->initNeuronComponents();

    //update color;
    change_color();

    //update markers
    link_new_marker_neuron();
    updatematchlist();

    updateview();
}

void NeuronLiveMatchDialog::enterEvent(QEvent *e)
{
    checkwindow();

    QDialog::enterEvent(e);
}


void NeuronLiveMatchDialog::creat()
{
    gridLayout = new QGridLayout();

    //matching zone
    QLabel* label_a = new QLabel("Step 1: match and affine ");
    gridLayout->addWidget(label_a,8,0,1,5);
    btn_match = new QPushButton("Match"); btn_match->setAutoDefault(false);
    connect(btn_match, SIGNAL(clicked()), this, SLOT(match()));
    gridLayout->addWidget(btn_match,8,5,1,1);

    cb_dir = new QComboBox(); cb_dir->addItem("x"); cb_dir->addItem("y"); cb_dir->addItem("z");
    cb_dir->setCurrentIndex(2);
    spin_zscale = new QDoubleSpinBox();
    spin_zscale->setRange(0,100000); spin_zscale->setValue(1);
    spin_ang = new QDoubleSpinBox();
    spin_ang->setRange(0,180); spin_ang->setValue(91);
    spin_matchdis = new QDoubleSpinBox();
    spin_matchdis->setRange(0,100000); spin_matchdis->setValue(100);
    spin_cmatchdis = new QDoubleSpinBox();
    spin_cmatchdis->setRange(0,100000); spin_cmatchdis->setValue(100);
    spin_maxcnum = new QSpinBox();
    spin_maxcnum->setRange(0,1e10); spin_maxcnum->setValue(1000);
    check_type = new QCheckBox("match by type defined in SWC file");

    QLabel* label_0 = new QLabel("stacking direction: ");
    gridLayout->addWidget(label_0,9,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(cb_dir,9,2,1,1);
    QLabel* label_1 = new QLabel("resacle stacking direction: ");
    gridLayout->addWidget(label_1,9,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_zscale,9,5,1,1);
    QLabel* label_2 = new QLabel("Max angular to match points (0~180): ");
    gridLayout->addWidget(label_2,10,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_ang,10,2,1,1);
    QLabel* label_3 = new QLabel("Max distance to match points: ");
    gridLayout->addWidget(label_3,10,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_matchdis,10,5,1,1);
    QLabel* label_5 = new QLabel("Max distance to match triangles: ");
    gridLayout->addWidget(label_5,11,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_cmatchdis,11,2,1,1);
    QLabel* label_61 = new QLabel("Max number of triangles to match: ");
    gridLayout->addWidget(label_61,11,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_maxcnum,11,5,1,1);
    gridLayout->addWidget(check_type,12,0,1,3,Qt::AlignRight);

    //border tips zone
    group_marker = new QGroupBox("search for border tips, otherwise use existing ones:");
    group_marker->setCheckable(true);
    group_marker->setChecked(true);
    spin_searchspan = new QDoubleSpinBox();
    spin_searchspan->setRange(0,100000); spin_searchspan->setValue(20);
    spin_segthr = new QDoubleSpinBox();
    spin_segthr->setRange(0,100000); spin_segthr->setValue(0);
    spin_gapthr = new QDoubleSpinBox();
    spin_gapthr->setRange(0,100000); spin_gapthr->setValue(0);
    spin_spineLen = new QSpinBox();
    spin_spineLen->setRange(0,100000); spin_spineLen->setValue(5); spin_spineLen->setEnabled(false);
    spin_spineAng = new QDoubleSpinBox();
    spin_spineAng->setRange(0,180); spin_spineAng->setValue(30); spin_spineAng->setEnabled(false);
    spin_spineRadius = new QDoubleSpinBox();
    spin_spineRadius->setRange(0,100000); spin_spineRadius->setValue(3); spin_spineRadius->setEnabled(false);
    check_spine = new QCheckBox("filter spines when matching:");
    check_spine -> setChecked(false);
    connect(check_spine, SIGNAL(stateChanged(int)), this, SLOT(spineCheck(int)));

    QGridLayout* groupLayout = new QGridLayout();
    QLabel* label_4 = new QLabel("match candidates searching span: ");
    groupLayout->addWidget(label_4,13,0,1,2,Qt::AlignRight);
    groupLayout->addWidget(spin_searchspan,13,2,1,1);
    QLabel* label_62 = new QLabel("small gap filter (gap size): ");
    groupLayout->addWidget(label_62,13,3,1,2,Qt::AlignRight);
    groupLayout->addWidget(spin_gapthr,13,5,1,1);
    QLabel* label_6 = new QLabel("fragment filter (0=keep all): ");
    groupLayout->addWidget(label_6,14,0,1,2,Qt::AlignRight);
    groupLayout->addWidget(spin_segthr,14,2,1,1);
    groupLayout->addWidget(check_spine,15,0,1,2);
    QLabel* label_7 = new QLabel("segment point #:");
    groupLayout->addWidget(label_7,16,0,1,1,Qt::AlignRight);
    groupLayout->addWidget(spin_spineLen,16,1,1,1);
    QLabel* label_8 = new QLabel("turning angle:");
    groupLayout->addWidget(label_8,16,2,1,1,Qt::AlignRight);
    groupLayout->addWidget(spin_spineAng,16,3,1,1);
    QLabel* label_9 = new QLabel("radius:");
    groupLayout->addWidget(label_9,16,4,1,1,Qt::AlignRight);
    groupLayout->addWidget(spin_spineRadius,16,5,1,1);
    group_marker->setLayout(groupLayout);
    gridLayout->addWidget(group_marker, 13,0,4,6);

    //stitching zone
    int stitchrow=17;
    QFrame *line_1 = new QFrame();
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line_1,stitchrow,0,1,6);
    QLabel* label_b = new QLabel("Step 2: stitch paired points");
    gridLayout->addWidget(label_b,stitchrow+1,0,1,5);
    cb_pair = new QComboBox();
    gridLayout->addWidget(cb_pair,stitchrow+2,0,1,2);
    connect(cb_pair, SIGNAL(currentIndexChanged(int)), this, SLOT(change_pair(int)));
    btn_manualmatch = new QPushButton("Manually Add"); btn_manualmatch->setAutoDefault(false);
    connect(btn_manualmatch, SIGNAL(clicked()), this, SLOT(manualadd()));
    gridLayout->addWidget(btn_manualmatch,stitchrow+2,2,1,1);
    btn_skip = new QPushButton("Skip"); btn_skip->setAutoDefault(false);
    connect(btn_skip, SIGNAL(clicked()), this, SLOT(skip()));
    gridLayout->addWidget(btn_skip,stitchrow+2,3,1,1);
    btn_stitch = new QPushButton("Stitch"); btn_stitch->setAutoDefault(false);
    connect(btn_stitch, SIGNAL(clicked()), this, SLOT(stitch()));
    gridLayout->addWidget(btn_stitch,stitchrow+2,4,1,1);
    btn_stitchall = new QPushButton("Stitch All"); btn_stitchall->setAutoDefault(false);
    connect(btn_stitchall, SIGNAL(clicked()), this, SLOT(stitchall()));
    gridLayout->addWidget(btn_stitchall,stitchrow+2,5,1,1);

    btn_stitch->setEnabled(false);
    btn_stitchall->setEnabled(false);
    btn_skip->setEnabled(false);

    QGroupBox* group_localview = new QGroupBox("zoom-in view the pair to stitch:");
    QGridLayout* localviewLayout = new QGridLayout();
    check_localview = new QCheckBox("Auto Launch Local View");
    check_localview->setChecked(true);
    btn_localview = new QPushButton("Launch Local View");
    btn_loadimage0 = new QPushButton("Bottom Section");
    btn_loadimage1 = new QPushButton("Top Section");
    spin_localview = new QSpinBox(); spin_localview->setRange(0,1000); spin_localview->setValue(50);

    QLabel* label_localview = new QLabel("Caution: zoom in view is only for visual inspection. For manual editing, please operate in the original 3D view.");
    localviewLayout->addWidget(label_localview,0,0,1,5);
    localviewLayout->addWidget(check_localview,1,0,1,2);
    localviewLayout->addWidget(btn_localview,1,2,1,1);
    QLabel* label_localviewmargin = new QLabel("Window margin:");
    localviewLayout->addWidget(label_localviewmargin, 1,3,1,1, Qt::AlignRight);
    localviewLayout->addWidget(spin_localview,1,4,1,1);
    QLabel* label_loadimage = new QLabel("Load the 3D image of the section to display:");
    localviewLayout->addWidget(label_loadimage,2,0,1,2);
    localviewLayout->addWidget(btn_loadimage0,2,2,1,1);
    localviewLayout->addWidget(btn_loadimage1,2,3,1,1);
    group_localview->setLayout(localviewLayout);
    gridLayout->addWidget(group_localview, stitchrow+3,0,4,6);

    connect(btn_localview,SIGNAL(clicked()), this, SLOT(updatelocalview()));
    connect(btn_loadimage0,SIGNAL(clicked()), this, SLOT(load_img0()));
    connect(btn_loadimage1,SIGNAL(clicked()), this, SLOT(load_img1()));

    //operation zone
    int optrow=25;
    QFrame *line_2 = new QFrame();
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line_2,optrow,0,1,6);
    cb_color = new QComboBox();
    cb_color->addItem("stitch result");
    cb_color->addItem("swc type");
    QLabel* label_color = new QLabel("Color neuron by:");
    gridLayout->addWidget(label_color,optrow+1,0,1,1,Qt::AlignRight);
    gridLayout->addWidget(cb_color,optrow+1,1,1,1);
    connect(cb_color, SIGNAL(currentIndexChanged(int)), this, SLOT(change_color()));
    btn_output = new QPushButton("Save"); btn_output->setAutoDefault(false);
    gridLayout->addWidget(btn_output,optrow+1,4,1,1);
    connect(btn_output,     SIGNAL(clicked()), this, SLOT(output()));
    btn_quit = new QPushButton("Quit"); btn_quit->setAutoDefault(false);
    connect(btn_quit,     SIGNAL(clicked()), this, SLOT(reject()));
    gridLayout->addWidget(btn_quit,optrow+1,5,1,1);

    setLayout(gridLayout);
}

void NeuronLiveMatchDialog::reject()
{
    _checkwindow();

    setback_swc_type();

    QDialog::reject();
}


void NeuronLiveMatchDialog::spineCheck(int c)
{
    if(check_spine->isChecked()){
        spin_spineLen->setEnabled(true);
        spin_spineAng->setEnabled(true);
        spin_spineRadius->setEnabled(true);
    }else{
        spin_spineLen->setEnabled(false);
        spin_spineAng->setEnabled(false);
        spin_spineRadius->setEnabled(false);
    }
}

void NeuronLiveMatchDialog::checkwindow()
{
    _checkwindow();
}

void NeuronLiveMatchDialog::updatelocalview()
{
    //check callback
    if (!callback){
        this->hide();
        return;
    }

    //check combobox
    if(cb_pair->count()<=0){
        return;
    }

    //prepair data for the window
    //calculate neuron offset
    int idx=cb_pair->currentIndex();
    VISIOCYTELONG winsize[4], winoffset[3];
    int winmargin = spin_localview->value();
    const NeuronSWC * S0, * S1;
    S0=&(ntList->at(0).listNeuron.at(pmatch0.at(idx)));
    S1=&(ntList->at(1).listNeuron.at(pmatch1.at(idx)));
    winsize[0]=fabs(S0->x-S1->x)+winmargin;
    winsize[1]=fabs(S0->y-S1->y)+winmargin;
    winsize[2]=fabs(S0->z-S1->z)+winmargin;
    winsize[3]=2;

    winoffset[0]=winsize[0]/2;
    winoffset[1]=winsize[1]/2;
    winoffset[2]=winsize[2]/2;

    float x_offset, y_offset, z_offset;
    x_offset=-(S0->x+S1->x)/2+winoffset[0];
    y_offset=-(S0->y+S1->y)/2+winoffset[1];
    z_offset=-(S0->z+S1->z)/2+winoffset[2];

    //image
    Image4DSimple * tmp_image = new Image4DSimple();
    unsigned char * p_img = new unsigned char[winsize[0]*winsize[1]*winsize[2]*winsize[3]];
    memset(p_img,0,winsize[0]*winsize[1]*winsize[2]*winsize[3]);
    //bottom section
    VISIOCYTELONG pagesize=winsize[0]*winsize[1]*winsize[2];
    if(p_img0!=0){
        for(VISIOCYTELONG sx=0; sx<winsize[0]; sx++){
            VISIOCYTELONG ox=sx-x_offset;
            if(ox>=size_img0[0] || ox<0) continue;
            for(VISIOCYTELONG sy=0; sy<winsize[1]; sy++){
                VISIOCYTELONG oy=sy-y_offset;
                if(oy>=size_img0[1] || oy<0) continue;
                for(VISIOCYTELONG sz=0; sz<winsize[2]; sz++){
                    VISIOCYTELONG oz=sz-z_offset;
                    if(oz>=size_img0[2] || oz<0) continue;
                    p_img[sx+sy*winsize[0]+sz*winsize[0]*winsize[1]+pagesize] =
                            p_img0[ox+oy*size_img0[0]+oz*size_img0[0]*size_img0[1]];
                }
            }
        }
    }
    //top section
    double amat[16], amat_inv[16];
    getAffineAmat(amat, matchfunc->shift_x, matchfunc->shift_y, matchfunc->shift_z, matchfunc->rotation_ang,
                  matchfunc->rotation_cx, matchfunc->rotation_cy, matchfunc->rotation_cz, matchfunc->direction);
    inverseAmat(amat, amat_inv);
    if(p_img1!=0){
        for(VISIOCYTELONG sx=0; sx<winsize[0]; sx++){
            VISIOCYTELONG tx=sx-x_offset;
            for(VISIOCYTELONG sy=0; sy<winsize[1]; sy++){
                VISIOCYTELONG ty=sy-y_offset;
                for(VISIOCYTELONG sz=0; sz<winsize[2]; sz++){
                    VISIOCYTELONG tz=sz-z_offset;
                    VISIOCYTELONG ox, oy, oz;
                    ox=amat_inv[0]*tx+amat_inv[1]*ty+amat_inv[2]*tz+amat_inv[3];
                    oy=amat_inv[4]*tx+amat_inv[5]*ty+amat_inv[6]*tz+amat_inv[7];
                    oz=amat_inv[8]*tx+amat_inv[9]*ty+amat_inv[10]*tz+amat_inv[11];
                    if(oz>=size_img1[2] || oz<0 || ox>=size_img1[0] || ox<0 || oy>=size_img1[1] || oy<0) continue;
                    p_img[sx+sy*winsize[0]+sz*winsize[0]*winsize[1]] =
                            p_img1[ox+oy*size_img1[0]+oz*size_img1[0]*size_img1[1]];
                }
            }
        }
    }
    tmp_image->setFileName(WINNAME_LOCAL);
    tmp_image->setData(p_img, winsize[0], winsize[1], winsize[2], winsize[3], (ImagePixelType)1);

    //neuron SWC
    NeuronTree local_nt;
    local_nt.editable=ntList->at(0).editable;
    local_nt.file="";
    local_nt.linemode=ntList->at(0).linemode;
    local_nt.n=ntList->at(0).n;
    local_nt.on=ntList->at(0).on;
    local_nt.selected=ntList->at(0).selected;
    local_nt.name=WINNAME_LOCAL;
    local_nt.listNeuron.clear();
    local_nt.hashNeuron.clear();
    local_nt.color.r = local_nt.color.g = local_nt.color.b = local_nt.color.a = 0;
    VISIOCYTELONG nmax=0;
    VISIOCYTELONG noffset=0;
    for(int ntid=0; ntid<2; ntid++){
        const NeuronTree* source = &(ntList->at(ntid));
        for(VISIOCYTELONG i=0; i<source->listNeuron.size(); i++)
        {
            NeuronSWC S;
            S.n = source->listNeuron[i].n+noffset;
            S.type = source->listNeuron[i].type;
            S.x = source->listNeuron[i].x+x_offset-1;
            S.y = source->listNeuron[i].y+y_offset-1;
            S.z = source->listNeuron[i].z+z_offset-1;
            S.r = source->listNeuron[i].r;
            S.pn = source->listNeuron[i].pn;
            if(S.pn>=0) S.pn+=noffset;
            local_nt.listNeuron.append(S);
            local_nt.hashNeuron.insert(S.n, local_nt.listNeuron.size()-1);
            nmax=MAX(nmax,S.n);
        }
        noffset=nmax+1;
    }

    //markers
    LandmarkList local_landmark;
    for(int i=0; i<mList->size(); i++){
        LocationSimple SP;
        SP.x=mList->at(i).x+x_offset;
        SP.y=mList->at(i).y+y_offset;
        SP.z=mList->at(i).z+z_offset;
        SP.color.r=mList->at(i).color.r;
        SP.color.g=mList->at(i).color.g;
        SP.color.b=mList->at(i).color.b;
        SP.color.a=mList->at(i).color.a;
        SP.comments=mList->at(i).comments;
        SP.name=mList->at(i).name;
        local_landmark.append(SP);
    }

    //push object into the window
    //locate 3d window
    visiocytehandleList allWindowList = callback->getImageWindowList();
    visiocytehandle localwin = 0;
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++)
    {
        if(callback->getImageName(allWindowList.at(i)).contains(WINNAME_LOCAL)){
            localwin = allWindowList[i];
            break;
        }
    }
    if(localwin == 0){
        localwin = callback->newImageWindow(WINNAME_LOCAL);
        callback->setImage(localwin, tmp_image);
        callback->setLandmark(localwin, local_landmark);
//        callback->setSWC(localwin, local_nt);
        callback->updateImageWindow(localwin);
        callback->open3DWindow(localwin);
        callback->pushObjectIn3DWindow(localwin);

        QList<NeuronTree> * local_ntList = callback->getHandleNeuronTrees_3DGlobalViewer(localwin);
        local_ntList->clear();
        local_ntList->push_back(local_nt);
        VisiocyteR_MainWindow * local3dwin = callback->find3DViewerByName(WINNAME_LOCAL);
        if(local3dwin)
            callback->update_3DViewer(local3dwin);

        qDebug()<<"LiveNeuronStitcher: launch local view for pair: "<<idx;
    }else{
        callback->setImage(localwin, tmp_image);
        callback->setLandmark(localwin, local_landmark);
//        callback->setSWC(localwin, local_nt);
        callback->updateImageWindow(localwin);
        callback->open3DWindow(localwin);
        callback->pushImageIn3DWindow(localwin);
        callback->pushObjectIn3DWindow(localwin);

//        View3DControl * local3dcontrol = callback->getView3DControl(localwin);
//        local3dcontrol->cancelSelect();

        QList<NeuronTree> * local_ntList = callback->getHandleNeuronTrees_3DGlobalViewer(localwin);
        local_ntList->clear();
        local_ntList->push_back(local_nt);
        VisiocyteR_MainWindow * local3dwin = callback->find3DViewerByName(WINNAME_LOCAL);
        if(local3dwin)
            callback->update_3DViewer(local3dwin);

        qDebug()<<"LiveNeuronStitcher: update local view for pair: "<<idx;
    }

}

void NeuronLiveMatchDialog::updateview()
{
    _checkwindow();

    if(visiocytewin){
        callback->update_3DViewer(visiocytewin);
    }

    if(visiocytecontrol){
        visiocytecontrol->updateLandmark();
    }
}

void NeuronLiveMatchDialog::match()
{
    _checkwindow();

    //retrive backup
    if(!group_marker->isChecked()){
        link_new_marker_neuron();
    }
    for(int i=0; i<2; i++){
        copyCoordinate(ntList_bk.at(i),ntList->at(i));
        copyProperty(ntList_bk.at(i),ntList->at(i));
    }
    if(!group_marker->isChecked()){
        update_marker_to_neuron();
    }

    updateview();

    matchfunc = new neuron_match_clique((NeuronTree*)(&ntList->at(0)),(NeuronTree*)(&ntList->at(1)));

    //parameters
    matchfunc->direction=cb_dir->currentIndex();
    matchfunc->zscale=spin_zscale->value();
    matchfunc->angThr_match=cos(spin_ang->value()/180*M_PI);
    matchfunc->pmatchThr = spin_matchdis->value();
    matchfunc->spanCand = spin_searchspan->value();
    matchfunc->cmatchThr = spin_cmatchdis->value();
    matchfunc->segmentThr = spin_segthr->value();
    matchfunc->gapThr = spin_gapthr->value();
    matchfunc->maxClique3Num = spin_maxcnum->value();
    if(check_spine->isChecked()){
        matchfunc->spineLengthThr = spin_spineLen->value();
        matchfunc->spineAngThr = cos(spin_spineAng->value()/180*M_PI);
        matchfunc->spineRadiusThr = spin_spineRadius->value();
    }else{
        matchfunc->spineLengthThr = 0;
        matchfunc->spineRadiusThr = 0;
    }
    matchfunc->typeConstrain = check_type->isChecked();

    //clean up dialog storage:
    pmatch0.clear();
    pmatch1.clear();
    mmatch0.clear();
    mmatch1.clear();
    stitchmask.clear();
    cur_pair=-1;

    //init clique and candidate
    if(group_marker->isChecked()){ //search for border tips
        mList->clear(); //clean up
        matchfunc->init();
    }else{//use exsiting marker as border tips for match
        matchfunc->init(mList);
        //matchfunc->initNeuronComponents();
    }

    //global match
    qDebug()<<"start global search";
    matchfunc->globalmatch();

    //update markers
    matchfunc->update_matchedPoints_to_Markers(mList);
    qDebug()<<"update makers: "<<mList->size();

    cur_pair=0;

    callback->update_NeuronBoundingBox(visiocytewin);

    updateview();
    updatematchlist();

    change_color();
}

void NeuronLiveMatchDialog::updatematchlist()
{
    QList<int> mm0, mm1;
    pmatch0.clear();
    pmatch1.clear();
    mmatch0.clear();
    mmatch1.clear();
    stitchmask.clear();
    cb_pair->clear();
    int info[4];

    for(int i=0; i<mList->size(); i++){
        if(get_marker_info(mList->at(i),info)){
            if(info[2]>=0){ //matched marker
                if(info[2]>i && info[2]<mList->size()){
                    if(info[0]==0){
                        mm0.append(i);
                        mm1.append(info[2]);
                    }else{
                        mm0.append(info[2]);
                        mm1.append(i);
                    }
                }
            }
        }
    }
    for(int i=0; i<mm0.size(); i++){
        //get the matched point on neuron
        get_marker_info(mList->at(mm0[i]),info);
        if(info[1]>=ntList->at(0).listNeuron.size()) continue;
        int tmp=info[1];
        get_marker_info(mList->at(mm1[i]),info);
        if(info[1]>=ntList->at(1).listNeuron.size()) continue;
        pmatch0.append(tmp);
        pmatch1.append(info[1]);
        mmatch0.append(mm0[i]);
        mmatch1.append(mm1[i]);

        //check loop or sititched
        if(fabs(ntList->at(0).listNeuron.at(pmatch0.last()).x-ntList->at(1).listNeuron.at(pmatch1.last()).x)<1 &&
                fabs(ntList->at(0).listNeuron.at(pmatch0.last()).y-ntList->at(1).listNeuron.at(pmatch1.last()).y)<1 &&
                fabs(ntList->at(0).listNeuron.at(pmatch0.last()).z-ntList->at(1).listNeuron.at(pmatch1.last()).z)<1)
            stitchmask.append(1);
        else if(matchfunc->checkloop(pmatch0.last(), pmatch1.last()))
            stitchmask.append(-1); //cannot stitch
        else
            stitchmask.append(0);

        if(stitchmask.last()<0)
            cb_pair->addItem("Marker: " + QString::number(mm0[i]+1) + " x Marker: " + QString::number(mm1[i]+1) +" = LOOP!");
        else if(stitchmask.last()==0)
            cb_pair->addItem("Marker: " + QString::number(mm0[i]+1) + " x Marker: " + QString::number(mm1[i]+1) );
        else
            cb_pair->addItem("Marker: " + QString::number(mm0[i]+1) + " x Marker: " + QString::number(mm1[i]+1) +" = stiched!");

    }

    if(pmatch0.size()>0){
        int id=cb_pair->currentIndex();
        btn_stitchall->setEnabled(true);
        btn_skip->setEnabled(true);
        btn_stitch->setEnabled(stitchmask.at(id)>=0);
        cur_pair=id;
    }
}

void NeuronLiveMatchDialog::backup_swc_type()
{
    swcType0.clear();
    swcType1.clear();
    for(VISIOCYTELONG i=0; i<ntList->at(0).listNeuron.size(); i++){
        swcType0.append(ntList->at(0).listNeuron.at(i).type);
    }
    for(VISIOCYTELONG i=0; i<ntList->at(1).listNeuron.size(); i++){
        swcType1.append(ntList->at(1).listNeuron.at(i).type);
    }
}

void NeuronLiveMatchDialog::setback_swc_type()
{
    for(int i=0; i<ntList->at(0).listNeuron.size(); i++){
        NeuronSWC *p = (NeuronSWC *)&(ntList->at(0).listNeuron.at(i));
        p->type=swcType0.at(i);
    }
    for(int i=0; i<ntList->at(1).listNeuron.size(); i++){
        NeuronSWC *p = (NeuronSWC *)&(ntList->at(1).listNeuron.at(i));
        p->type=swcType1.at(i);
    }
}

void NeuronLiveMatchDialog::highlight_pair()
{
    if(cb_pair->count()>0){
        int info[4];
        LocationSimple * SP = 0;

        //update location of all markers
        for(int i=0; i<mList->size(); i++){
            //update location
            SP=(LocationSimple*)&(mList->at(i));
            get_marker_info(*SP, info);
            SP->x = ntList->at(info[0]).listNeuron.at(info[1]).x;
            SP->y = ntList->at(info[0]).listNeuron.at(info[1]).y;
            SP->z = ntList->at(info[0]).listNeuron.at(info[1]).z;
        }

        //reset cur_pair
        if(cur_pair>=0 && cur_pair<mmatch0.size()){
            SP=(LocationSimple*)&(mList->at(mmatch0[cur_pair]));
            SP->color.r = 255; SP->color.g = 0; SP->color.b = 0;

            SP=(LocationSimple*)&(mList->at(mmatch1[cur_pair]));
            SP->color.r = 0; SP->color.g = 255; SP->color.b = 0;

            if(cb_color->currentIndex()==0){
                if(stitchmask.at(cur_pair)>0){
                    matchfunc->highlight_nt1_seg(pmatch1[cur_pair],COLOR_STITCHED1);
                    matchfunc->highlight_nt0_seg(pmatch0[cur_pair],COLOR_STITCHED0);
                }else{
                    matchfunc->highlight_nt1_seg(pmatch1[cur_pair],COLOR_ORG1);
                    matchfunc->highlight_nt0_seg(pmatch0[cur_pair],COLOR_ORG0);
                }
            }else{
                matchfunc->highlight_nt1_seg(pmatch1[cur_pair],-1);
                matchfunc->highlight_nt0_seg(pmatch0[cur_pair],-1);
            }
        }

        cur_pair=cb_pair->currentIndex();

        //highlight new pair
        SP=(LocationSimple*)&(mList->at(mmatch0[cur_pair]));
        SP->color.r = 255; SP->color.g = 255; SP->color.b = 128;

        SP=(LocationSimple*)&(mList->at(mmatch1[cur_pair]));
        SP->color.r = 255; SP->color.g = 255; SP->color.b = 128;

        //if(stitchmask.at(cur_pair)<=0){
        if(cb_color->currentIndex()==0){
            matchfunc->highlight_nt1_seg(pmatch1[cur_pair],COLOR_ACTIVE1);
            matchfunc->highlight_nt0_seg(pmatch0[cur_pair],COLOR_ACTIVE0);
        }
        //}

        updateview();

        if(check_localview->isChecked()){
            updatelocalview();
        }
    }
}

void NeuronLiveMatchDialog::change_pair(int idx)
{
    _checkwindow();

    highlight_pair();

    btn_stitch->setEnabled(stitchmask.at(idx)==0);
}

void NeuronLiveMatchDialog::change_color()
{
    _checkwindow();

    if(cb_color->currentIndex()==0){
        //reset the color of neuron list
        NeuronTree* ntp = 0;
        ntp = (NeuronTree*)&(ntList->at(0));
        ntp->color.r=ntp->color.g=ntp->color.b=ntp->color.a=0;
        for(int i=0; i<ntList->at(0).listNeuron.size(); i++){
            NeuronSWC *p = (NeuronSWC *)&(ntList->at(0).listNeuron.at(i));
            p->type=COLOR_ORG0;
        }
        ntp = (NeuronTree*)&(ntList->at(1));
        ntp->color.r=ntp->color.g=ntp->color.b=ntp->color.a=0;
        for(int i=0; i<ntList->at(1).listNeuron.size(); i++){
            NeuronSWC *p = (NeuronSWC *)&(ntList->at(1).listNeuron.at(i));
            p->type=COLOR_ORG1;
        }
        //adjust the color of stitched pairs
        //reset cur_pair
        for(int idx_pair=0; idx_pair<pmatch1.size(); idx_pair++){
            if(stitchmask.at(idx_pair)>0){
                matchfunc->highlight_nt1_seg(pmatch1[idx_pair],COLOR_STITCHED1);
                matchfunc->highlight_nt0_seg(pmatch0[idx_pair],COLOR_STITCHED0);
            }
        }
    }else if(cb_color->currentIndex()==1){
        //reset the color of neuron list
        NeuronTree* ntp = 0;
        ntp = (NeuronTree*)&(ntList->at(0));
        ntp->color.r=ntp->color.g=ntp->color.b=ntp->color.a=0;
        for(int i=0; i<ntList->at(0).listNeuron.size(); i++){
            NeuronSWC *p = (NeuronSWC *)&(ntList->at(0).listNeuron.at(i));
            p->type=swcType0.at(i);
        }
        ntp = (NeuronTree*)&(ntList->at(1));
        ntp->color.r=ntp->color.g=ntp->color.b=ntp->color.a=0;
        for(int i=0; i<ntList->at(1).listNeuron.size(); i++){
            NeuronSWC *p = (NeuronSWC *)&(ntList->at(1).listNeuron.at(i));
            p->type=swcType1.at(i);
        }
    }

    updateview();

    highlight_pair();
}

void NeuronLiveMatchDialog::load_img0()
{
    QString fname_input;
    fname_input = QFileDialog::getOpenFileName(0, QObject::tr("Choose the Image of Bottom Section "),
                                               fname_input,
                                               QObject::tr("Images (*.raw *.tif *.lsm *.visiocytepbd *.visiocyteraw);;All(*)"));
    if(fname_input.isEmpty()){
        return;
    }

    unsigned char * p_img_tmp = 0;
    VISIOCYTELONG sz_tmp[4];
    int type_tmp;
    if(!simple_loadimage_wrapper(*callback, fname_input.toStdString().c_str(), p_img_tmp, sz_tmp, type_tmp)){
        visiocyte_msg("failed to load image: "+fname_input);
        return;
    }
    if(type_tmp!=1){
        visiocyte_msg("Only support UINT8/8bit image. Please convert datatype first (Image/Data->image type).");
        return;
    }
    int ch=0;
    if(sz_tmp[3]!=1){
        bool ok;
        ch=QInputDialog::getInteger(0, "select channel","Which channel to display? (start from 0)",0,0,sz_tmp[3]-1,1,&ok);
        if(!ok)
            return;
    }
    if(p_img0!=0) delete[] p_img0;
    p_img0=p_img_tmp+ch*sz_tmp[0]*sz_tmp[1]*sz_tmp[2];
    size_img0[0]=sz_tmp[0];
    size_img0[1]=sz_tmp[1];
    size_img0[2]=sz_tmp[2];
    size_img0[3]=1;

    updatelocalview();
}

void NeuronLiveMatchDialog::load_img1()
{
    QString fname_input;
    fname_input = QFileDialog::getOpenFileName(0, QObject::tr("Choose the Image of Top Section "),
                                               fname_input,
                                               QObject::tr("Images (*.raw *.tif *.lsm *.visiocytepbd *.visiocyteraw);;All(*)"));
    if(fname_input.isEmpty()){
        return;
    }

    unsigned char * p_img_tmp = 0;
    VISIOCYTELONG sz_tmp[4];
    int type_tmp;
    if(!simple_loadimage_wrapper(*callback, fname_input.toStdString().c_str(), p_img_tmp, sz_tmp, type_tmp)){
        visiocyte_msg("failed to load image: "+fname_input);
        return;
    }
    if(type_tmp!=1){
        visiocyte_msg("Only support UINT8/8bit image. Please convert datatype first (Image/Data->image type).");
        return;
    }
    int ch=0;
    if(sz_tmp[3]!=1){
        bool ok;
        ch=QInputDialog::getInteger(0, "select channel","Which channel to display? (start from 0)",0,0,sz_tmp[3]-1,1,&ok);
        if(!ok)
            return;
    }
    if(p_img1!=0) delete[] p_img1;
    p_img1=p_img_tmp+ch*sz_tmp[0]*sz_tmp[1]*sz_tmp[2];
    size_img1[0]=sz_tmp[0];
    size_img1[1]=sz_tmp[1];
    size_img1[2]=sz_tmp[2];
    size_img1[3]=1;

    updatelocalview();
}

void NeuronLiveMatchDialog::manualadd()
{
    int info[4];
    LocationSimple * SP = 0;

    //recolor corrent one
    if(cb_pair->count()>0){
        //reset cur_pair
        if(cur_pair>=0 && cur_pair<mmatch0.size()){
            SP=(LocationSimple*)&(mList->at(mmatch0[cur_pair]));
            SP->color.r = 255; SP->color.g = 0; SP->color.b = 0;
            //update location
            get_marker_info(*SP, info);
            SP->x = ntList->at(0).listNeuron.at(info[1]).x;
            SP->y = ntList->at(0).listNeuron.at(info[1]).y;
            SP->z = ntList->at(0).listNeuron.at(info[1]).z;

            SP=(LocationSimple*)&(mList->at(mmatch1[cur_pair]));
            SP->color.r = 0; SP->color.g = 255; SP->color.b = 0;
            //update location
            get_marker_info(*SP, info);
            SP->x = ntList->at(1).listNeuron.at(info[1]).x;
            SP->y = ntList->at(1).listNeuron.at(info[1]).y;
            SP->z = ntList->at(1).listNeuron.at(info[1]).z;


            if(cb_color->currentIndex()==0){
                if(stitchmask.at(cur_pair)>0){
                    matchfunc->highlight_nt1_seg(pmatch1[cur_pair],COLOR_STITCHED1);
                    matchfunc->highlight_nt0_seg(pmatch0[cur_pair],COLOR_STITCHED0);
                }else{
                    matchfunc->highlight_nt1_seg(pmatch1[cur_pair],COLOR_ORG1);
                    matchfunc->highlight_nt0_seg(pmatch0[cur_pair],COLOR_ORG0);
                }
            }
        }
    }

    link_new_marker_neuron();

    marker_match_dialog dialog(callback, mList);
    dialog.exec();

    updatematchlist();

    //recolor markers
    for(int i=0; i<mList->size(); i++){
        SP=(LocationSimple*)&(mList->at(i));
        get_marker_info(*SP, info);
        if(info[0]==0){
            if(info[2]>=0){
                SP->color.r = 255; SP->color.g = 0; SP->color.b = 0;
            }else{
                SP->color.r = 128; SP->color.g = 0; SP->color.b = 128;
            }
        }else{
            if(info[2]>=0){
                SP->color.r = 0; SP->color.g = 255; SP->color.b = 0;
            }else{
                SP->color.r = 0; SP->color.g = 128; SP->color.b = 128;
            }
        }
    }

    if(cb_pair->count()>0)
        cb_pair->setCurrentIndex(cb_pair->count()-1);

    checkwindow();
}

void NeuronLiveMatchDialog::link_new_marker_neuron()
{
    if(mList->size()<=0){
        return;
    }


    double dis;
    QVector<int> nidList, tidList, midList;
    QVector<VISIOCYTELONG> pidList;
    int nonetip_count=0, cantip_count=0;
    for(int i=0; i<mList->size(); i++){ //find new markers
        int info[4];
        double mdis=1e10;
        if(get_marker_info(mList->at(i),info))
            continue;
        int nid=-1;
        VISIOCYTELONG pid=0, tid=-1;

        for(int j=0; j<2; j++){
            for(int k=0; k<ntList->at(j).listNeuron.size(); k++){
                dis=NTDIS(ntList->at(j).listNeuron[k],mList->at(i));
                if(dis<mdis){
                    mdis=dis;
                    nid=j;
                    pid=k;
                }
            }
        }
        if(nid==0){
            tid=matchfunc->search_tip0(pid);
        }else{
            tid=matchfunc->search_tip1(pid);
        }
        if(tid!=pid){
            nonetip_count++;
            if(tid>=0) cantip_count++;
        }

        nidList.append(nid);
        pidList.append(pid);
        tidList.append(tid);
        midList.append(i);
    }
    if(cantip_count>0){
        QString str_msg = QString::number(nidList.size()) + " new markers were identified.\n" +
                QString::number(nonetip_count) + " of them are not eligible border tips.\n" +
                QString::number(cantip_count) + " of them can be corrected.\n Do you want to correct it?";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Match Border Tips", str_msg,
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            for(int i=0; i<nidList.size(); i++){
                if(tidList.at(i)>=0){
                    pidList[i]=tidList.at(i);
                }
            }
        }
    }

    //update
    LocationSimple *p = 0;
    for(int j=0; j<nidList.size(); j++){
        int i=midList.at(j);
        int nid=nidList.at(j);
        VISIOCYTELONG pid=pidList.at(j);
        p = (LocationSimple *)&(mList->at(i));
        p->name=QString::number(i).toStdString();
        if(nid>=0){
            QString tmp = QString::number(nid) + " " + QString::number(pid) + " -1";
            p->comments=tmp.toStdString();
            if(nid==0){
                p->color.r = 128;
                p->color.g = 0;
                p->color.b = 128;
            }else{
                p->color.r = 0;
                p->color.g = 128;
                p->color.b = 128;
            }
            p->x=ntList->at(nid).listNeuron.at(pid).x;
            p->y=ntList->at(nid).listNeuron.at(pid).y;
            p->z=ntList->at(nid).listNeuron.at(pid).z;
        }
    }

    updateview();
}

void NeuronLiveMatchDialog::update_marker_to_neuron()
{
    if(mList->size()<=0){
        return;
    }

    LocationSimple *p = 0;
    for(int i=0; i<mList->size(); i++){
        int info[4];
        if(!get_marker_info(mList->at(i),info))
            continue;
        int nid=info[0];
        VISIOCYTELONG pid=info[1];

        p = (LocationSimple *)&(mList->at(i));
        p->name=QString::number(i).toStdString();
        if(nid>=0){
            QString tmp = QString::number(nid) + " " + QString::number(pid) + " -1";
            p->comments=tmp.toStdString();
            if(nid==0){
                p->x=ntList->at(0).listNeuron.at(pid).x;
                p->y=ntList->at(0).listNeuron.at(pid).y;
                p->z=ntList->at(0).listNeuron.at(pid).z;
            }else{
                p->x=ntList->at(1).listNeuron.at(pid).x;
                p->y=ntList->at(1).listNeuron.at(pid).y;
                p->z=ntList->at(1).listNeuron.at(pid).z;
            }
        }
    }

    updateview();
}

void NeuronLiveMatchDialog::skip()
{
    _checkwindow();
    if(cb_pair->currentIndex() < cb_pair->count()-1)
        cb_pair->setCurrentIndex(cb_pair->currentIndex()+1);
    else{
        visiocyte_msg("Reach the end. Restart from the beginnig.");
        cb_pair->setCurrentIndex(0);
    }
}

void NeuronLiveMatchDialog::stitch()
{
    _checkwindow();

    int idx=cb_pair->currentIndex();
    if(matchfunc->stitch(pmatch0.at(idx),pmatch1.at(idx))){
        stitchmask[idx]=1;
        cb_pair->setItemText(idx, cb_pair->currentText() + " = stitched!");
    }else{
        stitchmask[idx]=-1;
        cb_pair->setItemText(idx, cb_pair->currentText() + " = failed, LOOP!");
    }

    if(cb_pair->currentIndex() < cb_pair->count()-1)
        cb_pair->setCurrentIndex(cb_pair->currentIndex()+1);
    else{
        //visiocyte_msg("Reach the end. Restart from the beginnig.");
        if(cb_pair->currentIndex()==0){
            highlight_pair();
        }else{
            cb_pair->setCurrentIndex(0);
        }
    }
}

void NeuronLiveMatchDialog::stitchall()
{
    _checkwindow();

    for(int idx=0; idx<stitchmask.size(); idx++){
        if(stitchmask[idx] != 0)
            continue;
        if(matchfunc->stitch(pmatch0.at(idx),pmatch1.at(idx))){
            stitchmask[idx]=1;
            cb_pair->setItemText(idx, cb_pair->itemText(idx) + " = stitched!");

            //update the location of markers
            LocationSimple* SP=(LocationSimple*)&(mList->at(mmatch0.at(idx)));
            int info[4];
            //neuron 0
            get_marker_info(*SP, info);
            SP->x = ntList->at(0).listNeuron.at(info[1]).x;
            SP->y = ntList->at(0).listNeuron.at(info[1]).y;
            SP->z = ntList->at(0).listNeuron.at(info[1]).z;

            SP=(LocationSimple*)&(mList->at(mmatch1.at(idx)));
            //neuron 1
            get_marker_info(*SP, info);
            SP->x = ntList->at(1).listNeuron.at(info[1]).x;
            SP->y = ntList->at(1).listNeuron.at(info[1]).y;
            SP->z = ntList->at(1).listNeuron.at(info[1]).z;

            matchfunc->highlight_nt1_seg(pmatch1.at(idx),COLOR_STITCHED1);
            matchfunc->highlight_nt0_seg(pmatch0.at(idx),COLOR_STITCHED0);
        }else{
            stitchmask[idx]=-1;
            cb_pair->setItemText(idx, cb_pair->itemText(idx) + " = failed, LOOP!");
        }
    }

    if(cur_pair!=0){
        if(cb_pair->count()>0)
            cb_pair->setCurrentIndex(0);
    }else{
        btn_stitch->setEnabled(stitchmask[0] == 0);
        highlight_pair();
    }
    updateview();
}

void NeuronLiveMatchDialog::output()
{
    _checkwindow();

    QString fname_output = QFileDialog::getSaveFileName(0, QObject::tr("Select outputs folder and prefix"),
                                                        QObject::tr(""),
            QObject::tr(""
                ));
    if(fname_output.isEmpty())
        return;
//    QString folder_output = QFileDialog::getExistingDirectory(this, "Select Output Folder","");
//    if(folder_output.isEmpty())
//        return;
//    bool ok;
//    QString fname_base = QInputDialog::getText(this, "Output Prefix","Assign a prefix for all outputs",QLineEdit::Normal,"",&ok);
//    if(!ok)
//        return;
//    QString fname_output = QDir(folder_output).filePath(fname_base);

    matchfunc->output_candMatchScore(fname_output + "_matchscore.txt");
    matchfunc->output_affine(fname_output,ntList->at(0).file);
    matchfunc->output_matchedMarkers_orgspace(fname_output + "_nt0_matched.marker",fname_output + "_nt1_matched.marker");
    matchfunc->output_parameter(fname_output+"_param.txt");
    matchfunc->output_stitch(fname_output);
}

NeuronMatchDialog::NeuronMatchDialog()
{
    nt0 = nt1 = NULL;
    folder_output.clear();
    fname_output.clear();
    name_nt0.clear();
    name_nt1.clear();
    fname_nt0.clear();
    fname_nt1.clear();

    creat();
}

void NeuronMatchDialog::creat()
{
    gridLayout = new QGridLayout();

    //input zone
    label_load0 = new QLabel(QObject::tr("First Neuron for stitch (smaller in stacking direction):"));
    gridLayout->addWidget(label_load0,0,0,1,6);
    edit_load0 = new QLineEdit();
    edit_load0->setText(fname_nt0); edit_load0->setReadOnly(true);
    gridLayout->addWidget(edit_load0,1,0,1,5);
    btn_load0 = new QPushButton("...");
    gridLayout->addWidget(btn_load0,1,5,1,1);

    label_load1 = new QLabel(QObject::tr("Second Neuron for stitch (larger in stacking direction):"));
    gridLayout->addWidget(label_load1,2,0,1,6);
    edit_load1 = new QLineEdit();
    edit_load1->setText(fname_nt1); edit_load1->setReadOnly(true);
    gridLayout->addWidget(edit_load1,3,0,1,5);
    btn_load1 = new QPushButton("...");
    gridLayout->addWidget(btn_load1,3,5,1,1);

    label_output = new QLabel(QObject::tr("Output Direction"));
    gridLayout->addWidget(label_output,4,0,1,6);
    edit_output = new QLineEdit();
    edit_output->setText(fname_output);
    gridLayout->addWidget(edit_output,5,0,1,5);
    btn_output = new QPushButton("...");
    gridLayout->addWidget(btn_output,5,5,1,1);

    connect(btn_load0, SIGNAL(clicked()), this, SLOT(load0()));
    connect(btn_load1, SIGNAL(clicked()), this, SLOT(load1()));
    connect(btn_output, SIGNAL(clicked()), this, SLOT(output()));
    connect(edit_output, SIGNAL(textChanged(QString)), this, SLOT(outputchange(QString)));

    //parameter zone
    QFrame *line_1 = new QFrame();
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line_1,8,0,1,6);

    cb_dir = new QComboBox(); cb_dir->addItem("x"); cb_dir->addItem("y"); cb_dir->addItem("z");
    cb_dir->setCurrentIndex(2);
    spin_zscale = new QDoubleSpinBox();
    spin_zscale->setRange(0,100000); spin_zscale->setValue(1);
    spin_ang = new QDoubleSpinBox();
    spin_ang->setRange(0,180); spin_ang->setValue(60);
    spin_matchdis = new QDoubleSpinBox();
    spin_matchdis->setRange(0,100000); spin_matchdis->setValue(100);
    spin_searchspan = new QDoubleSpinBox();
    spin_searchspan->setRange(0,100000); spin_searchspan->setValue(20);
    spin_cmatchdis = new QDoubleSpinBox();
    spin_cmatchdis->setRange(0,100000); spin_cmatchdis->setValue(100);
    check_type = new QCheckBox("match by type defined in SWC file");
    spin_segthr = new QDoubleSpinBox();
    spin_segthr->setRange(0,100000); spin_segthr->setValue(0);
    spin_gapthr = new QDoubleSpinBox();
    spin_gapthr->setRange(0,100000); spin_gapthr->setValue(0);
    spin_maxcnum = new QSpinBox();
    spin_maxcnum->setRange(0,1e10); spin_maxcnum->setValue(1000);
    spin_spineLen = new QSpinBox();
    spin_spineLen->setRange(0,100000); spin_spineLen->setValue(5); spin_spineLen->setEnabled(false);
    spin_spineAng = new QDoubleSpinBox();
    spin_spineAng->setRange(0,180); spin_spineAng->setValue(30); spin_spineAng->setEnabled(false);
    spin_spineRadius = new QDoubleSpinBox();
    spin_spineRadius->setRange(0,100000); spin_spineRadius->setValue(3); spin_spineRadius->setEnabled(false);
    check_spine = new QCheckBox("filter spines when matching:");
    check_spine -> setChecked(false);
    connect(check_spine, SIGNAL(stateChanged(int)), this, SLOT(spineCheck(int)));
    QLabel* label_0 = new QLabel("stacking direction: ");
    gridLayout->addWidget(label_0,9,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(cb_dir,9,2,1,1);
    QLabel* label_1 = new QLabel("resacle stacking direction: ");
    gridLayout->addWidget(label_1,9,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_zscale,9,5,1,1);
    QLabel* label_2 = new QLabel("Max angular to match points (0~180): ");
    gridLayout->addWidget(label_2,10,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_ang,10,2,1,1);
    QLabel* label_3 = new QLabel("Max distance to match points: ");
    gridLayout->addWidget(label_3,10,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_matchdis,10,5,1,1);
    QLabel* label_4 = new QLabel("match candidates searching span: ");
    gridLayout->addWidget(label_4,11,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_searchspan,11,2,1,1);
    QLabel* label_5 = new QLabel("Max distance to match triangles: ");
    gridLayout->addWidget(label_5,11,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_cmatchdis,11,5,1,1);
    QLabel* label_6 = new QLabel("fragment filter (0=keep all): ");
    gridLayout->addWidget(label_6,12,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_segthr,12,2,1,1);
    QLabel* label_61 = new QLabel("Max number of triangles (smaller, faster): ");
    gridLayout->addWidget(label_61,12,3,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_maxcnum,12,5,1,1);
    QLabel* label_62 = new QLabel("small gap filter (0=no filter): ");
    gridLayout->addWidget(label_62,13,0,1,2,Qt::AlignRight);
    gridLayout->addWidget(spin_gapthr,13,2,1,1);
    gridLayout->addWidget(check_type,13,3,1,3,Qt::AlignRight);
    gridLayout->addWidget(check_spine,14,0,1,2);
    QLabel* label_7 = new QLabel("segment point #:");
    gridLayout->addWidget(label_7,15,0,1,1,Qt::AlignRight);
    gridLayout->addWidget(spin_spineLen,15,1,1,1);
    QLabel* label_8 = new QLabel("turning angle:");
    gridLayout->addWidget(label_8,15,2,1,1,Qt::AlignRight);
    gridLayout->addWidget(spin_spineAng,15,3,1,1);
    QLabel* label_9 = new QLabel("radius:");
    gridLayout->addWidget(label_9,15,4,1,1,Qt::AlignRight);
    gridLayout->addWidget(spin_spineRadius,15,5,1,1);

    //operation zone
    int optrow=16;
    QFrame *line_2 = new QFrame();
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line_2,optrow,0,1,6);
    check_stitch = new QCheckBox("Stitch Matched Points");
    gridLayout->addWidget(check_stitch,optrow+1,0,1,2);
    btn_test = new QPushButton("Test");
    connect(btn_test, SIGNAL(clicked()), this, SLOT(examinRun()));
    gridLayout->addWidget(btn_test,optrow+1,3,1,1);
    btn_test->setVisible(false); //show it only for test
    btn_run = new QPushButton("Run");
    connect(btn_run,     SIGNAL(clicked()), this, SLOT(run()));
    gridLayout->addWidget(btn_run,optrow+1,4,1,1);
    btn_quit = new QPushButton("Quit");
    connect(btn_quit,     SIGNAL(clicked()), this, SLOT(reject()));
    gridLayout->addWidget(btn_quit,optrow+1,5,1,1);

    btn_run->setEnabled(false);

    setLayout(gridLayout);
}

bool NeuronMatchDialog::load0()
{
    QString fileOpenName;
    fileOpenName = QFileDialog::getOpenFileName(0, QObject::tr("Open File"),
            "",
            QObject::tr("Supported file (*.swc *.eswc)"
                ";;Neuron structure	(*.swc)"
                ";;Extended neuron structure (*.eswc)"
                ));
    if(fileOpenName.isEmpty())
        return false;
    NeuronTree* nt = new NeuronTree();
    if (fileOpenName.toUpper().endsWith(".SWC") || fileOpenName.toUpper().endsWith(".ESWC"))
    {
        *nt = readSWC_file(fileOpenName);
    }
    if(nt->listNeuron.size()<=0){
        visiocyte_msg("failed to read SWC file: "+fileOpenName);
        return false;
    }
    nt0 = nt;
    fname_nt0 = fileOpenName;
    name_nt0 = QFileInfo(fileOpenName).baseName();
    edit_load0->setText(fname_nt0);

    if(fname_nt0.length() * fname_nt1.length() != 0){
        fname_output = QDir(folder_output).filePath(name_nt1 + "_to_" + name_nt0);
    }

    QDir::setCurrent(QFileInfo(fileOpenName).absoluteFilePath());

    if(fname_nt0.length() * fname_nt1.length() * folder_output.length() != 0){
        btn_run->setEnabled(true);
        edit_output->setText(fname_output);
    }

    return true;
}

bool NeuronMatchDialog::load1()
{
    QString fileOpenName;
    fileOpenName = QFileDialog::getOpenFileName(0, QObject::tr("Open File"),
            "",
            QObject::tr("Supported file (*.swc *.eswc)"
                ";;Neuron structure	(*.swc)"
                ";;Extended neuron structure (*.eswc)"
                ));
    if(fileOpenName.isEmpty())
        return false;
    NeuronTree* nt = new NeuronTree();
    if (fileOpenName.toUpper().endsWith(".SWC") || fileOpenName.toUpper().endsWith(".ESWC"))
    {
        *nt = readSWC_file(fileOpenName);
    }
    if(nt->listNeuron.size()<=0){
        visiocyte_msg("failed to read SWC file: "+fileOpenName);
        return false;
    }
    nt1 = nt;
    fname_nt1 = fileOpenName;
    name_nt1 = QFileInfo(fileOpenName).baseName();
    edit_load1->setText(fname_nt1);

    if(fname_nt0.length() * fname_nt1.length() != 0){
        fname_output = QDir(folder_output).filePath(name_nt1 + "_to_" + name_nt0);
    }

    QDir::setCurrent(QFileInfo(fileOpenName).absoluteFilePath());

    if(fname_nt0.length() * fname_nt1.length() * folder_output.length() != 0){
        btn_run->setEnabled(true);
        edit_output->setText(fname_output);
    }

    return true;
}

bool NeuronMatchDialog::output()
{
    QString fileSaveDir;
    fileSaveDir = QFileDialog::getExistingDirectory(0, QObject::tr("Select Directory to Save Results"),
            "~",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(fileSaveDir.isEmpty())
        return false;

    folder_output = fileSaveDir;

    fname_output = QDir(folder_output).filePath(name_nt1 + "_to_" + name_nt0);
    edit_output->setText(fname_output);

    QDir::setCurrent(fileSaveDir);

    if(fname_nt0.length() * fname_nt1.length() * folder_output.length() != 0){
        btn_run->setEnabled(true);
    }

    return true;
}

void NeuronMatchDialog::outputchange(QString text)
{
    fname_output=text;
}

void NeuronMatchDialog::examinRun()
{
    gridLayout->setEnabled(false);


    vector<double> walltimes;
    vector<double> cputimes;
    vector<int> num_TP, num_FP, num_FN, num_truth, num_bp0, num_bp1, num_tri0, num_tri1, num_trimatch;
    vector<int> tri_number;
    vector<double> pmatchThr,pmatchAng,tmatchThr;

    //triangle number
    for(int i=0; i<10; i++){
        int tri=100*pow(2.0,i);
        //test triangle number and running time
        //init matching function
        int tmpnum[9];
        neuron_match_clique matchfunc(nt0,nt1);

        //parameters
        matchfunc.direction=cb_dir->currentIndex();
        matchfunc.zscale=spin_zscale->value();
        matchfunc.angThr_match=cos(spin_ang->value()/180*M_PI);
        matchfunc.pmatchThr = spin_matchdis->value();
        matchfunc.spanCand = spin_searchspan->value();
        matchfunc.cmatchThr = spin_cmatchdis->value();
        matchfunc.segmentThr = spin_segthr->value();
        matchfunc.maxClique3Num = tri;
        matchfunc.gapThr = spin_gapthr->value();
        if(check_spine->isChecked()){
            matchfunc.spineLengthThr = spin_spineLen->value();
            matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
            matchfunc.spineRadiusThr = spin_spineRadius->value();
        }else{
            matchfunc.spineLengthThr = 0;
            matchfunc.spineRadiusThr = 0;
        }
        tri_number.push_back(matchfunc.maxClique3Num);
        pmatchThr.push_back(matchfunc.pmatchThr);
        pmatchAng.push_back(matchfunc.angThr_match);
        tmatchThr.push_back(matchfunc.cmatchThr);

        double wall0 = get_wall_time();
        double cpu0  = get_cpu_time();
        //init clique and candidate
        matchfunc.init();

        //global match
        matchfunc.globalmatch();
        //  Stop timers
        double wall1 = get_wall_time();
        double cpu1  = get_cpu_time();

        matchfunc.examineMatchingResult(tmpnum);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<tmpnum[0]<<":"<<tmpnum[1]<<":"<<tmpnum[2]<<":"<<tmpnum[3]<<":"<<tmpnum[4]<<":"<<tmpnum[5]<<":"
                  <<tmpnum[6]<<":"<<tmpnum[7]<<":"<<tmpnum[8];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back(tmpnum[0]);
        num_FP.push_back(tmpnum[1]);
        num_FN.push_back(tmpnum[2]);
        num_truth.push_back(tmpnum[3]);
        num_bp0.push_back(tmpnum[4]);
        num_bp1.push_back(tmpnum[5]);
        num_tri0.push_back(tmpnum[6]);
        num_tri1.push_back(tmpnum[7]);
        num_trimatch.push_back(tmpnum[8]);
    }

    //point match distance
    for(double param=20; param<=300; param+=20){
        //test triangle number and running time
        //init matching function
        int tmpnum[9];
        neuron_match_clique matchfunc(nt0,nt1);

        //parameters
        matchfunc.direction=cb_dir->currentIndex();
        matchfunc.zscale=spin_zscale->value();
        matchfunc.angThr_match=cos(spin_ang->value()/180*M_PI);
        matchfunc.pmatchThr = param;
        matchfunc.spanCand = spin_searchspan->value();
        matchfunc.cmatchThr = spin_cmatchdis->value();
        matchfunc.segmentThr = spin_segthr->value();
        matchfunc.maxClique3Num = spin_maxcnum->value();
        matchfunc.gapThr = spin_gapthr->value();
        if(check_spine->isChecked()){
            matchfunc.spineLengthThr = spin_spineLen->value();
            matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
            matchfunc.spineRadiusThr = spin_spineRadius->value();
        }else{
            matchfunc.spineLengthThr = 0;
            matchfunc.spineRadiusThr = 0;
        }
        tri_number.push_back(matchfunc.maxClique3Num);
        pmatchThr.push_back(matchfunc.pmatchThr);
        pmatchAng.push_back(matchfunc.angThr_match);
        tmatchThr.push_back(matchfunc.cmatchThr);

        double wall0 = get_wall_time();
        double cpu0  = get_cpu_time();
        //init clique and candidate
        matchfunc.init();

        //global match
        matchfunc.globalmatch();
        //  Stop timers
        double wall1 = get_wall_time();
        double cpu1  = get_cpu_time();

        matchfunc.examineMatchingResult(tmpnum);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<tmpnum[0]<<":"<<tmpnum[1]<<":"<<tmpnum[2]<<":"<<tmpnum[3]<<":"<<tmpnum[4]<<":"<<tmpnum[5]<<":"
                  <<tmpnum[6]<<":"<<tmpnum[7]<<":"<<tmpnum[8];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back(tmpnum[0]);
        num_FP.push_back(tmpnum[1]);
        num_FN.push_back(tmpnum[2]);
        num_truth.push_back(tmpnum[3]);
        num_bp0.push_back(tmpnum[4]);
        num_bp1.push_back(tmpnum[5]);
        num_tri0.push_back(tmpnum[6]);
        num_tri1.push_back(tmpnum[7]);
        num_trimatch.push_back(tmpnum[8]);
    }

    //point match angular threshold
    for(double param=10; param<181; param+=10){
        //test triangle number and running time
        //init matching function
        int tmpnum[9];
        neuron_match_clique matchfunc(nt0,nt1);

        //parameters
        matchfunc.direction=cb_dir->currentIndex();
        matchfunc.zscale=spin_zscale->value();
        matchfunc.angThr_match=cos(param/180*M_PI);
        matchfunc.pmatchThr = spin_matchdis->value();
        matchfunc.spanCand = spin_searchspan->value();
        matchfunc.cmatchThr = spin_cmatchdis->value();
        matchfunc.segmentThr = spin_segthr->value();
        matchfunc.maxClique3Num = spin_maxcnum->value();
        matchfunc.gapThr = spin_gapthr->value();
        if(check_spine->isChecked()){
            matchfunc.spineLengthThr = spin_spineLen->value();
            matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
            matchfunc.spineRadiusThr = spin_spineRadius->value();
        }else{
            matchfunc.spineLengthThr = 0;
            matchfunc.spineRadiusThr = 0;
        }
        tri_number.push_back(matchfunc.maxClique3Num);
        pmatchThr.push_back(matchfunc.pmatchThr);
        pmatchAng.push_back(matchfunc.angThr_match);
        tmatchThr.push_back(matchfunc.cmatchThr);

        double wall0 = get_wall_time();
        double cpu0  = get_cpu_time();
        //init clique and candidate
        matchfunc.init();

        //global match
        matchfunc.globalmatch();
        //  Stop timers
        double wall1 = get_wall_time();
        double cpu1  = get_cpu_time();

        matchfunc.examineMatchingResult(tmpnum);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<tmpnum[0]<<":"<<tmpnum[1]<<":"<<tmpnum[2]<<":"<<tmpnum[3]<<":"<<tmpnum[4]<<":"<<tmpnum[5]<<":"
                  <<tmpnum[6]<<":"<<tmpnum[7]<<":"<<tmpnum[8];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back(tmpnum[0]);
        num_FP.push_back(tmpnum[1]);
        num_FN.push_back(tmpnum[2]);
        num_truth.push_back(tmpnum[3]);
        num_bp0.push_back(tmpnum[4]);
        num_bp1.push_back(tmpnum[5]);
        num_tri0.push_back(tmpnum[6]);
        num_tri1.push_back(tmpnum[7]);
        num_trimatch.push_back(tmpnum[8]);
    }

    //triangle match distance
    for(double param=20; param<=300; param+=20){
        //test triangle number and running time
        //init matching function
        int tmpnum[9];
        neuron_match_clique matchfunc(nt0,nt1);

        //parameters
        matchfunc.direction=cb_dir->currentIndex();
        matchfunc.zscale=spin_zscale->value();
        matchfunc.angThr_match=cos(spin_ang->value()/180*M_PI);
        matchfunc.pmatchThr = spin_matchdis->value();
        matchfunc.spanCand = spin_searchspan->value();
        matchfunc.cmatchThr = param;
        matchfunc.segmentThr = spin_segthr->value();
        matchfunc.maxClique3Num = spin_maxcnum->value();
        matchfunc.gapThr = spin_gapthr->value();
        if(check_spine->isChecked()){
            matchfunc.spineLengthThr = spin_spineLen->value();
            matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
            matchfunc.spineRadiusThr = spin_spineRadius->value();
        }else{
            matchfunc.spineLengthThr = 0;
            matchfunc.spineRadiusThr = 0;
        }
        tri_number.push_back(matchfunc.maxClique3Num);
        pmatchThr.push_back(matchfunc.pmatchThr);
        pmatchAng.push_back(matchfunc.angThr_match);
        tmatchThr.push_back(matchfunc.cmatchThr);

        double wall0 = get_wall_time();
        double cpu0  = get_cpu_time();
        //init clique and candidate
        matchfunc.init();

        //global match
        matchfunc.globalmatch();
        //  Stop timers
        double wall1 = get_wall_time();
        double cpu1  = get_cpu_time();

        matchfunc.examineMatchingResult(tmpnum);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<tmpnum[0]<<":"<<tmpnum[1]<<":"<<tmpnum[2]<<":"<<tmpnum[3]<<":"<<tmpnum[4]<<":"<<tmpnum[5]<<":"
                  <<tmpnum[6]<<":"<<tmpnum[7]<<":"<<tmpnum[8];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back(tmpnum[0]);
        num_FP.push_back(tmpnum[1]);
        num_FN.push_back(tmpnum[2]);
        num_truth.push_back(tmpnum[3]);
        num_bp0.push_back(tmpnum[4]);
        num_bp1.push_back(tmpnum[5]);
        num_tri0.push_back(tmpnum[6]);
        num_tri1.push_back(tmpnum[7]);
        num_trimatch.push_back(tmpnum[8]);
    }

    //output
    QString fname_out=fname_output+"_testlog.txt";
    ofstream fp;
    fp.open(fname_out.toStdString().c_str());
    for(int i=0; i<walltimes.size(); i++){
        fp<<tri_number[i]<<"\t";
        fp<<pmatchThr[i]<<"\t";
        fp<<pmatchAng[i]<<"\t";
        fp<<tmatchThr[i]<<"\t";
        fp<<walltimes[i]<<"\t";
        fp<<cputimes[i]<<"\t";
        fp<<num_TP[i]<<"\t";
        fp<<num_FP[i]<<"\t";
        fp<<num_FN[i]<<"\t";
        fp<<num_truth[i]<<"\t";
        fp<<num_bp0[i]<<"\t";
        fp<<num_bp1[i]<<"\t";
        fp<<num_tri0[i]<<"\t";
        fp<<num_tri1[i]<<"\t";
        fp<<num_trimatch[i]<<endl;
    }
    fp.close();

    neuron_match_clique matchfunc(nt0,nt1);
    matchfunc.direction=cb_dir->currentIndex();
    matchfunc.zscale=spin_zscale->value();
    matchfunc.angThr_match=cos(spin_ang->value()/180*M_PI);
    matchfunc.pmatchThr = spin_matchdis->value();
    matchfunc.spanCand = spin_searchspan->value();
    matchfunc.cmatchThr = spin_cmatchdis->value();
    matchfunc.segmentThr = spin_segthr->value();
    matchfunc.maxClique3Num = spin_maxcnum->value();
    matchfunc.gapThr = spin_gapthr->value();
    if(check_spine->isChecked()){
        matchfunc.spineLengthThr = spin_spineLen->value();
        matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
        matchfunc.spineRadiusThr = spin_spineRadius->value();
    }else{
        matchfunc.spineLengthThr = 0;
        matchfunc.spineRadiusThr = 0;
    }
    matchfunc.output_parameter(fname_output+"_param.txt");


    gridLayout->setEnabled(true);
    visiocyte_msg("testing finished");
}

void NeuronMatchDialog::run()
{
    gridLayout->setEnabled(false);
    //init matching function
    neuron_match_clique matchfunc(nt0,nt1);

    //parameters
    matchfunc.direction=cb_dir->currentIndex();
    matchfunc.zscale=spin_zscale->value();
    matchfunc.angThr_match=cos(spin_ang->value()/180*M_PI);
    matchfunc.pmatchThr = spin_matchdis->value();
    matchfunc.spanCand = spin_searchspan->value();
    matchfunc.cmatchThr = spin_cmatchdis->value();
    matchfunc.segmentThr = spin_segthr->value();
    matchfunc.maxClique3Num = spin_maxcnum->value();
    matchfunc.gapThr = spin_gapthr->value();
    matchfunc.typeConstrain = check_type->isChecked();
    if(check_spine->isChecked()){
        matchfunc.spineLengthThr = spin_spineLen->value();
        matchfunc.spineAngThr = cos(spin_spineAng->value()/180*M_PI);
        matchfunc.spineRadiusThr = spin_spineRadius->value();
    }else{
        matchfunc.spineLengthThr = 0;
        matchfunc.spineRadiusThr = 0;
    }


    //init clique and candidate
    matchfunc.init();

    //global match
    qDebug()<<"start global search";
    matchfunc.globalmatch();

    //stitch if checked
    if(check_stitch->isChecked()){
        qDebug()<<"stitch matched points";
        matchfunc.stitch();
        matchfunc.output_stitch(fname_output);
    }

    //iterative local match
    matchfunc.output_candMatchScore(fname_output + "_matchscore.txt");
    matchfunc.output_affine(fname_output,fname_nt0);
    matchfunc.output_matchedMarkers_orgspace(QDir(folder_output).filePath(name_nt0 + "_matched.marker"),QDir(folder_output).filePath(name_nt1 + "_matched.marker"));
    matchfunc.output_parameter(fname_output+"_param.txt");

    gridLayout->setEnabled(true);
    visiocyte_msg("matching finished");
}

void NeuronMatchDialog::spineCheck(int c)
{
    if(check_spine->isChecked()){
        spin_spineLen->setEnabled(true);
        spin_spineAng->setEnabled(true);
        spin_spineRadius->setEnabled(true);
    }else{
        spin_spineLen->setEnabled(false);
        spin_spineAng->setEnabled(false);
        spin_spineRadius->setEnabled(false);
    }
}

neuron_match_clique::neuron_match_clique(NeuronTree* botNeuron, NeuronTree* topNeuron)
{
    nt0_stitch=botNeuron;
    nt1_stitch=topNeuron;

    nt0=new NeuronTree;
    nt0_org=new NeuronTree;
    nt1=new NeuronTree;
    nt1_org=new NeuronTree;
    nt1_a=new NeuronTree;

    backupNeuron(*nt0_stitch,*nt0);
    backupNeuron(*nt1_stitch,*nt1);
    backupNeuron(*nt0_stitch,*nt0_org);
    backupNeuron(*nt1_stitch,*nt1_org);
    backupNeuron(*nt1_stitch,*nt1_a);

    constructNeuronGraph(*nt0, ng0);
    constructNeuronGraph(*nt1, ng1);

    setSWCType(nt0, nt1);

    spanCand = 20;
    direction = 2;
    midplane = 0;
    angThr_match = cos(M_PI/3);
    angThr_stack = -1;
    dir_range = 100;
    cmatchThr = 100;
    pmatchThr = 100;
    zscale = 1;
    segmentThr = 0;
    gapThr = 0;
    spineLengthThr = 0;
    spineAngThr = -1;
    spineRadiusThr = 0;
    maxClique3Num = 10000;
    typeConstrain = false;
}

void neuron_match_clique::globalmatch()
{
    candmatch0.clear();
    candmatch1.clear();
    pmatch0.clear();
    pmatch1.clear();
    double bestEnergy = 0;
    QList<int> candtype0, candtype1;
    if(typeConstrain){
        for(VISIOCYTELONG i=0; i<candID0.size(); i++){
            candtype0.append(swcType0.at(candID0.at(i)));
        }
        for(VISIOCYTELONG i=0; i<candID1.size(); i++){
            candtype1.append(swcType1.at(candID1.at(i)));
        }
    }else{
        for(VISIOCYTELONG i=0; i<candID0.size(); i++){
            candtype0.append(1);
        }
        for(VISIOCYTELONG i=0; i<candID1.size(); i++){
            candtype1.append(1);
        }
    }

    if(candID0.size()==0 || candID1.size()==0){
        return;
    }else if(candID0.size()==1){ //only one candidate, match by angle
        double mang=-1;
        int mid=-1;
        for(int i=0; i<candID1.size(); i++){
            //match by swc type first
            if(candtype0.at(0) != candtype1.at(i))
                continue;
            //find the one with the highest angle similarity
            double ang=NTDOT(canddir0.at(0),canddir1.at(i));
            if(ang>mang){
                mang=ang;
                mid=i;
            }
        }
        if(mid>=0){
            candmatch0.append(0);
            candmatch1.append(mid);
        }
    }else if(candID1.size()==1){ //only one candidate, match by angle
        double mang=-1;
        int mid=-1;
        for(int i=0; i<candID0.size(); i++){
            //match by swc type first
            if(candtype0.at(i) != candtype1.at(0))
                continue;
            //find the one with the highest angle similarity
            double ang=NTDOT(canddir0.at(i),canddir1.at(0));
            if(ang>mang){
                mang=ang;
                mid=i;
            }
        }
        if(mid>=0){
            candmatch0.append(mid);
            candmatch1.append(0);
        }
    }else if(candID0.size()==2){ //only two markers to match
        QList<int> tmpmatch0, tmpmatch1;
        double errdis=1e16;
        double errang=-2;
        for(int i=0; i<candID1.size(); i++){
            for(int j=i+1; j<candID1.size(); j++){
                //check swc type first
                if((candtype0.at(0) != candtype1.at(i) && candtype0.at(0) != candtype1.at(j) ) ||
                        (candtype0.at(1) != candtype1.at(i) && candtype0.at(1) != candtype1.at(j) ))
                    continue;
                //check distance
                double tmp_shift_x=0,tmp_shift_y=0,tmp_shift_z=0,tmp_angle=0,tmp_cent_x=0,tmp_cent_y=0,tmp_cent_z=0;
                if(direction==0) tmp_shift_x=-1;
                if(direction==1) tmp_shift_y=-1;
                if(direction==2) tmp_shift_z=-1;
                QList<XYZ> c0,c1,tmpcoord, tmpdir;
                c0.append(XYZ(candcoord0.at(0)));
                c0.append(XYZ(candcoord0.at(1)));
                c1.append(XYZ(candcoord1.at(i)));
                c1.append(XYZ(candcoord1.at(j)));
                double dis=compute_affine_4dof(c0,c1,tmp_shift_x,tmp_shift_y,tmp_shift_z,tmp_angle,tmp_cent_x,tmp_cent_y,tmp_cent_z,direction);
                if(dis<0){
                    continue;
                }
                //check angular
                if(dis<errdis){
                    affine_XYZList(candcoord1, tmpcoord, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    affine_XYZList(canddircoord1, tmpdir, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    minus_XYZList(tmpdir,tmpcoord,tmpdir);
                    QList<int> tmpMatchMarkers[2];
                    tmpMatchMarkers[0]=QList<int>();
                    tmpMatchMarkers[1]=QList<int>();
                    getMatchPairs_XYZList(candcoord0, tmpcoord, canddir0, tmpdir, candcomponents0, candcomponents1, candtype0, candtype1, tmpMatchMarkers, pmatchThr, angThr_match);
                    double ang=NTDOT(canddir0.at(0),tmpdir.at(i));
                    ang+=NTDOT(canddir0.at(1),tmpdir.at(j));
                    if((ang>errang && tmpMatchMarkers[0].size()>=tmpmatch0.size()) || tmpMatchMarkers[0].size()>tmpmatch0.size()){
                        errang=ang;
                        errdis=dis;
                        tmpmatch0=tmpMatchMarkers[0];
                        tmpmatch1=tmpMatchMarkers[1];
                    }

                    affine_XYZList(candcoord1, tmpcoord, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle+180, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    affine_XYZList(canddircoord1, tmpdir, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle+180, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    minus_XYZList(tmpdir,tmpcoord,tmpdir);
                    getMatchPairs_XYZList(candcoord0, tmpcoord, canddir0, tmpdir, candcomponents0, candcomponents1, candtype0, candtype1, tmpMatchMarkers, pmatchThr, angThr_match);
                    ang=NTDOT(canddir0.at(1),tmpdir.at(i));
                    ang+=NTDOT(canddir0.at(0),tmpdir.at(j));
                    if((ang>errang && tmpMatchMarkers[0].size()>=tmpmatch0.size()) || tmpMatchMarkers[0].size()>tmpmatch0.size()){
                        errang=ang;
                        errdis=dis;
                        tmpmatch0=tmpMatchMarkers[0];
                        tmpmatch1=tmpMatchMarkers[1];
                    }
                }
            }
        }
        candmatch0 = tmpmatch0;
        candmatch1 = tmpmatch1;
    }else if(candID1.size()==2){
        QList<int> tmpmatch0, tmpmatch1;
        double errdis=1e16;
        double errang=-2;
        for(int i=0; i<candID0.size(); i++){
            for(int j=i+1; j<candID0.size(); j++){
                //check swc type first
                if((candtype0.at(i) != candtype1.at(0) && candtype0.at(i) != candtype1.at(1) ) ||
                        (candtype0.at(j) != candtype1.at(0) && candtype0.at(j) != candtype1.at(1) ))
                    continue;
                //check distance
                double tmp_shift_x=0,tmp_shift_y=0,tmp_shift_z=0,tmp_angle=0,tmp_cent_x=0,tmp_cent_y=0,tmp_cent_z=0;
                if(direction==0) tmp_shift_x=-1;
                if(direction==1) tmp_shift_y=-1;
                if(direction==2) tmp_shift_z=-1;
                QList<XYZ> c0,c1,tmpcoord, tmpdir;
                c0.append(XYZ(candcoord0.at(i)));
                c0.append(XYZ(candcoord0.at(j)));
                c1.append(XYZ(candcoord1.at(0)));
                c1.append(XYZ(candcoord1.at(1)));
                double dis=compute_affine_4dof(c0,c1,tmp_shift_x,tmp_shift_y,tmp_shift_z,tmp_angle,tmp_cent_x,tmp_cent_y,tmp_cent_z,direction);
                if(dis<0){
                    continue;
                }
                //check angular
                if(dis<errdis){
                    affine_XYZList(candcoord1, tmpcoord, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    affine_XYZList(canddircoord1, tmpdir, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    minus_XYZList(tmpdir,tmpcoord,tmpdir);
                    QList<int> tmpMatchMarkers[2];
                    tmpMatchMarkers[0]=QList<int>();
                    tmpMatchMarkers[1]=QList<int>();
                    getMatchPairs_XYZList(candcoord0, tmpcoord, canddir0, tmpdir, candcomponents0, candcomponents1, candtype0, candtype1, tmpMatchMarkers, pmatchThr, angThr_match);
                    double ang=NTDOT(canddir0.at(i),tmpdir.at(0));
                    ang+=NTDOT(canddir0.at(j),tmpdir.at(1));
                    if((ang>errang && tmpMatchMarkers[0].size()>=tmpmatch0.size()) || tmpMatchMarkers[0].size()>tmpmatch0.size()){
                        errang=ang;
                        errdis=dis;
                        tmpmatch0=tmpMatchMarkers[0];
                        tmpmatch1=tmpMatchMarkers[1];
                    }

                    affine_XYZList(candcoord1, tmpcoord, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle+180, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    affine_XYZList(canddircoord1, tmpdir, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle+180, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                    minus_XYZList(tmpdir,tmpcoord,tmpdir);
                    getMatchPairs_XYZList(candcoord0, tmpcoord, canddir0, tmpdir, candcomponents0, candcomponents1, candtype0, candtype1, tmpMatchMarkers, pmatchThr, angThr_match);
                    ang=NTDOT(canddir0.at(i),tmpdir.at(1));
                    ang+=NTDOT(canddir0.at(j),tmpdir.at(0));
                    if((ang>errang && tmpMatchMarkers[0].size()>=tmpmatch0.size()) || tmpMatchMarkers[0].size()>tmpmatch0.size()){
                        errang=ang;
                        errdis=dis;
                        tmpmatch0=tmpMatchMarkers[0];
                        tmpmatch1=tmpMatchMarkers[1];
                    }
                }
            }
        }
        candmatch0 = tmpmatch0;
        candmatch1 = tmpmatch1;
    }else{
        if(rankedCliqueMatch.size()<=0)
            return;

        //search for best unconflict groups of matched points
        double better_shift_x=0,better_shift_y=0,better_shift_z=0,better_angle=0,better_cent_x=0,better_cent_y=0,better_cent_z=0;

        //edgeMask[i][j]==1 if cand0[i] and cand1[j] has been paired as initial
        vector<vector<int> > edgeMask;
        edgeMask.resize(candID0.size());
        for(int tmpi=0; tmpi<candID0.size(); tmpi++){
            edgeMask[tmpi].resize(candID1.size(), 0);
        }

        multimap<int, QVector<int> , std::greater<int> >::iterator iter_conflict=rankedCliqueMatch.begin();
        //try 10 times, accept best one if no better one can be found with current trial
        for(int trial=0; trial<candID0.size()+candID1.size(); trial++){
            if(iter_conflict==rankedCliqueMatch.end()) break;
            //construct in-conflict matching pairs
            QList<int> tmpmatch0, tmpmatch1;
            int c0 = iter_conflict->second.at(0);//.value().at(0);
            int c1 = iter_conflict->second.at(1);
            tmpmatch0.append(cliqueList0.at(c0).idx[0]);
            tmpmatch0.append(cliqueList0.at(c0).idx[1]);
            tmpmatch0.append(cliqueList0.at(c0).idx[2]);
            tmpmatch1.append(cliqueList1.at(c1).idx[0]);
            tmpmatch1.append(cliqueList1.at(c1).idx[1]);
            tmpmatch1.append(cliqueList1.at(c1).idx[2]);
            iter_conflict++;

            //check if any pair of the edges has been used as initial
            if((edgeMask[tmpmatch0.at(0)][tmpmatch1.at(0)] +
                    edgeMask[tmpmatch0.at(1)][tmpmatch1.at(1)] +
                    edgeMask[tmpmatch0.at(2)][tmpmatch1.at(2)]) > 1){
                trial--;
                continue;
            }
            edgeMask[tmpmatch0.at(0)][tmpmatch1.at(0)]=1;
            edgeMask[tmpmatch0.at(1)][tmpmatch1.at(1)]=1;
            edgeMask[tmpmatch0.at(2)][tmpmatch1.at(2)]=1;

            if(_TESTFLAG)
                cout<<"matched points: "<<trial<<": nt0:"<<tmpmatch0.at(0)<<":"<<tmpmatch0.at(1)<<":"<<tmpmatch0.at(2)<<";"
                   <<"nt1:"<<tmpmatch1.at(0)<<":"<<tmpmatch1.at(1)<<":"<<tmpmatch1.at(2)<<"; match:";

            //perform affine transform
            //double tmpEnergy=-1;
            while(1){
                if(_TESTFLAG)
                    cout<<":"<<tmpmatch0.size();

                double tmp_shift_x=0,tmp_shift_y=0,tmp_shift_z=0,tmp_angle=0,tmp_cent_x=0,tmp_cent_y=0,tmp_cent_z=0;
                if(direction==0) tmp_shift_x=-1;
                if(direction==1) tmp_shift_y=-1;
                if(direction==2) tmp_shift_z=-1;
                QList<XYZ> c0,c1,tmpcoord, tmpdir;
                for(int i=0; i<tmpmatch0.size(); i++){
                    c0.append(XYZ(candcoord0.at(tmpmatch0.at(i))));
                    c1.append(XYZ(candcoord1.at(tmpmatch1.at(i))));
                }
                if(compute_affine_4dof(c0,c1,tmp_shift_x,tmp_shift_y,tmp_shift_z,tmp_angle,tmp_cent_x,tmp_cent_y,tmp_cent_z,direction)<0){
                    break;
                }
                affine_XYZList(candcoord1, tmpcoord, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                affine_XYZList(canddircoord1, tmpdir, tmp_shift_x, tmp_shift_y, tmp_shift_z, tmp_angle, tmp_cent_x, tmp_cent_y, tmp_cent_z, direction);
                minus_XYZList(tmpdir,tmpcoord,tmpdir);
                QList<int> tmpMatchMarkers[2];
                tmpMatchMarkers[0]=QList<int>();
                tmpMatchMarkers[1]=QList<int>();
                getMatchPairs_XYZList(candcoord0, tmpcoord, canddir0, tmpdir, candcomponents0, candcomponents1, candtype0, candtype1, tmpMatchMarkers, pmatchThr, angThr_match);
                //double tmpE = getMatchPairs_XYZList_energy(candcoord0, tmpcoord, canddir0, tmpdir, candcomponents0, candcomponents1, tmpMatchMarkers, pmatchThr, angThr_match);
                if(tmpMatchMarkers[0].size()>tmpmatch0.size()){
                //if(tmpE>tmpEnergy){
                    tmpmatch0=tmpMatchMarkers[0];
                    tmpmatch1=tmpMatchMarkers[1];
                    better_shift_x=tmp_shift_x;
                    better_shift_y=tmp_shift_y;
                    better_shift_z=tmp_shift_z;
                    better_angle=tmp_angle;
                    better_cent_x=tmp_cent_x;
                    better_cent_y=tmp_cent_y;
                    better_cent_z=tmp_cent_z;
                    //tmpEnergy=tmpE;
                }else{
                    break;
                }
            }
            if(_TESTFLAG)
                cout<<endl;

            if(tmpmatch0.size()>bestEnergy){
            //if(tmpEnergy>bestEnergy){
                bestEnergy = tmpmatch0.size();
                //bestEnergy = tmpEnergy;
                candmatch0 = tmpmatch0;
                candmatch1 = tmpmatch1;
//                shift_x = better_shift_x;
//                shift_y = better_shift_y;
//                shift_z = better_shift_z;
//                rotation_ang = better_angle;
//                rotation_cx = better_cent_x;
//                rotation_cy = better_cent_y;
//                rotation_cz = better_cent_z;
                trial=0; //try another 10 times
            }
        }
    }

    for(int i=0; i<candmatch0.size(); i++){
        pmatch0.append(candID0.at(candmatch0.at(i)));
        pmatch1.append(candID1.at(candmatch1.at(i)));
    }
    qDebug()<<"global match: "<<pmatch0.size()<<" matched points found; energy: "<<bestEnergy;
    if(pmatch0.size()>0){
        //affine based on matched points
        affine_nt1();
    }
}

void neuron_match_clique::init()
{
    qDebug()<<"rescale in stacking direction";
    //rescale neurons for matching
    if(zscale!=1){
        if(direction == 0){
            proc_neuron_multiply_factor(nt0, zscale, 1, 1);
            proc_neuron_multiply_factor(nt1, zscale, 1, 1);
        }else if(direction == 1){
            proc_neuron_multiply_factor(nt0, 1, zscale, 1);
            proc_neuron_multiply_factor(nt1, 1, zscale, 1);
        }else{
            proc_neuron_multiply_factor(nt0, 1, 1, zscale);
            proc_neuron_multiply_factor(nt1, 1, 1, zscale);
        }
    }

    qDebug()<<"align in stacking direction";
    if(direction==0)
        shift_x=quickMoveNeuron(nt0, nt1, direction);
    else if(direction==1)
        shift_y=quickMoveNeuron(nt0, nt1, direction);
    else
        shift_z=quickMoveNeuron(nt0, nt1, direction);

    midplane=getNeuronTreeMidplane((*nt0), (*nt1), direction);    

    //find candidates
    qDebug()<<"search candidates";
    initNeuronAndCandidate(*nt0,ng0,neuronType0,candID0,candcoord0,canddir0,canddircoord0,components0,candcomponents0,parent0,1);
    initNeuronAndCandidate(*nt1,ng1,neuronType1,candID1,candcoord1,canddir1,canddircoord1,components1,candcomponents1,parent1,-1);

    //adjust component id of nt1
    int ccmax=0;
    for(int i=0; i<components0.size(); i++){
        ccmax=ccmax>components0.at(i)?ccmax:components0.at(i);
    }
    ccmax+=10;
    for(int i=0; i<components1.size(); i++){
        components1[i]+=ccmax;
    }
    for(int i=0; i<candcomponents1.size(); i++){
        candcomponents1[i]+=ccmax;
    }
    qDebug()<<"init neuron 0: cand:"<<candID0.size();
    qDebug()<<"init neuron 1: cand:"<<candID1.size();

    //find cliques
    qDebug()<<"start find cliques";
    getTopCliques(*nt0,candID0, candcoord0, canddir0, cliqueList0, 0, direction, maxClique3Num);
    getTopCliques(*nt1,candID1, candcoord1, canddir1, cliqueList1, 0, direction, maxClique3Num);
    qDebug()<<"init neuron 0: 3clique:"<<cliqueList0.size();
    qDebug()<<"init neuron 1: 3clique:"<<cliqueList1.size();

    qDebug()<<"start match cliques";

    //find the matched cliques
    matchCliquesAndCands();
}

void neuron_match_clique::init(LandmarkList* mList)
{
    //get markers and register them with the neurons
    load_cand_from_Markers(mList);

    qDebug()<<"rescale in stacking direction";
    //rescale neurons for matching
    if(zscale!=1){
        if(direction == 0){
            proc_neuron_multiply_factor(nt0, zscale, 1, 1);
            proc_neuron_multiply_factor(nt1, zscale, 1, 1);
        }else if(direction == 1){
            proc_neuron_multiply_factor(nt0, 1, zscale, 1);
            proc_neuron_multiply_factor(nt1, 1, zscale, 1);
        }else{
            proc_neuron_multiply_factor(nt0, 1, 1, zscale);
            proc_neuron_multiply_factor(nt1, 1, 1, zscale);
        }
    }

    qDebug()<<"align in stacking direction based on markers";
    if(direction==0)
        shift_x=quickMoveNeuron(nt0, candID0, nt1, candID1, direction);
    else if(direction==1)
        shift_y=quickMoveNeuron(nt0, candID0, nt1, candID1, direction);
    else
        shift_z=quickMoveNeuron(nt0, candID0, nt1, candID1, direction);

    midplane=getNeuronTreeMidplane((*nt0), (*nt1), direction);

    this->initNeuronComponents();

    //update candidates
    qDebug()<<"search candidates";
    update_cand(*nt0,ng0,neuronType0,candID0,parent0,components0,candcoord0,canddir0,canddircoord0,candcomponents0,1);
    update_cand(*nt1,ng1,neuronType1,candID1,parent1,components1,candcoord1,canddir1,canddircoord1,candcomponents1,-1);

    qDebug()<<"init neuron 0: cand:"<<candID0.size();
    qDebug()<<"init neuron 1: cand:"<<candID1.size();

    //find cliques
    qDebug()<<"start find cliques";
    getTopCliques(*nt0,candID0, candcoord0, canddir0, cliqueList0, 0, direction, maxClique3Num);
    getTopCliques(*nt1,candID1, candcoord1, canddir1, cliqueList1, 0, direction, maxClique3Num);
    qDebug()<<"init neuron 0: 3clique:"<<cliqueList0.size();
    qDebug()<<"init neuron 1: 3clique:"<<cliqueList1.size();

    qDebug()<<"start match cliques";

    //find the matched cliques
    matchCliquesAndCands();
}

void neuron_match_clique::output_parameter(QString fname)
{

    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"stack_direction(0:x,1:y,2:z)= "<<direction<<endl;  //stack direction, 0x, 1y, 2z
    myfile<<"candidate_search_plane= "<<midplane<<endl;    //coordinate of the plane in between
    myfile<<"candidate_search_threshold_distance_to_plane= "<<spanCand<<endl;  //searching span from the stack plane for the candidate
    myfile<<"candidate_search_threshold_angle_to_plane(cos(a))= "<<angThr_stack<<endl; //angular threshold for candidate in stack direction (-1 ~ 1), -1 means nothing will be thresholded
    myfile<<"candidate_search_fragments_threshold= "<<segmentThr<<endl;  //threshold to filter out small segments when selecting candidates
    myfile<<"candidate_search_section_gap_threshold= "<<gapThr<<endl;  //threshold to filter out small gaps between segments
    myfile<<"point_match_threshold_distance= "<<pmatchThr<<endl;   //match threshold for points
    myfile<<"point_match_threshold_angular(cos(a))= "<<angThr_match<<endl; //angular threshold for clique match (-1 ~ 1)=cos(theta), when angle is larger than theta (<cos(theta)), the point will not be matched
    myfile<<"triangle_match_threshold_distance= "<<cmatchThr<<endl;   //match threshold for length of cliques
    myfile<<"stack_direction_rescale= "<<zscale<<endl;  //resacle stack direction
    myfile<<"segment_length_for_calculate_direction= "<<dir_range<<endl;   //length of section used to calculate direction of dead end
    myfile<<"spine_length_point_number= "<<spineLengthThr<<endl; //number of maximum points for spine
    myfile<<"spine_angle_threshold= "<<spineAngThr<<endl; //angular threshold for spine turning
    myfile<<"max_number_of_triangles_to_match= "<<maxClique3Num<<endl; //compare less triangles can increase the speed
    myfile<<"match_branches_by_type= "<<(typeConstrain?1:0)<<endl; //if true/1, only match the branch with the same type defined in SWC
    file.close();
}

//update matched point from markers, not necessary need. to do later
void neuron_match_clique::load_cand_from_Markers(LandmarkList * mList)
{
    candID0.clear();
    candID1.clear();

    if(mList->size()<=0){
        return;
    }

    double dis;
    for(int i=0; i<mList->size(); i++){
        double mdis=1e10;
        int nid=-1;
        int pid=0;

        for(int k=0; k<nt0_org->listNeuron.size(); k++){
            dis=NTDIS(nt0_org->listNeuron[k],mList->at(i));
            if(dis<mdis){
                mdis=dis;
                nid=0;
                pid=k;
            }
        }

        for(int k=0; k<nt1_org->listNeuron.size(); k++){
            dis=NTDIS(nt1_org->listNeuron[k],mList->at(i));
            if(dis<mdis){
                mdis=dis;
                nid=1;
                pid=k;
            }
        }

        //push to candidate list
        if(nid==0)
            candID0.append(pid);
        else if(nid==1)
            candID1.append(pid);
    }
}

void neuron_match_clique::update_cand(const NeuronTree& nt, const HBNeuronGraph& ng, const QList<int>& neuronType,
                                      const QList<int>& cand, const QList<int>& pList, const QList<int>& components,
                                      QList<XYZ>& candcoord, QList<XYZ>& canddir, QList<XYZ>& canddircoord, QList<int>& candcomponents, const int orientation)
{
    candcoord.clear();
    canddir.clear();
    canddircoord.clear();
    candcomponents.clear();

    for(VISIOCYTELONG i=0; i<cand.size(); i++){
        XYZ tmpdir(0,0,0);
        VISIOCYTELONG curid = cand[i];
        VISIOCYTELONG nexid, preid=curid;
        if(neuronType.at(cand.at(i))==6){ //normal tips or spine tips
            float lentmp = 0;
            while(lentmp<dir_range && curid>=0){
                if(pList.at(curid)>0)
                    lentmp+=sqrt(NTDIS(nt.listNeuron.at(curid),nt.listNeuron.at(pList.at(curid))));
                nexid=nextPointNeuronGraph(ng,curid,preid);
                preid=curid;
                curid=nexid;
            }
            tmpdir.x = nt.listNeuron.at(cand[i]).x - nt.listNeuron.at(preid).x;
            tmpdir.y = nt.listNeuron.at(cand[i]).y - nt.listNeuron.at(preid).y;
            tmpdir.z = nt.listNeuron.at(cand[i]).z - nt.listNeuron.at(preid).z;
        }else if(neuronType.at(cand.at(i))==56){ //spine fork
            float lentmp = 0;
            while(lentmp<dir_range && curid>=0){
                if(pList.at(curid)>0)
                    lentmp+=sqrt(NTDIS(nt.listNeuron.at(curid),nt.listNeuron.at(pList.at(curid))));
                nexid=-1;
                for(int j=0; j<ng.at(curid).size(); j++){
                    if(ng.at(curid).at(j)==preid) continue;
                    if(neuronType.at(ng.at(curid).at(j))==2 || neuronType.at(ng.at(curid).at(j))== 52){
                        if(nexid==-1){
                            nexid=ng.at(curid).at(j);
                        }else{
                            nexid=-2;
                            break;
                        }
                    }
                }
                preid=curid;
                curid=nexid;
            }
            tmpdir.x = nt.listNeuron.at(cand[i]).x - nt.listNeuron.at(preid).x;
            tmpdir.y = nt.listNeuron.at(cand[i]).y - nt.listNeuron.at(preid).y;
            tmpdir.z = nt.listNeuron.at(cand[i]).z - nt.listNeuron.at(preid).z;
        }

        //normalize direction
        double tmpNorm = sqrt(tmpdir.x*tmpdir.x+tmpdir.y*tmpdir.y+tmpdir.z*tmpdir.z);
        if(tmpNorm<1e-16) tmpNorm=1e-16;
        tmpdir.x/=tmpNorm; tmpdir.x*=orientation;
        tmpdir.y/=tmpNorm; tmpdir.y*=orientation;
        tmpdir.z/=tmpNorm; tmpdir.z*=orientation;

        //for test
        //qDebug()<<candcoord.size()<<":"<<cand[i]<<":"<<neuronType.at(cand.at(i))<<"; dir"<<tmpdir.x<<":"<<tmpdir.y<<":"<<tmpdir.z<<":"<<tmpNorm;

        candcoord.append(XYZ(nt.listNeuron.at(cand[i])));
        canddir.append(tmpdir);
        canddircoord.append(XYZ(candcoord.last().x+canddir.last().x,candcoord.last().y+canddir.last().y,candcoord.last().z+canddir.last().z));
        candcomponents.append(components.at(cand[i]));
    }
}

void neuron_match_clique::update_matchedPoints_to_Markers(LandmarkList * mList)
{
    mList->clear();
    int idx=0;
    //matched candidates first
    for(int i=0; i<pmatch0.size(); i++){
        int a=pmatch0.at(i);
        int aa=candmatch0.at(i);
        LocationSimple S0 = LocationSimple(nt0_stitch->listNeuron[a].x,
                                          nt0_stitch->listNeuron[a].y,
                                          nt0_stitch->listNeuron[a].z);
        S0.color.r = 255; S0.color.g = 0; S0.color.b = 0;
        S0.name = QString::number(idx++).toStdString();
        QString c0 = "0 "+QString::number(candID0.at(aa))+" "+QString::number(idx);

        if(_TESTFLAG) //for test
            c0 = c0 + " " + QString::number(neuronType0.at(candID0.at(aa))) + " " + QString::number(aa);

        S0.comments = c0.toStdString();
        mList->append(S0);

        int b=pmatch1.at(i);
        int bb=candmatch1.at(i);
        LocationSimple S1 = LocationSimple(nt1_stitch->listNeuron[b].x,
                                          nt1_stitch->listNeuron[b].y,
                                          nt1_stitch->listNeuron[b].z);
        S1.color.r = 0; S1.color.g = 255; S1.color.b = 0;
        S1.name = QString::number(idx++).toStdString();
        QString c1 = "1 "+QString::number(candID1.at(bb))+" "+QString::number(idx-2);

        if(_TESTFLAG) //for test
            c1 = c1 + " " + QString::number(neuronType1.at(candID1.at(bb))) + " " + QString::number(bb);

        S1.comments = c1.toStdString();
        mList->append(S1);
    }
    //then unmatched
    for(int i=0; i<candID0.size(); i++){
        if(candmatch0.contains(i)) continue;
        LocationSimple S = LocationSimple(nt0_stitch->listNeuron[candID0.at(i)].x,
                nt0_stitch->listNeuron[candID0.at(i)].y,
                nt0_stitch->listNeuron[candID0.at(i)].z);
        S.name = QString::number(idx++).toStdString();
        QString ctmp = "0 "+QString::number(candID0.at(i))+" -1 ";

        if(_TESTFLAG) //for test
            ctmp = ctmp + " " + QString::number(neuronType0.at(candID0.at(i))) + " " + QString::number(i);

        S.comments = ctmp.toStdString();
        S.color.r = 128; S.color.g = 0; S.color.b = 128;
        mList->append(S);
    }
    for(int i=0; i<candID1.size(); i++){
        if(candmatch1.contains(i)) continue;
        LocationSimple S = LocationSimple(nt1_stitch->listNeuron[candID1.at(i)].x,
                nt1_stitch->listNeuron[candID1.at(i)].y,
                nt1_stitch->listNeuron[candID1.at(i)].z);
        S.name = QString::number(idx++).toStdString();
        QString ctmp = "1 "+QString::number(candID1.at(i))+" -1";

        if(_TESTFLAG) //for test
            ctmp = ctmp + " " + QString::number(neuronType1.at(candID1.at(i))) + " " + QString::number(i);

        S.comments = ctmp.toStdString();
        S.color.r = 0; S.color.g = 128; S.color.b = 128;
        mList->append(S);
    }
}

void neuron_match_clique::output_candMatchScore(QString fname)
{
    QList<int> xs, ys;
    for(int i=0; i<candmatch0.size(); i++){
        xs.append(candmatch0.at(i));
        ys.append(candmatch1.at(i));
    }
    for(int i=0; i<candID0.size(); i++){
        if(!xs.contains(i))
            xs.append(i);
    }
    for(int i=0; i<candID1.size(); i++){
        if(!ys.contains(i))
            ys.append(i);
    }

    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
//    {//cojoc: for test
//        myfile<<"0\t";
//        for(int j=0; j<candID1.size(); j++){
//            myfile<<candID1.at(j)<<"\t";
//        }
//        myfile<<endl;
//        for(int i=0; i<candID0.size(); i++){
//            myfile<<candID0.at(i)<<"\t";
//            for(int j=0; j<candID1.size(); j++){
//                myfile<<CANDMS_ENTRY(i,j)<<"\t";
//            }
//            myfile<<endl;
//        }
//    }
    myfile<<"0\t";
    for(int j=0; j<candmatch0.size(); j++){
        myfile<<j*2+2<<"\t";
    }
    for(int j=candmatch0.size()+MS_x; j<MS_x+MS_y; j++){
        myfile<<j+1<<"\t";
    }
    myfile<<endl;
    for(int i=0; i<xs.size(); i++){
        if(i<candmatch0.size())
            myfile<<i*2+1<<"\t";
        else
            myfile<<i+candmatch0.size()+1<<"\t";
        for(int j=0; j<ys.size(); j++){
            myfile<<CANDMS_ENTRY(xs.at(i),ys.at(j))<<"\t";
        }
        myfile<<endl;
    }
    file.close();
}

void neuron_match_clique::output_matchedMarkers(QString fname, const NeuronTree& nt, QList<int> points)
{
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    int idx=0;
    //matched candidates first
    for(int i=0; i<points.size(); i++){
        int a=points.at(i);
        myfile<<nt.listNeuron[a].x<<","<<nt.listNeuron[a].y<<","<<nt.listNeuron[a].z<<",0,1, "<<idx++<<", "
             <<", 255, 0, 0"<<endl;
    }
    file.close();
}

void neuron_match_clique::output_matchedMarkers_orgspace(QString fname_0, QString fname_1)
{
    QFile file(fname_0);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname_0 +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    int idx=0;
    //matched candidates first
    for(int i=0; i<pmatch0.size(); i++){
        int a=pmatch0.at(i);
        int aa=candmatch0.at(i);
        myfile<<nt0_org->listNeuron[a].x<<","<<nt0_org->listNeuron[a].y<<","<<nt0_org->listNeuron[a].z<<",0,1, "<<idx<<","
             <<"0 "<<candID0.at(aa)<<" -1 "
             <<canddir0[aa].x<<" "<<canddir0[aa].y<<" "<<canddir0[aa].z<<", 255, 0, 0"<<endl;
        idx++;
    }
    file.close();

    QFile file1(fname_1);
    if (!file1.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname_1 +" for write");
        return;
    }
    QTextStream myfile1(&file1);
    myfile1<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    idx=0;
    for(int i=0; i<pmatch0.size(); i++){
        int b=pmatch1.at(i);
        int bb=candmatch1.at(i);
        myfile1<<nt1_org->listNeuron[b].x<<","<<nt1_org->listNeuron[b].y<<","<<nt1_org->listNeuron[b].z<<",0,1, "<<idx<<","
             <<"1 "<<candID1.at(bb)<<" -1 "
             <<canddir1[bb].x<<" "<<canddir1[bb].y<<" "<<canddir1[bb].z<<", 0, 255, 0"<<endl;
        idx++;
    }
    file1.close();
}

void neuron_match_clique::output_markers_candspace(QString fname)
{
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    int idx=0;
    //matched candidates first
    for(int i=0; i<candmatch0.size(); i++){
        int a=candmatch0.at(i);
        myfile<<candcoord0[a].x<<","<<candcoord0[a].y<<","<<candcoord0[a].z<<",0,1, "<<idx<<","
             <<"0 "<<candID0.at(a)<<" "<<idx+1<<" "
             <<canddir0[a].x<<" "<<canddir0[a].y<<" "<<canddir0[a].z<<", 255, 0, 0"<<endl;
        idx++;

        int b=candmatch1.at(i);
        myfile<<candcoord1[b].x<<","<<candcoord1[b].y<<","<<candcoord1[b].z<<",0,1, "<<idx<<","
             <<"1 "<<candID1.at(b)<<" "<<idx-1<<" "
             <<canddir1[b].x<<" "<<canddir1[b].y<<" "<<canddir1[b].z<<", 0, 255, 0"<<endl;
        idx++;
    }
    //then unmatched
    for(int i=0; i<candID0.size(); i++){
        if(candmatch0.contains(i)) continue;
        myfile<<candcoord0[i].x<<","<<candcoord0[i].y<<","<<candcoord0[i].z<<",0,1, "<<idx++<<","
             <<"0 "<<candID0.at(i)<<" -1 "
             <<canddir0[i].x<<" "<<canddir0[i].y<<" "<<canddir0[i].z<<", 128, 0, 128"<<endl;
    }
    for(int i=0; i<candID1.size(); i++){
        if(candmatch1.contains(i)) continue;
        myfile<<candcoord1[i].x<<","<<candcoord1[i].y<<","<<candcoord1[i].z<<",0,1, "<<idx++<<","
             <<"1 "<<candID1.at(i)<<" -1 "
             <<canddir1[i].x<<" "<<canddir1[i].y<<" "<<canddir1[i].z<<", 0, 128, 128"<<endl;
    }
    file.close();
}

void neuron_match_clique::output_markers_orgspace(QString fname)
{
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    int idx=0;
    //matched candidates first
    for(int i=0; i<pmatch0.size(); i++){
        int a=pmatch0.at(i);
        int aa=candmatch0.at(i);
        myfile<<nt0_org->listNeuron[a].x<<","<<nt0_org->listNeuron[a].y<<","<<nt0_org->listNeuron[a].z<<",0,1, "<<idx<<","
             <<"0 "<<candID0.at(aa)<<" "<<idx+1<<" "
             <<canddir0[aa].x<<" "<<canddir0[aa].y<<" "<<canddir0[aa].z<<", 255, 0, 0"<<endl;
        idx++;

        int b=pmatch1.at(i);
        int bb=candmatch1.at(i);
        myfile<<nt1_org->listNeuron[b].x<<","<<nt1_org->listNeuron[b].y<<","<<nt1_org->listNeuron[b].z<<",0,1, "<<idx<<","
             <<"1 "<<candID1.at(bb)<<" "<<idx-1<<" "
             <<canddir1[bb].x<<" "<<canddir1[bb].y<<" "<<canddir1[bb].z<<", 0, 255, 0"<<endl;
        idx++;
    }
    //then unmatched
    for(int i=0; i<candID0.size(); i++){
        if(candmatch0.contains(i)) continue;
        myfile<<nt0_org->listNeuron[candID0.at(i)].x<<","<<nt0_org->listNeuron[candID0.at(i)].y<<","<<nt0_org->listNeuron[candID0.at(i)].z<<",0,1, "<<idx++<<","
             <<"0 "<<candID0.at(i)<<" -1 "
             <<canddir0[i].x<<" "<<canddir0[i].y<<" "<<canddir0[i].z<<", 128, 0, 128"<<endl;
    }
    for(int i=0; i<candID1.size(); i++){
        if(candmatch1.contains(i)) continue;
        myfile<<nt1_org->listNeuron[candID1.at(i)].x<<","<<nt1_org->listNeuron[candID1.at(i)].y<<","<<nt1_org->listNeuron[candID1.at(i)].z<<",0,1, "<<idx++<<","
             <<"1 "<<candID1.at(i)<<" -1 "
             <<canddir1[i].x<<" "<<canddir1[i].y<<" "<<canddir1[i].z<<", 0, 128, 128"<<endl;
    }
    file.close();
}

void neuron_match_clique::output_markers_affinespace(QString fname)
{
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    int idx=0;
    //matched candidates first
    for(int i=0; i<pmatch0.size(); i++){
        int a=pmatch0.at(i);
        int aa=candmatch0.at(i);
        myfile<<nt0_org->listNeuron[a].x<<","<<nt0_org->listNeuron[a].y<<","<<nt0_org->listNeuron[a].z<<",0,1, "<<idx<<","
             <<"0 "<<candID0.at(aa)<<" "<<idx+1<<" "
             <<canddir0[aa].x<<" "<<canddir0[aa].y<<" "<<canddir0[aa].z<<", 255, 0, 0"<<endl;
        idx++;

        int b=pmatch1.at(i);
        int bb=candmatch1.at(i);
        myfile<<nt1_a->listNeuron[b].x<<","<<nt1_a->listNeuron[b].y<<","<<nt1_a->listNeuron[b].z<<",0,1, "<<idx<<","
             <<"1 "<<candID1.at(bb)<<" "<<idx-1<<" "
             <<canddir1[bb].x<<" "<<canddir1[bb].y<<" "<<canddir1[bb].z<<", 0, 255, 0"<<endl;
        idx++;
    }
    //then unmatched
    for(int i=0; i<candID0.size(); i++){
        if(candmatch0.contains(i)) continue;
        myfile<<nt0_org->listNeuron[candID0.at(i)].x<<","<<nt0_org->listNeuron[candID0.at(i)].y<<","<<nt0_org->listNeuron[candID0.at(i)].z<<",0,1, "<<idx++<<","
             <<"0 "<<candID0.at(i)<<" -1 "
             <<canddir0[i].x<<" "<<canddir0[i].y<<" "<<canddir0[i].z<<", 128, 0, 128"<<endl;
    }
    for(int i=0; i<candID1.size(); i++){
        if(candmatch1.contains(i)) continue;
        myfile<<nt1_a->listNeuron[candID1.at(i)].x<<","<<nt1_a->listNeuron[candID1.at(i)].y<<","<<nt1_a->listNeuron[candID1.at(i)].z<<",0,1, "<<idx++<<","
             <<"1 "<<candID1.at(i)<<" -1 "
             <<canddir1[i].x<<" "<<canddir1[i].y<<" "<<canddir1[i].z<<", 0, 128, 128"<<endl;
    }
    file.close();
}

void neuron_match_clique::output_markers_stitchspace(QString fname)
{
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        visiocyte_msg("cannot open "+ fname +" for write");
        return;
    }
    QTextStream myfile(&file);
    myfile<<"##x,y,z,radius,shape,name,comment, color_r,color_g,color_b"<<endl;
    int idx=0;
    //matched candidates first
    for(int i=0; i<pmatch0.size(); i++){
        int a=pmatch0.at(i);
        int aa=candmatch0.at(i);
        myfile<<nt0_stitch->listNeuron[a].x<<","<<nt0_stitch->listNeuron[a].y<<","<<nt0_stitch->listNeuron[a].z<<",0,1, "<<idx<<","
             <<"0 "<<candID0.at(aa)<<" "<<idx+1<<" "
             <<canddir0[aa].x<<" "<<canddir0[aa].y<<" "<<canddir0[aa].z<<", 255, 0, 0"<<endl;
        idx++;

        int b=pmatch1.at(i);
        int bb=candmatch1.at(i);
        myfile<<nt1_a->listNeuron[b].x<<","<<nt1_a->listNeuron[b].y<<","<<nt1_a->listNeuron[b].z<<",0,1, "<<idx<<","
             <<"1 "<<candID1.at(bb)<<" "<<idx-1<<" "
             <<canddir1[bb].x<<" "<<canddir1[bb].y<<" "<<canddir1[bb].z<<", 0, 255, 0"<<endl;
        idx++;
    }
    //then unmatched
    for(int i=0; i<candID0.size(); i++){
        if(candmatch0.contains(i)) continue;
        myfile<<nt0_stitch->listNeuron[candID0.at(i)].x<<","<<nt0_stitch->listNeuron[candID0.at(i)].y<<","<<nt0_stitch->listNeuron[candID0.at(i)].z<<",0,1, "<<idx++<<","
             <<"0 "<<candID0.at(i)<<" -1 "
             <<canddir0[i].x<<" "<<canddir0[i].y<<" "<<canddir0[i].z<<", 128, 0, 128"<<endl;
    }
    for(int i=0; i<candID1.size(); i++){
        if(candmatch1.contains(i)) continue;
        myfile<<nt1_a->listNeuron[candID1.at(i)].x<<","<<nt1_a->listNeuron[candID1.at(i)].y<<","<<nt1_a->listNeuron[candID1.at(i)].z<<",0,1, "<<idx++<<","
             <<"1 "<<candID1.at(i)<<" -1 "
             <<canddir1[i].x<<" "<<canddir1[i].y<<" "<<canddir1[i].z<<", 0, 128, 128"<<endl;
    }
    file.close();
}

void neuron_match_clique::output_stitch(QString fname)
{
//    if(candmatch0.size()<=0)
//        return;
    if(nt0_stitch->listNeuron.size()<=0)
        return;
    if(nt1_stitch->listNeuron.size()<=0)
        return;

    bool eswc0_flag=false;
    bool eswc1_flag=false;

    QString fname_neuron0;
    if(nt0_org->file.section('.',-1).toUpper() == "ESWC"){
        fname_neuron0 = fname+"_stitched_nt0.eswc";
        eswc0_flag=true;
    }
    else
        fname_neuron0 = fname+"_stitched_nt0.swc";
    //output neuron
    if (!export_list2file(nt0_stitch->listNeuron,fname_neuron0,nt0_org->name))
    {
        visiocyte_msg("fail to write the output swc file:\n" + fname_neuron0);
        return;
    }

    QString fname_neuron1;
    if(nt1_org->file.section('.',-1).toUpper() == "ESWC"){
        eswc1_flag=true;
        fname_neuron1 = fname+"_stitched_nt1.eswc";
    }
    else
        fname_neuron1 = fname+"_stitched_nt1.swc";
    //output neuron
    if (!export_list2file(nt1_stitch->listNeuron,fname_neuron1,nt1_org->name))
    {
        visiocyte_msg("fail to write the output swc file:\n" + fname_neuron1);
        return;
    }

    //combine neurons and output
    QList<NeuronSWC> combined;
    int idmax=0;
    for(int i=0; i<nt0_stitch->listNeuron.size(); i++){
        NeuronSWC S;
        S.x = nt0_stitch->listNeuron.at(i).x;
        S.y = nt0_stitch->listNeuron.at(i).y;
        S.z = nt0_stitch->listNeuron.at(i).z;
        S.n = nt0_stitch->listNeuron.at(i).n;
        S.pn = nt0_stitch->listNeuron.at(i).pn;
        S.r = nt0_stitch->listNeuron.at(i).r;
        S.type = swcType0.at(i);
        if(eswc0_flag){
            S.seg_id = nt0_org->listNeuron.at(i).seg_id;
            S.level= nt0_org->listNeuron.at(i).level;
            S.fea_val = nt0_org->listNeuron.at(i).fea_val;
        }
        combined.append(S);
        idmax=idmax>S.n?idmax:S.n;
        idmax=idmax>S.pn?idmax:S.pn;
    }
    for(int i=0; i<nt1_stitch->listNeuron.size(); i++){
        NeuronSWC S;
        S.x = nt1_stitch->listNeuron.at(i).x;
        S.y = nt1_stitch->listNeuron.at(i).y;
        S.z = nt1_stitch->listNeuron.at(i).z;
        S.n = nt1_stitch->listNeuron.at(i).n+idmax+1;
        S.r = nt1_stitch->listNeuron.at(i).r;
        S.type = swcType1.at(i);
        if(eswc1_flag){
            S.seg_id = nt1_org->listNeuron.at(i).seg_id;
            S.level= nt1_org->listNeuron.at(i).level;
            S.fea_val = nt1_org->listNeuron.at(i).fea_val;
        }
        if(nt1_stitch->listNeuron.at(i).pn>=0)
            S.pn = nt1_stitch->listNeuron.at(i).pn+idmax+1;
        else
            S.pn=-1;
        combined.append(S);
    }
    QString fname_neuron;
    if(eswc0_flag || eswc1_flag)
        fname_neuron = fname+"_stitched.eswc";
    else
        fname_neuron = fname+"_stitched.swc";
    if (!export_list2file(combined,fname_neuron,nt0_org->file + " + " + nt1_org->file))
    {
        visiocyte_msg("fail to write the output swc file:\n" + fname_neuron);
        return;
    }

    //output markers
    QString fname_marker = fname+"_stitched.marker";
    output_matchedMarkers(fname_marker, *nt0_stitch, pmatch0);
    QString fname_allmarker = fname+"_stitch_all.marker";
    output_markers_stitchspace(fname_allmarker);

    //output ano
    QString fname_ano = fname+"_stitched.ano";
    QFile qf_anofile(fname_ano);
    if(!qf_anofile.open(QIODevice::WriteOnly))
    {
        visiocyte_msg("Cannot open file for writing!");
        return;
    }
    QTextStream out(&qf_anofile);
    out<<"SWCFILE="<<fname_neuron0<<endl;
    out<<"SWCFILE="<<fname_neuron1<<endl;
    qf_anofile.close();
}

void neuron_match_clique::output_affine(QString fname, QString fname_nt0)
{
    if(candmatch0.size()<=0)
        return;

    QString fname_neuron;
    if(nt1_org->file.section('.',-1).toUpper() == "ESWC")
        fname_neuron = fname+"_affine.eswc";
    else
        fname_neuron = fname+"_affine.swc";

    //output neuron
    if (!export_list2file(nt1_a->listNeuron,fname_neuron,nt1_org->file))
    {
        visiocyte_msg("fail to write the output swc file:\n" + fname_neuron);
        return;
    }

    //output markers
    QString fname_marker = fname+"_affine.marker";
    output_markers_affinespace(fname_marker);

    //output amat
    double amat[16];
    getAffineAmat(amat, shift_x, shift_y, shift_z, rotation_ang, rotation_cx, rotation_cy, rotation_cz, direction);
    QString fname_amat = fname+"_amat.txt";
    writeAmat(fname_amat.toStdString().c_str(), amat);

    //output ano
    QString fname_ano = fname+"_affine.ano";
    QFile qf_anofile(fname_ano);
    if(!qf_anofile.open(QIODevice::WriteOnly))
    {
        visiocyte_msg("Cannot open file for writing!");
        return;
    }
    QTextStream out(&qf_anofile);
    out<<"SWCFILE="<<fname_nt0<<endl;
    out<<"SWCFILE="<<fname_neuron<<endl;
    qf_anofile.close();
}

void neuron_match_clique::affine_nt1()
{
    if(pmatch0.size()<=0){
        qDebug()<<"not match point identified, quit align";
        return;
    }

    copyCoordinate(*nt1_org, *nt1_a);
    double shift_stack = quickMoveNeuron(nt0_org, nt1_a, direction);

    QList<XYZ> c0, c1;
    for(int i=0; i<pmatch0.size(); i++){
        c0.append(XYZ(nt0_org->listNeuron.at(pmatch0.at(i))));
        c1.append(XYZ(nt1_a->listNeuron.at(pmatch1.at(i))));
    }

    double tmp_shift_x=0,tmp_shift_y=0,tmp_shift_z=0,tmp_angle=0,tmp_cent_x=0,tmp_cent_y=0,tmp_cent_z=0;
    if(direction==0) tmp_shift_x=-1;
    if(direction==1) tmp_shift_y=-1;
    if(direction==2) tmp_shift_z=-1;
    if(compute_affine_4dof(c0,c1,tmp_shift_x,tmp_shift_y,tmp_shift_z,tmp_angle,tmp_cent_x,tmp_cent_y,tmp_cent_z,direction)<0){
        return;
    }

    affineNeuron(*nt1_a,*nt1_a,tmp_shift_x,tmp_shift_y,tmp_shift_z,tmp_angle,tmp_cent_x,tmp_cent_y,tmp_cent_z,direction);
    copyCoordinate(*nt1_a,*nt1_stitch);
    if(direction==0) tmp_shift_x=shift_stack;
    if(direction==1) tmp_shift_y=shift_stack;
    if(direction==2) tmp_shift_z=shift_stack;
    shift_x = tmp_shift_x;
    shift_y = tmp_shift_y;
    shift_z = tmp_shift_z;
    rotation_ang = tmp_angle;
    rotation_cx = tmp_cent_x;
    rotation_cy = tmp_cent_y;
    rotation_cz = tmp_cent_z;
}

void neuron_match_clique::matchCliquesAndCands()
{
    rankedCliqueMatch.clear();
    candMS.clear();
    MS_x=candID0.size();
    MS_y=candID1.size();

    candMS.resize(MS_x*MS_y,0);
    QList<int> candtype0, candtype1;
    if(typeConstrain){
        for(VISIOCYTELONG i=0; i<candID0.size(); i++){
            candtype0.append(swcType0.at(candID0.at(i)));
        }
        for(VISIOCYTELONG i=0; i<candID1.size(); i++){
            candtype1.append(swcType1.at(candID1.at(i)));
        }
    }else{
        for(VISIOCYTELONG i=0; i<candID0.size(); i++){
            candtype0.append(1);
        }
        for(VISIOCYTELONG i=0; i<candID1.size(); i++){
            candtype1.append(1);
        }
    }
//    candMS=QVector(MS_x*MS_y,0);

    QVector<QVector<int> > cliquePairs;
    for(int i=0; i<cliqueList0.size(); i++){
        for(int j=0; j<cliqueList1.size(); j++){
            //find matching clique
            if(candtype0.at(cliqueList0[i].idx[0]) != candtype1.at(cliqueList1[j].idx[0])) continue;
            if(candtype0.at(cliqueList0[i].idx[1]) != candtype1.at(cliqueList1[j].idx[1])) continue;
            if(candtype0.at(cliqueList0[i].idx[2]) != candtype1.at(cliqueList1[j].idx[2])) continue;
            if(fabs(cliqueList0[i].e[0]-cliqueList1[j].e[0])>cmatchThr) continue;
            if(fabs(cliqueList0[i].e[1]-cliqueList1[j].e[1])>cmatchThr) continue;
            if(fabs(cliqueList0[i].e[2]-cliqueList1[j].e[2])>cmatchThr) continue;
            if(NTDOT(cliqueList0[i].dir[0],cliqueList1[j].dir[0])<angThr_match) continue;
            if(NTDOT(cliqueList0[i].dir[1],cliqueList1[j].dir[1])<angThr_match) continue;
            if(NTDOT(cliqueList0[i].dir[2],cliqueList1[j].dir[2])<angThr_match) continue;

            QVector<int> pair;
            pair<<i<<j;
            cliquePairs<<pair;
            for(int k=0; k<3; k++)
                CANDMS_ENTRY(cliqueList0[i].idx[k],cliqueList1[j].idx[k])++;
        }
    }

    qDebug()<<"found "<<cliquePairs.size()<<" pair of cliques";

    for(int i=0; i<cliquePairs.size(); i++){
        int matchscore=0;
        for(int k=0; k<3; k++){
            matchscore+=CANDMS_ENTRY(cliqueList0[cliquePairs[i].at(0)].idx[k],cliqueList1[cliquePairs[i].at(1)].idx[k]);
        }
        rankedCliqueMatch.insert(pair<int, QVector<int> >(matchscore,cliquePairs[i]) );
    }
}

void neuron_match_clique::stitch()
{
    for(int pid=0; pid<pmatch0.size(); pid++){
        if(components0[pmatch0.at(pid)]==components1[pmatch1.at(pid)]) //loop
            continue;

        //stitchMatchedPoint(nt0_stitch, nt1_stitch, parent0, parent1, pmatch0.at(pid), pmatch1.at(pid));
        stitchMatchedPoint(nt0_stitch, nt1_stitch, ng0, ng1, neuronType0, neuronType1, pmatch0.at(pid), pmatch1.at(pid));

        //update cand components
        int cid=components1[pmatch1.at(pid)];
        int nid=components0[pmatch0.at(pid)];
        int idx=components0.indexOf(cid);
        while(idx>=0){
            components0[idx]=nid;
            idx=components0.indexOf(cid,idx+1);
        }
        idx=candcomponents0.indexOf(cid);
        while(idx>=0){
            candcomponents0[idx]=nid;
            idx=candcomponents0.indexOf(cid, idx+1);
        }
        idx=components1.indexOf(cid);
        while(idx>=0){
            components1[idx]=nid;
            idx=components1.indexOf(cid,idx+1);
        }
        idx=candcomponents1.indexOf(cid);
        while(idx>=0){
            candcomponents1[idx]=nid;
            idx=candcomponents1.indexOf(cid, idx+1);
        }
    }
}

bool neuron_match_clique::stitch(int point0, int point1)
{
    if(components0.size()>0 && components1.size()>0)
        if(checkloop(point0, point1))
            return false;

    //stitch
    //stitchMatchedPoint(nt0_stitch, nt1_stitch, parent0, parent1, point0, point1);
    stitchMatchedPoint(nt0_stitch, nt1_stitch, ng0, ng1, neuronType0, neuronType1, point0, point1);

    //update cand components
    int cid=components1[point1];
    int nid=components0[point0];
    int idx=components0.indexOf(cid);
    while(idx>=0){
        components0[idx]=nid;
        idx=components0.indexOf(cid,idx+1);
    }
    idx=candcomponents0.indexOf(cid);
    while(idx>=0){
        candcomponents0[idx]=nid;
        idx=candcomponents0.indexOf(cid, idx+1);
    }
    idx=components1.indexOf(cid);
    while(idx>=0){
        components1[idx]=nid;
        idx=components1.indexOf(cid,idx+1);
    }
    idx=candcomponents1.indexOf(cid);
    while(idx>=0){
        candcomponents1[idx]=nid;
        idx=candcomponents1.indexOf(cid, idx+1);
    }

    return true;
}

bool neuron_match_clique::checkloop(int point0, int point1)
{
    if(components0.at(point0) == components1.at(point1)){
        return true;
    }
    return false;
}

void neuron_match_clique::highlight_nt1_seg(int point1, int type)
{
    int cid=components1.at(point1);
    int idx=components1.indexOf(cid);
    while(idx>=0){
        NeuronSWC* p = (NeuronSWC*)&(nt1_stitch->listNeuron.at(idx));
        if(type>=0){
            p->type=type;
        }else{
            p->type=swcType1.at(idx);
        }
        idx=components1.indexOf(cid, idx+1);
    }
}

void neuron_match_clique::highlight_nt0_seg(int point0, int type)
{
    int cid=components0.at(point0);
    int idx=components0.indexOf(cid);
    while(idx>=0){
        NeuronSWC* p = (NeuronSWC*)&(nt0_stitch->listNeuron.at(idx));
        if(type>=0){
            p->type=type;
        }else{
            p->type=swcType0.at(idx);
        }
        idx=components0.indexOf(cid, idx+1);
    }
}

VISIOCYTELONG neuron_match_clique::search_tip0(VISIOCYTELONG point0) //find the id of closest tips point, if there is none, return -1
{
    return findNearestTips(ng0, neuronType0, point0);
}

VISIOCYTELONG neuron_match_clique::search_tip1(VISIOCYTELONG point1) //find the id of closest tips point, if there is none, return -1
{
    return findNearestTips(ng1, neuronType1, point1);

}

////orientation should be 1/-1 for smaller/larger stack in direction
//void neuron_match_clique::initNeuronAndCandidate(NeuronTree& nt, QList<int>& cand, QList<XYZ>& candcoord,
//                                                 QList<XYZ>& canddir, QList<XYZ>& canddircoord, QList<int>& components,
//                                                 QList<int>& candcomponents, QList<int>& pList, int orientation)
//{
//    float min=midplane-spanCand;
//    float max=midplane+spanCand;

//    cand.clear();
//    candcoord.clear();
//    canddir.clear();
//    canddircoord.clear();
//    candcomponents.clear();

//    QVector<int> childNum(nt.listNeuron.size(), 0);
//    QVector<int> connNum(nt.listNeuron.size(), 0);
//    QVector<double> sectionLength(nt.listNeuron.size(), 0);
//    //QList<int> components;
//    components.clear();
//    pList.clear();
//    QVector<VISIOCYTELONG> componentSize;
//    QVector<VISIOCYTELONG> componentLength;
//    VISIOCYTELONG curid=0;
//    for(VISIOCYTELONG i=0; i<nt.listNeuron.size(); i++){
//        if(nt.listNeuron.at(i).pn<0){
//            connNum[i]--; //root that only have 1 clide will also be a dead end
//            components.append(curid); curid++;
//            pList.append(-1);
//        }
//        else{
//            int pid = nt.hashNeuron.value(nt.listNeuron.at(i).pn);
//            childNum[pid]++;
//            connNum[pid]++;
//            sectionLength[i]=sqrt(NTDIS(nt.listNeuron.at(i),nt.listNeuron.at(pid)));
//            components.append(-1);
//            pList.append(pid);
//        }
//    }
//    //connected component
//    for(VISIOCYTELONG cid=0; cid<curid; cid++){
//        QStack<int> pstack;
//        int chid, size = 0;
//        if(!components.contains(cid)) //should not happen, just in case
//            continue;
//        if(components.indexOf(cid)!=components.lastIndexOf(cid)) //should not happen
//            qDebug("unexpected multiple tree root, please check the code: neuron_stitch_func.cpp");
//        //recursively search for child and mark them as the same component
//        pstack.push(components.indexOf(cid));
//        size++;
//        while(!pstack.isEmpty()){
//            int pid=pstack.pop();
//            chid = -1;
//            chid = pList.indexOf(pid,chid+1);
//            while(chid>=0){
//                pstack.push(chid);
//                components[chid]=cid;
//                chid=pList.indexOf(pid,chid+1);
//                size++;
//            }
//        }
//        componentSize.append(size);
//    }
//    //component size
//    for(VISIOCYTELONG cid=0; cid<curid; cid++){
//        double length = 0;
//        int idx = -1;
//        for(VISIOCYTELONG i=0; i<componentSize[cid]; i++){
//            idx = components.indexOf(cid,idx+1);
//            length+=sectionLength[idx];
//        }
//        componentLength.append(length);
//    }


//    QList<int> tmpcand; tmpcand.clear();
//    for(VISIOCYTELONG i=0; i<childNum.size(); i++){
//        if(connNum[i]<=0 && componentLength[components[i]]>=segmentThr){
//            if(direction==0){//x
//                if(nt.listNeuron.at(i).x>min && nt.listNeuron.at(i).x<max){
//                    tmpcand.append(i);
//                }
//            }else if(direction==1){//y
//                if(nt.listNeuron.at(i).y>min && nt.listNeuron.at(i).y<max){
//                    tmpcand.append(i);
//                }
//            }else if(direction==2){//z
//                if(nt.listNeuron.at(i).z>min && nt.listNeuron.at(i).z<max){
//                    tmpcand.append(i);
//                }
//            }else{//all tips
//                tmpcand.append(i);
//            }
//        }
//    }

//    for(VISIOCYTELONG i=0; i<tmpcand.size(); i++){
//        XYZ tmpdir(0,0,0);
//        int id = tmpcand[i];
//        if(childNum[id]==1){ //single child root
//            float lentmp = 0;
//            while(lentmp<dir_range && id>=0){
//                int sid = pList.indexOf(id);
//                if(sid<0) break;
//                lentmp+=sectionLength[sid];
//                tmpdir.x += nt.listNeuron.at(id).x-nt.listNeuron.at(sid).x;
//                tmpdir.y += nt.listNeuron.at(id).y-nt.listNeuron.at(sid).y;
//                tmpdir.z += nt.listNeuron.at(id).z-nt.listNeuron.at(sid).z;
//                id=sid;
//            }
//        }else{ //tips
//            float lentmp = 0;
//            while(lentmp<dir_range && id>=0){
//                int sid = pList[id];
//                if(sid<0) break;
//                lentmp+=sectionLength[id];
//                tmpdir.x += nt.listNeuron.at(id).x-nt.listNeuron.at(sid).x;
//                tmpdir.y += nt.listNeuron.at(id).y-nt.listNeuron.at(sid).y;
//                tmpdir.z += nt.listNeuron.at(id).z-nt.listNeuron.at(sid).z;
//                id=sid;
//            }
//        }
//        //normalize direction
//        double tmpNorm = sqrt(tmpdir.x*tmpdir.x+tmpdir.y*tmpdir.y+tmpdir.z*tmpdir.z);
//        tmpdir.x/=tmpNorm; tmpdir.x*=orientation;
//        tmpdir.y/=tmpNorm; tmpdir.y*=orientation;
//        tmpdir.z/=tmpNorm; tmpdir.z*=orientation;

//        //judgement to the direction to avoid the connection that moves from plan to
//        double sa = 1;
//        if(direction==0){
//            sa = tmpdir.x;
//        }else if(direction == 1){
//            sa = tmpdir.y;
//        }else if(direction == 2){
//            sa = tmpdir.z;
//        }
//        //for test:
//        //qDebug()<<i<<":"<<tmpcand[i]<<"; coord"<<nt.listNeuron.at(tmpcand[i]).x<<":"<<nt.listNeuron.at(tmpcand[i]).y<<":"<<nt.listNeuron.at(tmpcand[i]).z<<"; dir"<<tmpdir.x<<":"<<tmpdir.y<<":"<<tmpdir.z<<":"<<tmpNorm;

//        if(sa<angThr_stack) continue;

//        cand.append(tmpcand[i]);
//        candcoord.append(XYZ(nt.listNeuron.at(tmpcand[i])));
//        canddir.append(tmpdir);
//        canddircoord.append(XYZ(candcoord.last().x+canddir.last().x,candcoord.last().y+canddir.last().y,candcoord.last().z+canddir.last().z));
//        candcomponents.append(components.at(tmpcand[i]));
//    }
//}



//orientation should be 1/-1 for smaller/larger stack in directio
//new type: 1:root; 2:path; 5:fork point; 6:end point; 7:single child root;
void neuron_match_clique::initNeuronAndCandidate(NeuronTree& nt, const HBNeuronGraph& ng, QList<int>& neuronType, QList<int>& cand, QList<XYZ>& candcoord,
                                                 QList<XYZ>& canddir, QList<XYZ>& canddircoord, QList<int>& components,
                                                 QList<int>& candcomponents, QList<int>& pList, int orientation)
{
    float min=midplane-spanCand;
    float max=midplane+spanCand;

    cand.clear();
    candcoord.clear();
    canddir.clear();
    canddircoord.clear();
    candcomponents.clear();

    QVector<int> childNum(nt.listNeuron.size(), 0);
    QVector<int> connNum(nt.listNeuron.size(), 0);
    QVector<double> sectionLength(nt.listNeuron.size(), 0);
    //QList<int> components;
    components.clear();
    pList.clear();
    QVector<VISIOCYTELONG> componentSize;
    QVector<VISIOCYTELONG> componentLength;
    VISIOCYTELONG curid=0;
    for(VISIOCYTELONG i=0; i<nt.listNeuron.size(); i++){
        int pid = nt.hashNeuron.value(nt.listNeuron.at(i).pn, -1);
        if(pid<0){
            connNum[i]--; //root that only have 1 clide will also be a dead end
            components.append(curid); curid++;
            pList.append(-1);
        }
        else{
            childNum[pid]++;
            connNum[pid]++;
            sectionLength[i]=sqrt(NTDIS(nt.listNeuron.at(i),nt.listNeuron.at(pid)));
            components.append(-1);
            pList.append(pid);
        }
    }
    //neuron type
    initNeuronType(nt,ng,neuronType);

//    //for test
//    qDebug()<<"connected component";

    //connected component
    for(VISIOCYTELONG cid=0; cid<curid; cid++){
        QStack<int> pstack;
        int chid, size = 0;
        if(!components.contains(cid)) //should not happen, just in case
            continue;
        if(components.indexOf(cid)!=components.lastIndexOf(cid)) //should not happen
            qDebug("unexpected multiple tree root, please check the code: neuron_stitch_func.cpp");
        //recursively search for child and mark them as the same component
        pstack.push(components.indexOf(cid));
        size++;
        while(!pstack.isEmpty()){
            int pid=pstack.pop();
            chid = -1;
            chid = pList.indexOf(pid,chid+1);
            while(chid>=0){
                pstack.push(chid);
                components[chid]=cid;
                chid=pList.indexOf(pid,chid+1);
                size++;
            }
        }
        componentSize.append(size);
    }
    //component size
    for(VISIOCYTELONG cid=0; cid<curid; cid++){
        double length = 0;
        int idx = -1;
        for(VISIOCYTELONG i=0; i<componentSize[cid]; i++){
            idx = components.indexOf(cid,idx+1);
            length+=sectionLength[idx];
        }
        componentLength.append(length);
    }


//    //for test
//    qDebug()<<"find candidate";

    //find candidate
    QList<int> tmpcand; tmpcand.clear();
    for(VISIOCYTELONG i=0; i<neuronType.size(); i++){
        if((neuronType.at(i)%10)==6 && componentLength[components[i]]>=segmentThr){
            if(direction==0){//x
                if(nt.listNeuron.at(i).x>min && nt.listNeuron.at(i).x<max){
                    tmpcand.append(i);
                }
            }else if(direction==1){//y
                if(nt.listNeuron.at(i).y>min && nt.listNeuron.at(i).y<max){
                    tmpcand.append(i);
                }
            }else if(direction==2){//z
                if(nt.listNeuron.at(i).z>min && nt.listNeuron.at(i).z<max){
                    tmpcand.append(i);
                }
            }else{//all tips
                tmpcand.append(i);
            }
        }
    }

    //gap filter
    if(gapThr>1e-6){
        QList<int> gapmask, tmpcandbk;
        for(VISIOCYTELONG i=0; i<tmpcand.size(); i++){
            tmpcandbk.append(tmpcand.at(i));
            gapmask.append(0);
        }
        for(VISIOCYTELONG i=0; i<tmpcand.size(); i++){
            for(VISIOCYTELONG j=i+1; j<tmpcand.size(); j++){
                if(components.at(tmpcand.at(i)) == components.at(tmpcand.at(j)))
                        continue;
                if(NTDIS(nt.listNeuron.at(tmpcand.at(i)),nt.listNeuron.at(tmpcand.at(j)))<gapThr*gapThr){
                    gapmask[i]++;
                    gapmask[j]++;
                }
            }
        }
        tmpcand.clear();
        for(VISIOCYTELONG i=0; i<tmpcandbk.size(); i++){
            if(gapmask.at(i)<=0){
                tmpcand.append(tmpcandbk.at(i));
            }
        }
    }

//    //for test
//    qDebug()<<"finalize candidate";

    for(VISIOCYTELONG i=0; i<tmpcand.size(); i++){
        XYZ tmpdir(0,0,0);
        VISIOCYTELONG curid = tmpcand[i];
        VISIOCYTELONG nexid, preid=curid;
        if(neuronType.at(tmpcand.at(i))==6){ //normal tips or spine tips
            float lentmp = 0;
            while(lentmp<dir_range && curid>=0){
                lentmp+=sectionLength[curid];
                nexid=nextPointNeuronGraph(ng,curid,preid);
                preid=curid;
                curid=nexid;
            }
            tmpdir.x = nt.listNeuron.at(tmpcand[i]).x - nt.listNeuron.at(preid).x;
            tmpdir.y = nt.listNeuron.at(tmpcand[i]).y - nt.listNeuron.at(preid).y;
            tmpdir.z = nt.listNeuron.at(tmpcand[i]).z - nt.listNeuron.at(preid).z;
        }else if(neuronType.at(tmpcand.at(i))==56){ //spine fork
            float lentmp = 0;
            while(lentmp<dir_range && curid>=0){
                lentmp+=sectionLength[curid];
                nexid=-1;
                for(int j=0; j<ng.at(curid).size(); j++){
                    if(ng.at(curid).at(j)==preid) continue;
                    if(neuronType.at(ng.at(curid).at(j))==2 || neuronType.at(ng.at(curid).at(j))== 52){
                        if(nexid==-1){
                            nexid=ng.at(curid).at(j);
                        }else{
                            nexid=-2;
                            break;
                        }
                    }
                }
                preid=curid;
                curid=nexid;
            }
            tmpdir.x = nt.listNeuron.at(tmpcand[i]).x - nt.listNeuron.at(preid).x;
            tmpdir.y = nt.listNeuron.at(tmpcand[i]).y - nt.listNeuron.at(preid).y;
            tmpdir.z = nt.listNeuron.at(tmpcand[i]).z - nt.listNeuron.at(preid).z;
        }
        //normalize direction
        double tmpNorm = sqrt(tmpdir.x*tmpdir.x+tmpdir.y*tmpdir.y+tmpdir.z*tmpdir.z);
        if(tmpNorm<1e-16) tmpNorm=1e-16;
        tmpdir.x/=tmpNorm; tmpdir.x*=orientation;
        tmpdir.y/=tmpNorm; tmpdir.y*=orientation;
        tmpdir.z/=tmpNorm; tmpdir.z*=orientation;

        //judgement to the direction to avoid the connection that moves from plan to
        double sa = 1;
        if(direction==0){
            sa = tmpdir.x;
        }else if(direction == 1){
            sa = tmpdir.y;
        }else if(direction == 2){
            sa = tmpdir.z;
        }
        //for test:
//        qDebug()<<cand.size()<<":"<<tmpcand[i]<<"; coord"<<nt.listNeuron.at(tmpcand[i]).x<<":"<<nt.listNeuron.at(tmpcand[i]).y<<":"<<nt.listNeuron.at(tmpcand[i]).z<<"; dir"<<tmpdir.x<<":"<<tmpdir.y<<":"<<tmpdir.z<<":"<<tmpNorm;
//        qDebug()<<cand.size()<<":"<<tmpcand[i]<<"; dir"<<tmpdir.x<<":"<<tmpdir.y<<":"<<tmpdir.z<<":"<<tmpNorm;

        if(sa<angThr_stack) continue;

        cand.append(tmpcand[i]);
        candcoord.append(XYZ(nt.listNeuron.at(tmpcand[i])));
        canddir.append(tmpdir);
        canddircoord.append(XYZ(candcoord.last().x+canddir.last().x,candcoord.last().y+canddir.last().y,candcoord.last().z+canddir.last().z));
        candcomponents.append(components.at(tmpcand[i]));
    }
}

//orientation should be 1/-1 for smaller/larger stack in directio
//new type: 1:root; 2:path; 5:fork point; 6:end point; 7:single child root;
void neuron_match_clique::initNeuron(NeuronTree& nt, const HBNeuronGraph& ng, QList<int>& neuronType, QList<int>& components, QList<int>& pList)
{
    QVector<int> childNum(nt.listNeuron.size(), 0);
    QVector<int> connNum(nt.listNeuron.size(), 0);
    QVector<double> sectionLength(nt.listNeuron.size(), 0);
    //QList<int> components;
    components.clear();
    pList.clear();
    QVector<VISIOCYTELONG> componentSize;
    QVector<VISIOCYTELONG> componentLength;
    VISIOCYTELONG curid=0;
    for(VISIOCYTELONG i=0; i<nt.listNeuron.size(); i++){
        if(nt.listNeuron.at(i).pn<0){
            connNum[i]--; //root that only have 1 clide will also be a dead end
            components.append(curid); curid++;
            pList.append(-1);
        }
        else{
            int pid = nt.hashNeuron.value(nt.listNeuron.at(i).pn);
            childNum[pid]++;
            connNum[pid]++;
            sectionLength[i]=sqrt(NTDIS(nt.listNeuron.at(i),nt.listNeuron.at(pid)));
            components.append(-1);
            pList.append(pid);
        }
    }
    //neuron type
    initNeuronType(nt,ng,neuronType);

//    //for test
//    qDebug()<<"connected component";

    //connected component
    for(VISIOCYTELONG cid=0; cid<curid; cid++){
        QStack<int> pstack;
        int chid, size = 0;
        if(!components.contains(cid)) //should not happen, just in case
            continue;
        if(components.indexOf(cid)!=components.lastIndexOf(cid)) //should not happen
            qDebug("unexpected multiple tree root, please check the code: neuron_stitch_func.cpp");
        //recursively search for child and mark them as the same component
        pstack.push(components.indexOf(cid));
        size++;
        while(!pstack.isEmpty()){
            int pid=pstack.pop();
            chid = -1;
            chid = pList.indexOf(pid,chid+1);
            while(chid>=0){
                pstack.push(chid);
                components[chid]=cid;
                chid=pList.indexOf(pid,chid+1);
                size++;
            }
        }
        componentSize.append(size);
    }
    //component size
    for(VISIOCYTELONG cid=0; cid<curid; cid++){
        double length = 0;
        int idx = -1;
        for(VISIOCYTELONG i=0; i<componentSize[cid]; i++){
            idx = components.indexOf(cid,idx+1);
            length+=sectionLength[idx];
        }
        componentLength.append(length);
    }
}

//Neuron Type from SWC file as extra matching constrain
//set all the type to 1 to disable type constrain
void neuron_match_clique::resetSWCType()
{
    swcType0.clear();
    swcType1.clear();
    for(VISIOCYTELONG i=0; i<nt0->listNeuron.size(); i++){
        swcType0.append(2);
    }
    for(VISIOCYTELONG i=0; i<nt1->listNeuron.size(); i++){
        swcType1.append(2);
    }
}

void neuron_match_clique::setSWCType(NeuronTree* botNeuron, NeuronTree* topNeuron)
{
    if(botNeuron->listNeuron.size()!=nt0->listNeuron.size() ||
            topNeuron->listNeuron.size()!=nt1->listNeuron.size()){
        resetSWCType();
        qDebug()<<"ERROR: setSWCType - the number of node in the swc source does not match with target";
        return;
    }
    swcType0.clear();
    swcType1.clear();
    for(VISIOCYTELONG i=0; i<nt0->listNeuron.size(); i++){
        swcType0.append(botNeuron->listNeuron.at(i).type);
    }
    for(VISIOCYTELONG i=0; i<nt1->listNeuron.size(); i++){
        swcType1.append(topNeuron->listNeuron.at(i).type);
    }
}

void neuron_match_clique::setSWCType(QList<int>& botType, QList<int>& topType)
{
    if(botType.size()!=nt0->listNeuron.size() ||
            topType.size()!=nt1->listNeuron.size()){
        resetSWCType();
        qDebug()<<"ERROR: setSWCType - the number of node in the type source does not match with target";
        return;
    }
    swcType0.clear();
    swcType0=botType;
    swcType1.clear();
    swcType1=topType;
}

//normal type: 1:single root; 2:path; 5:fork point; 6:end point;
//spine related: 21:path; 61:end point;
//spine related fork: 51:have 0 none spine path (single root); 56:have only one none spine path (end); 52:have two none spine path (path); 55:have more than two none spine path (fork);
//all types ended with '6' can be used for matching across sections
int neuron_match_clique::initNeuronType(const NeuronTree& nt, const HBNeuronGraph& ng, QList<int>& neuronType)
{
    if(_TESTFLAG) //for test
        qDebug()<<"Init neuron type: spineLen:"<<spineLengthThr<<"; spineAng:"<<spineAngThr<<"; spineRadius:"<<spineRadiusThr;
    return getNeuronType(nt, ng, neuronType, spineLengthThr, spineAngThr, spineRadiusThr);
}

void neuron_match_clique::initNeuronComponents()
{
    //initNeuronComponents(*nt0, components0, parent0);
    //initNeuronComponents(*nt1, components1, parent1);
    initNeuron(*nt0, ng0, neuronType0, components0, parent0);
    initNeuron(*nt1, ng1, neuronType1, components1, parent1);

    //adjust component id of nt1
    int ccmax=0;
    for(int i=0; i<components0.size(); i++){
        ccmax=ccmax>components0.at(i)?ccmax:components0.at(i);
    }
    ccmax+=10;
    for(int i=0; i<components1.size(); i++){
        components1[i]+=ccmax;
    }
}

void neuron_match_clique::initNeuronComponents(NeuronTree& nt, QList<int>& components, QList<int>& pList)
{
    QVector<int> childNum(nt.listNeuron.size(), 0);
    QVector<int> connNum(nt.listNeuron.size(), 0);
    //QList<int> components;
    components.clear();
    pList.clear();
    VISIOCYTELONG curid=0;
    for(VISIOCYTELONG i=0; i<nt.listNeuron.size(); i++){
        if(nt.listNeuron.at(i).pn<0){
            connNum[i]--; //root that only have 1 clide will also be a dead end
            components.append(curid); curid++;
            pList.append(-1);
        }
        else{
            int pid = nt.hashNeuron.value(nt.listNeuron.at(i).pn);
            childNum[pid]++;
            connNum[pid]++;
            components.append(-1);
            pList.append(pid);
        }
    }
    //connected component
    for(VISIOCYTELONG cid=0; cid<curid; cid++){
        QStack<int> pstack;
        int chid, size = 0;
        if(!components.contains(cid)) //should not happen, just in case
            continue;
        if(components.indexOf(cid)!=components.lastIndexOf(cid)) //should not happen
            qDebug("unexpected multiple tree root, please check the code: neuron_stitch_func.cpp");
        //recursively search for child and mark them as the same component
        pstack.push(components.indexOf(cid));
        size++;
        while(!pstack.isEmpty()){
            int pid=pstack.pop();
            chid = -1;
            chid = pList.indexOf(pid,chid+1);
            while(chid>=0){
                pstack.push(chid);
                components[chid]=cid;
                chid=pList.indexOf(pid,chid+1);
                size++;
            }
        }
    }
}

void neuron_match_clique::examineMatchingResult(int num[9])
{
    num[4]=candID0.size();
    num[5]=candID1.size();
    num[6]=cliqueList0.size();
    num[7]=cliqueList1.size();
    num[8]=rankedCliqueMatch.size();

    //find all ground truth matches
    QSet<int> connSet0, connSet1;
    QMap<VISIOCYTELONG, int> VtxConnMap0,VtxConnMap1;
    for(VISIOCYTELONG pid=0; pid<nt0_org->listNeuron.size(); pid++){
        if(nt0_org->listNeuron.at(pid).type>0){
            connSet0.insert(nt0_org->listNeuron.at(pid).type);
            VtxConnMap0[pid]=nt0_org->listNeuron.at(pid).type;
        }
    }
    for(VISIOCYTELONG pid=0; pid<nt1_org->listNeuron.size(); pid++){
        if(nt1_org->listNeuron.at(pid).type>0){
            connSet1.insert(nt1_org->listNeuron.at(pid).type);
            VtxConnMap1[pid]=nt1_org->listNeuron.at(pid).type;
        }
    }
    connSet0.intersect(connSet1);
    num[3]=connSet0.size();

    //check all matches found
    num[0]=num[1]=num[2]=0;
    int maxcomp=0;
    for(QSet<int>::iterator iter=connSet0.begin(); iter!=connSet0.end(); iter++){
        maxcomp=MAX(maxcomp, *iter+1);
    }
    vector<int> record(maxcomp,-1);
    for(QSet<int>::iterator iter=connSet0.begin(); iter!=connSet0.end(); iter++){
        record[*iter]=0;
    }

    for(int i=0; i<pmatch0.size(); i++){
        if((!VtxConnMap0.contains(pmatch0.at(i)))||(!VtxConnMap1.contains(pmatch1.at(i)))){
            num[1]++;
            continue;
        }
        if(VtxConnMap0[pmatch0.at(i)] != VtxConnMap1[pmatch1.at(i)]){
            num[1]++;
            continue;
        }
        record[VtxConnMap0[pmatch0.at(i)]]++;
    }
    for(int i=0; i<record.size(); i++){
        if(record[i]==0){
            num[2]++;
        }
        if(record[i]>0){
            num[0]++;
        }
    }
}

void neuron_match_clique::examineMatchingResult(double num[11], NeuronTree* nt_truth)
{
    int tmpnum[9];
    examineMatchingResult(tmpnum);

    for(int i=0; i<9; i++){
        num[i]=tmpnum[i];
    }

    //calculate distance to truth
    if(nt_truth->listNeuron.size() == nt1_a->listNeuron.size()){
        num[9]=0;
        num[10]=0;
        for(int i=0; i<nt_truth->listNeuron.size(); i++){
            double tmpd=0;
            double tmps=(nt_truth->listNeuron.at(i).x-nt1_a->listNeuron.at(i).x);
            tmpd+=tmps*tmps;
            tmps=(nt_truth->listNeuron.at(i).y-nt1_a->listNeuron.at(i).y);
            tmpd+=tmps*tmps;
            num[10]+=sqrt(tmpd);
            tmps=(nt_truth->listNeuron.at(i).z-nt1_a->listNeuron.at(i).z)*zscale;
            tmpd+=tmps*tmps;
            num[9]+=sqrt(tmpd);
        }
        num[9]/=nt_truth->listNeuron.size();
        num[10]/=nt_truth->listNeuron.size();
    }else{
        num[9]=-1;
        num[10]=-1;
    }
}
