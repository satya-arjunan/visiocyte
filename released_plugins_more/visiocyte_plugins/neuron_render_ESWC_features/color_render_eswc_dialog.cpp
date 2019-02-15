#include "color_render_eswc_dialog.h"

color_render_ESWC_dialog::color_render_ESWC_dialog(VISIOCYTEPluginCallback2 * cb, VisiocyteR_MainWindow* inwin, int nid)
{
    visiocytewin=inwin;
    callback=cb;

    //load neuron tree
    nt=(NeuronTree*)&(callback->getHandleNeuronTrees_Any3DViewer(visiocytewin)->at(nid));
    feaNum=nt->listNeuron.at(0).fea_val.size();
    if(feaNum<=0.1){
        visiocyte_msg("Cannot identify feature value for the selected neuron");
        return;
    }
    //back up type
    bk_type.clear();
    for(int i=0; i<nt->listNeuron.size(); i++){
        bk_type.push_back(nt->listNeuron.at(i).type);
    }

    //create
    cb_feature = new QComboBox();
    for(int i=0; i<feaNum; i++)
        cb_feature->addItem(QString::number(i+1));
    spin_min = new QDoubleSpinBox();
    spin_max = new QDoubleSpinBox();
    spin_black = new QDoubleSpinBox();
    spin_white = new QDoubleSpinBox();
    spin_meg = new QDoubleSpinBox();
    check_black = new QCheckBox("black");
    check_white = new QCheckBox("white");
    check_meg = new QCheckBox("magenta");
    btn_update = new QPushButton("update");
    btn_quit = new QPushButton("quit");
    btn_reset = new QPushButton("reset");

    connect(check_black, SIGNAL(clicked()),this,SLOT(set_black()));
    connect(check_white, SIGNAL(clicked()),this,SLOT(set_white()));
    connect(check_meg, SIGNAL(clicked()),this,SLOT(set_magenta()));
    connect(btn_update, SIGNAL(clicked()),this,SLOT(update()));
    connect(btn_quit, SIGNAL(clicked()),this,SLOT(reject()));
    connect(btn_reset, SIGNAL(clicked()),this,SLOT(reset()));
    connect(cb_feature, SIGNAL(currentIndexChanged(int)),this,SLOT(set_span()));

    //layout
    QGridLayout * gridLayout = new QGridLayout();
    QLabel* label_feature = new QLabel("feature id:");
    gridLayout->addWidget(label_feature,1,0,1,2);
    gridLayout->addWidget(cb_feature,1,2,1,1);
    QLabel* label_min = new QLabel("lower span (blue):");
    gridLayout->addWidget(label_min,2,0,1,2);
    gridLayout->addWidget(spin_min,2,2,1,1);
    QLabel* label_max = new QLabel("upper span (red):");
    gridLayout->addWidget(label_max,3,0,1,2);
    gridLayout->addWidget(spin_max,3,2,1,1);

    QFrame *line_2 = new QFrame();
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line_2,4,0,1,3);
    QLabel* label_color = new QLabel("special color");
    QLabel* label_val = new QLabel("value");
    gridLayout->addWidget(label_color,5,0,1,2);
    gridLayout->addWidget(label_val,5,2,1,1);
    gridLayout->addWidget(check_black,6,0,1,2);
    gridLayout->addWidget(spin_black,6,2,1,1);
    gridLayout->addWidget(check_white,7,0,1,2);
    gridLayout->addWidget(spin_white,7,2,1,1);
    gridLayout->addWidget(check_meg,8,0,1,2);
    gridLayout->addWidget(spin_meg,8,2,1,1);

    QFrame *line_3 = new QFrame();
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line_3,13,0,1,3);
    gridLayout->addWidget(btn_update,14,2,1,1);
    gridLayout->addWidget(btn_reset,15,2,1,1);
    gridLayout->addWidget(btn_quit,16,2,1,1);

    setLayout(gridLayout);

    set_black();
    set_white();
    set_magenta();
    set_span();
}

void color_render_ESWC_dialog::enterEvent(QEvent *e)
{
    checkwindow();

    QDialog::enterEvent(e);
}

void color_render_ESWC_dialog::set_span()
{
    checkwindow();

    int feaId=cb_feature->currentIndex();
    float lower=nt->listNeuron.at(0).fea_val.at(feaId);
    float upper=nt->listNeuron.at(0).fea_val.at(feaId);
    for(VISIOCYTELONG nid=0; nid<nt->listNeuron.size(); nid++){
        lower=MIN(lower, nt->listNeuron.at(nid).fea_val.at(feaId));
        upper=MAX(upper, nt->listNeuron.at(nid).fea_val.at(feaId));
    }
    spin_max->setMinimum(lower);
    spin_max->setMaximum(upper);
    spin_min->setMinimum(lower);
    spin_min->setMaximum(upper);
    spin_max->setValue(upper);
    spin_min->setValue(lower);

    spin_black->setMinimum(lower);
    spin_black->setMaximum(upper);
    spin_white->setMinimum(lower);
    spin_white->setMaximum(upper);
    spin_meg->setMinimum(lower);
    spin_meg->setMaximum(upper);
}

void color_render_ESWC_dialog::update()
{
    checkwindow();

    float lower=spin_min->value();
    float upper=spin_max->value();
    float scale=upper-lower;
    int feaId=cb_feature->currentIndex();
    for(VISIOCYTELONG nid=0; nid<nt->listNeuron.size(); nid++){
        float tmp=0;
        if(nt->listNeuron.at(nid).fea_val.size()>feaId){
            if(check_black->isChecked() && nt->listNeuron.at(nid).fea_val.at(feaId)==spin_black->value()){
                tmp=1;
            }else if(check_white->isChecked() && nt->listNeuron.at(nid).fea_val.at(feaId)==spin_white->value()){
                tmp=0;
            }else if(check_meg->isChecked() && nt->listNeuron.at(nid).fea_val.at(feaId)==spin_meg->value()){
                tmp=4;
            }else{
                tmp=(nt->listNeuron.at(nid).fea_val.at(feaId)-lower)/scale;
                if(tmp<0) tmp=0;
                if(tmp>1) tmp=1;
                tmp=tmp*235+20;
            }
        }
        nt->listNeuron[nid].type=tmp;
    }

    if(visiocytewin){
        callback->update_3DViewer(visiocytewin);
    }
}

void color_render_ESWC_dialog::reset()
{
    checkwindow();

    for(VISIOCYTELONG i=0; i<bk_type.size(); i++){
        nt->listNeuron[i].type=bk_type.at(i);
    }

    if(visiocytewin){
        callback->update_3DViewer(visiocytewin);
    }
}

void color_render_ESWC_dialog::reject()
{
    reset();
    if(visiocytewin){
        callback->update_3DViewer(visiocytewin);
    }

    QDialog::reject();
}

void color_render_ESWC_dialog::set_black()
{
    spin_black->setEnabled(check_black->isChecked());
}

void color_render_ESWC_dialog::set_white()
{
    spin_white->setEnabled(check_white->isChecked());
}

void color_render_ESWC_dialog::set_magenta()
{
    spin_meg->setEnabled(check_meg->isChecked());
}

void color_render_ESWC_dialog::checkwindow()
{
    //check if current window is closed
    if (!callback){
        this->hide();
        return;
    }

    bool isclosed = true;
    //search to see if the window is still open
    QList <VisiocyteR_MainWindow *> allWindowList = callback->getListAll3DViewers();
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++)
    {
        if(allWindowList.at(i)==visiocytewin){
            isclosed = false;
            break;
        }
    }

    //close the window
    if(isclosed){
        this->hide();
        return;
    }
}
