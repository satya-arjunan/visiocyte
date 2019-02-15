#include "manual_correct_dialog.h"
#include "common.h"

#define main_win_name "spine_detector_result"
#define fname_image "truncated_view"
#define fname_image_seg "segmented view"

manual_correct_dialog::manual_correct_dialog(VISIOCYTEPluginCallback2 *cb, int proofread_code)
{
    callback=cb;
    view_code=proofread_code;
    image1Dc_in=0;
    image1Dc_spine=0;
    image_seg=0;
    image_trun=0;
    label=0;
    create();
    initDlg();
    check_button();
    //create_big_image();
    edit_flag=false;
    seg_edit_flag=false;
}

manual_correct_dialog::manual_correct_dialog(VISIOCYTEPluginCallback2 *cb)
{
    callback=cb;
    image1Dc_in=0;
    image1Dc_spine=0;
    image_seg=0;
    image_trun=0;
    label=0;
    create_big_image();
    initDlg();
    check_button();
    edit_flag=false;
    seg_edit_flag=false;
}

void manual_correct_dialog::initDlg()
{
    QSettings settings("VISIOCYTE plugin","spine_detector");
    if (settings.contains("fname_swc"))
        swc_load_path=settings.value("fname_swc").toString();
    if (settings.contains("fname_img"))
        image_load_path=settings.value("fname_img").toString();
    if (settings.contains("output_folder"))
        csv_load_path=settings.value("output_folder").toString();
}

void manual_correct_dialog::create()
{
    QGridLayout *mygridLayout = new QGridLayout;
    QLabel* label_load = new QLabel(QObject::tr("Load Image:"));
    mygridLayout->addWidget(label_load,0,0,1,1);
    edit_load = new QLineEdit;
    edit_load->setText(""); edit_load->setReadOnly(true);
    mygridLayout->addWidget(edit_load,0,1,1,6);
    btn_load = new QPushButton("...");
    mygridLayout->addWidget(btn_load,0,7,1,1);

    QLabel* label_swc = new QLabel(QObject::tr("Load swc:"));
    mygridLayout->addWidget(label_swc,1,0,1,1);
    edit_swc = new QLineEdit;
    edit_swc->setText(""); edit_swc->setReadOnly(true);
    mygridLayout->addWidget(edit_swc,1,1,1,6);
    btn_swc = new QPushButton("...");
    mygridLayout->addWidget(btn_swc,1,7,1,1);

    QLabel* label_csv = new QLabel(QObject::tr("Output directory:"));
    mygridLayout->addWidget(label_csv,2,0,1,1);
    edit_csv = new QLineEdit;
    edit_csv->setText(""); edit_csv->setReadOnly(true);
    mygridLayout->addWidget(edit_csv,2,1,1,6);
    btn_csv = new QPushButton("...");
    mygridLayout->addWidget(btn_csv,2,7,1,1);

    QLabel *channel = new QLabel(tr("Which channel to use?"));
    channel_menu = new QComboBox;
    channel_menu->addItem("red");
    channel_menu->addItem("green");
    channel_menu->addItem("blue");
    mygridLayout->addWidget(channel,3,0,1,2);
    mygridLayout->addWidget(channel_menu,3,3,1,5);

    //para setting
    QFrame *line_1 = new QFrame();
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    mygridLayout->addWidget(line_1,4,0,1,8);

    QLabel *bg_thr = new QLabel(tr("Background threshold:"));
    mygridLayout->addWidget(bg_thr,5,0,1,6);
    QLabel *max_pixel=new QLabel (tr("Max spine volume:"));
    mygridLayout->addWidget(max_pixel,6,0,1,6);
    QLabel *min_pixel=new QLabel (tr("Min spine volume:"));
    mygridLayout->addWidget(min_pixel,7,0,1,6);
    QLabel *max_dis=new QLabel(tr("Max spine distance to surface:"));
    mygridLayout->addWidget(max_dis,8,0,1,6);
    QLabel *width_thr=new QLabel(tr("Max spine width:"));
    mygridLayout->addWidget(width_thr,9,0,1,6);

    spin_max_dis=new QSpinBox;
    spin_max_dis->setRange(5,80);
    spin_max_dis->setValue(40);
    spin_min_pixel=new QSpinBox;
    spin_min_pixel->setRange(10,100);
    spin_min_pixel->setValue(30);
    spin_bg_thr=new QSpinBox;
    spin_bg_thr->setRange(1,255);
    spin_bg_thr->setValue(90);
    spin_max_pixel=new QSpinBox;
    spin_max_pixel->setRange(2000,8000);
    spin_max_pixel->setValue(7000);
    spin_width_thr=new QSpinBox;
    spin_width_thr->setRange(10,100);
    spin_width_thr->setValue(35);
    mygridLayout->addWidget(spin_bg_thr,5,6,1,2);
    mygridLayout->addWidget(spin_max_pixel,6,6,1,2);
    mygridLayout->addWidget(spin_min_pixel,7,6,1,2);
    mygridLayout->addWidget(spin_max_dis,8,6,1,2);
    mygridLayout->addWidget(spin_width_thr,9,6,1,2);

    QFrame *line_2 = new QFrame();
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    mygridLayout->addWidget(line_2,15,0,1,8);
    btn_run    = new QPushButton("Run");
    QPushButton *cancel = new QPushButton("Cancel");
    mygridLayout->addWidget(btn_run,16,2,1,2);
    mygridLayout->addWidget(cancel,16,5,1,2);

    //operation zone
    QFrame *line_3 = new QFrame();
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    mygridLayout->addWidget(line_3,17,0,1,8);
    QString info=">> SpineDetector first performs automatic spines detections near dendrites,then allows"
            " users to do manual proofreading.<br>";
    info+=">> <b>Background threshold: </b> All voxels above the background threshold are foreground, others are background voxles (voxels not considered spines).<br>";
    info+=">> <b>Max spine volume:</b> Maximum number of voxels allowed for a spine. Groups with larger volumes are considered not spines.<br>";
    info+=">> <b>Min spine volume:</b> Minumum number of voxels allowed for a spine.<br>";
    info+=">> <b>Max spine distance:</b> Spine distance is defined as the distance from the surface of dendrite. Automatic spine search will only occur in the region"
              " defined by the max spine distance.<br>";
    info+=">> <b>Max spine width:</b> Maximum width allowed for each spine.<br>";
    info+=">> For further questions, please contact Yujie Li at yujie.jade@gmail.com)";

    QTextEdit *text_info = new QTextEdit;
    text_info->setText(info);
    text_info->setReadOnly(true);
    mygridLayout->addWidget(text_info,18,0,1,9);

    this->setLayout(mygridLayout);
    this->setWindowTitle("SpineDetector");
    this->show();
    if (view_code==1)
        connect(btn_run,SIGNAL(clicked()), this, SLOT(run_view_by_seg()));
    else if(view_code==0)
        connect(btn_run,SIGNAL(clicked()), this, SLOT(run_view_by_spine()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_load, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(btn_swc,SIGNAL(clicked()),this,SLOT(load_swc()));
    connect(btn_csv,SIGNAL(clicked()),this,SLOT(csv_out()));
    //connect(this,SIGNAL(finished(int)),this,SLOT(dialoguefinish(int)));
}

bool manual_correct_dialog::check_button()
{
    if (this->edit_load->text().isEmpty() || this->edit_swc->text().isEmpty()
            ||this->edit_csv->text().isEmpty())
    {
        btn_run->setEnabled(false);
        return false;
    }else{
        btn_run->setEnabled(true);
        return true;
    }
}

void manual_correct_dialog::run_view_by_spine()
{
    qDebug()<<"in check data";

    if(!check_button())
    {
        visiocyte_msg("You have not provided valid input/output");
        return;
    }

    this->accept();
    get_para();
    if(!auto_spine_detect())
        return;
    if(before_proof_dialog())
        create_standing_dialog();
    else
        return;

}

void manual_correct_dialog::run_view_by_seg()
{
    qDebug()<<"in check data2";
    if(!check_button())
    {
        visiocyte_msg("You have not provided valid input/output");
        return;
    }
    this->accept();
    get_para();
    if(!auto_spine_detect())
        return;
    if(before_proof_dialog())
    {
        check_neuron_tree();  //get ready checked_neurontree,weed out nodes outside image
        segment_neuronswc=neurontree_divide_swc(&checked_neuron,all_para.max_dis*6);
        standing_segment_dialog();
    }
    else
        return;
}

void manual_correct_dialog::create_standing_dialog()
{
    mydialog=new QDialog;
    mydialog->setWindowTitle("spine proofreading");
    markers=new QComboBox;
    for (int i=0;i<LList_in.size();i++)
        markers->addItem(QString("marker ")+QString::number(i+1));
    //markers->setFixedWidth(250);
    markers->setCurrentIndex(0);
    QGridLayout *layout2=new QGridLayout;
    edit_status = new QPlainTextEdit;
    edit_status->setReadOnly(true);
    edit_status->setFixedHeight(50);
    layout2->addWidget(markers,0,0,1,4);
    layout2->addWidget(edit_status,1,0,1,6);

    QPushButton *accept=new QPushButton(tr("Accept"));
    QPushButton *reject=new QPushButton(tr("Delete"));
    QPushButton *skip = new QPushButton(tr("Skip"));

    layout2->addWidget(accept,3,0,1,2);
    layout2->addWidget(reject,3,2,1,2);
    layout2->addWidget(skip,3,4,1,2);

    QPushButton *dilate = new QPushButton(tr("Dilate"));
    QPushButton *erode =new QPushButton (tr("Erode"));
    QPushButton *reset = new QPushButton (tr("Reset spine"));
    layout2->addWidget(dilate,4,0,1,2);
    layout2->addWidget(erode,4,2,1,2);
    layout2->addWidget(reset,4,4,1,2);

    QFrame *line_1 = new QFrame();
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    layout2->addWidget(line_1,5,0,1,6);

    QPushButton *button_save=new QPushButton;
    button_save->setText("Finish");
    layout2->addWidget(button_save,6,0,1,2);
//    small_remover=new QCheckBox;
//    small_remover->setText(QObject::tr("Remove groups < min spine pixel"));
//    small_remover->setChecked(false);
//    layout2->addWidget(small_remover,6,2,1,4);
//    QPushButton *button_p_cancel=new QPushButton;
//    button_p_cancel->setText("Quit");
//    layout2->addWidget(button_p_cancel,6,2,1,1);

    mydialog->setLayout(layout2);

    connect(button_save,SIGNAL(clicked()),this,SLOT(finish_proof_dialog()));
    connect(markers,SIGNAL(currentIndexChanged(int)),this,SLOT(marker_roi()));
    connect(accept,SIGNAL(clicked()),this,SLOT(accept_marker()));
    connect(reject,SIGNAL(clicked()),this,SLOT(delete_marker()));
    connect(skip,SIGNAL(clicked()),this,SLOT(skip_marker()));
    connect(dilate,SIGNAL(clicked()),this,SLOT(dilate()));
    connect(erode,SIGNAL(clicked()),this,SLOT(erode()));
    connect(reset,SIGNAL(clicked()),this,SLOT(reset_edit()));

    marker_roi();
    mydialog->show();
}


bool manual_correct_dialog::csv_out()
{
    QString fileSaveDir;
    fileSaveDir = QFileDialog::getExistingDirectory(0, QObject::tr("Select Directory to Save Results"),
            csv_load_path,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(fileSaveDir.isEmpty())
        return false;

#if defined(Q_OS_MAC)
        //detect if there is a Qt redundant folder bug, if yes, then make a correction
        // This fix is done by PHC, 2015May14. This should work in most cases. However
        //if a user choose a strange tmp folder with the name "/abc/abc/abc" then this fix
        //will wrongly go to the parent folder "abc/abc".
        QDir tmp1(fileSaveDir);
        QString tmp2 = tmp1.dirName();
        if (fileSaveDir.endsWith(tmp2+'/'+tmp2))
        {
            fileSaveDir = fileSaveDir.left(fileSaveDir.size() - tmp2.size() - 1);
        }
#endif

    folder_output = fileSaveDir;

    //fname_output = QDir(folder_output).filePath("spine_profile.csv");
    edit_csv->setText(folder_output);
    QSettings settings("VISIOCYTE plugin","spine_detector");
    settings.setValue("output_folder",edit_csv->text());
    check_button();
    //QDir::setCurrent(fileSaveDir);
    return true;
}

bool manual_correct_dialog::load_swc()
{
    eswc_flag=false;
    QString filename;
    filename = QFileDialog::getOpenFileName(0, 0,swc_load_path,"Supported file (*.swc *.eswc)" ";;Neuron structure(*.swc *eswc)",0,0);

    if(filename.isEmpty())
    {
        visiocyte_msg("No swc file loaded.");
        return false;
    }
    NeuronSWC *p_cur=0;
    neuron = readSWC_file(filename);

    for (VISIOCYTELONG ii=0; ii<neuron.listNeuron.size(); ii++)
    {
        p_cur = (NeuronSWC *)(&(neuron.listNeuron.at(ii)));
        if (p_cur->r<0)
        {
            visiocyte_msg("You have illeagal radius values. Check your data.");
            return false;
        }
     }

    if (filename.contains(".eswc"))
    {
        VISIOCYTELONG sum_level=0;
        bool possible_eswc=true;
        for (VISIOCYTELONG ii=0; ii<neuron.listNeuron.size(); ii++)
        {
            p_cur = (NeuronSWC *)(&(neuron.listNeuron.at(ii)));
//            qDebug()<<"I:"<<ii<<"seg_id:"<<neuron.listNeuron[ii].seg_id;//<<":"<<neuron.listNeuron[ii].fea_val.size();
//            qDebug()<<" level:"<<neuron.listNeuron[ii].level;
//            qDebug()<<"fea1:"<<neuron.listNeuron[ii].fea_val[0]<<"size:"<<neuron.listNeuron[ii].fea_val.size();
            sum_level+=p_cur->level;
            if (p_cur->fea_val.size()<2)
            {
                visiocyte_msg("No additional node info is provided. The csv output will onlly"
                " produce the basic spine info.");
                possible_eswc=false;
                break;
            }
        }
        if (possible_eswc && sum_level!=0)  //this is a eswc file
            eswc_flag=true;
    }

    edit_swc->setText(filename);
    check_button();
    QSettings settings("VISIOCYTE plugin","spine_detector");
    settings.setValue("fname_swc",edit_swc->text());\
    qDebug()<<"finished reading"<<neuron.listNeuron.size();
    return true;
}

void manual_correct_dialog::load_marker()
{
//    QString filename;
//    filename = QFileDialog::getOpenFileName(0, 0,"","Supported file (*.marker)" ";;Marker file(*.marker)",0,0);

//    if(filename.isEmpty())
//    {
//        visiocyte_msg("You don't have any marker file open in the main window.");
//        return;
//    }
//    QList<ImageMarker> tmp_list;
//    tmp_list=readMarker_file(filename.toAscii());
//    for (int i=0;i<tmp_list.size();i++)
//    {
//        LocationSimple tmp;
//        tmp.x=tmp_list.at(i).x;
//        tmp.y=tmp_list.at(i).y;
//        tmp.z=tmp_list.at(i).z;
//        tmp.color.r=tmp.color.g=tmp.color.b=255;
//        LList_in.append(tmp);
//    }
//    edit_marker->setText(filename);
}

bool manual_correct_dialog::loadImage()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(0, QObject::tr("Choose the input image "),
             image_load_path,QObject::tr("Images (*.raw *.tif *.lsm *.visiocytepbd *.visiocyteraw);;All(*)"));

    if (!fileName.isEmpty())
    {
        if (!simple_loadimage_wrapper(*callback, fileName.toStdString().c_str(), image1Dc_in, sz_img, intype))
        {
            QMessageBox::information(0,"","load image "+fileName+" error!");
            return false;
        }
        if (sz_img[3]>3)
        {
            sz_img[3]=3;
            QMessageBox::information(0,"","More than 3 channels were loaded."
                                     "The first 3 channel will be applied for analysis.");
        }

        VISIOCYTELONG size_tmp=sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3];
        if(intype==1)
        {
        }
        else if (intype == 2) //VISIOCYTE_UINT16;
        {
            convert2UINT8((unsigned short*)image1Dc_in, image1Dc_in, size_tmp);
        }
        else if(intype == 4) //VISIOCYTE_FLOAT32;
        {
            convert2UINT8((float*)image1Dc_in, image1Dc_in, size_tmp);
        }
        else
        {
            QMessageBox::information(0,"","Currently this program only supports UINT8, UINT16, and FLOAT32 data type.");
            return false;
        }
        edit_load->setText(fileName);
        QSettings settings("VISIOCYTE plugin","spine_detector");
        settings.setValue("fname_img",edit_load->text());
        qDebug()<<"Image set"<<sz_img[0]<<":"<<sz_img[1]<<sz_img[2]<<sz_img[3];
        check_button();
        return true;
    }
}

void manual_correct_dialog::get_para()
{
    //obtain all para
    all_para.bgthr=spin_bg_thr->value();
    all_para.max_dis=spin_max_dis->value();
    all_para.intensity_max_pixel=spin_max_pixel->value();
    all_para.min_pixel=spin_min_pixel->value();
    all_para.width_thr=spin_width_thr->value();
    sel_channel=channel_menu->currentIndex();
    all_para.dst_max_pixel=2000;
}

bool manual_correct_dialog::before_proof_dialog()
{
    QMessageBox mybox;
    int size=label_group.size();
    QString info="The automatic spine detector finds "+ QString::number(size)+" spines";
    mybox.setText(info);
    QPushButton *start=mybox.addButton(tr("Start proofreading"),QMessageBox::ActionRole);
    QPushButton *cancel_button=mybox.addButton(QMessageBox::Cancel);

    mybox.setDefaultButton(start);
    mybox.exec();
    //open_main_triview();
     if (mybox.clickedButton() == start) {
         return true;
     } else if (mybox.clickedButton() == cancel_button) {
         QString info="The spine csv profile is saved at "+ edit_csv->text();
         QMessageBox::information(0,"spine_detector",info,QMessageBox::Ok);
         write_spine_profile("auto_spine_profile.csv");
         vector<int> dummy;
         dummy.resize(label_group.size(),1);
         write_svm_file("predict.txt",dummy);
         open_main_triview();
         callback->setImageName(main_win,"automatic_spine_detector_result");
         callback->setLandmark(main_win,LList_in);
         callback->updateImageWindow(main_win);
         callback->open3DWindow(main_win);
         callback->pushObjectIn3DWindow(main_win);

         //prepare label
         VISIOCYTELONG size_page=sz_img[0]*sz_img[1]*sz_img[2];
         unsigned short *label = new unsigned short [size_page*3];
         memset(label,0,size_page*3);
         for(int i=0; i<label_group.size(); i++)
         {
             GetColorRGB(rgb,i);
             GOV tmp = label_group[i];
             for (int j=0; j<tmp.size(); j++)
             {
                 label[tmp.at(j)->pos] = rgb[0];
                 label[tmp.at(j)->pos+size_page]=rgb[1];
                 label[tmp.at(j)->pos+2*size_page]=rgb[2];
             }
         }

         Image4DSimple image_label;
         image_label.setData((unsigned char*)label,sz_img[0],sz_img[1],sz_img[2],sz_img[3],VISIOCYTE_UINT16);
         QString name="auto_spine_detector_label";
         visiocytehandle new_win2=callback->newImageWindow(name);
         callback->setImage(new_win2,&image_label);
         callback->setLandmark(new_win2,LList_in);
         callback->updateImageWindow(new_win2);
         callback->open3DWindow(new_win2);
         callback->pushObjectIn3DWindow(new_win2);
         return false;
     }
}

bool manual_correct_dialog::finish_proof_dialog()
{
    QMessageBox mybox;
    mybox.setText("Have you finished proofreading?");

    QPushButton *save_button = mybox.addButton(tr("Finish and save"),QMessageBox::ActionRole);
    QPushButton *cancel_button=mybox.addButton(QMessageBox::Cancel);
    QPushButton *discard_button=mybox.addButton(QMessageBox::Discard);

    mybox.setDefaultButton(save_button);
    mybox.exec();

     if (mybox.clickedButton() == save_button) {
         int final_landmarks_num;
         final_landmarks_num = save(fname_image);
         QMessageBox mynewbox;
         QString info="After proofreading "+ QString::number(final_landmarks_num)+" spines were found\n";
         info+="The spine csv profile is saved at "+ edit_csv->text();
         mynewbox.information(0,"spine detector",info,QMessageBox::Ok);
         return true;

     } else if (mybox.clickedButton() == cancel_button) {
         return false;
     }
     else if (mybox.clickedButton()== discard_button) {
         //need to close all image windows //check 3D window
         visiocytehandleList list_triwin = callback->getImageWindowList();
         for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
             if(callback->getImageName(list_triwin.at(i)).contains(fname_image))
             {
                 callback->close3DWindow(list_triwin[i]);
             }
         }
         mydialog->close();
         return false;
     }
}


bool manual_correct_dialog::auto_spine_detect()
{
    int progress_id=0;
    QProgressDialog progress("Auto spine detection starts....","Abort",
                                                 0,10,this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);

    //qDebug()<<"~~~~Auto spine detection starts....";
    spine_fun spine_obj(callback,all_para,sel_channel);
    if (!spine_obj.pushImageData(image1Dc_in,sz_img))
        return false;
//    if (eswc_flag)
//        spine_obj.push_eswc_Data(neuron);
//    else
        spine_obj.pushSWCData(neuron);

    progress_id=progress_id+6;
    progress.setValue(progress_id); //6

    if(!spine_obj.init()){
        visiocyte_msg("No spine candidates were found. Please check image and swc file");
        return false;
    }
    progress.setValue(++progress_id); //7
    if(!spine_obj.reverse_dst_grow())
    {
        visiocyte_msg("No spines candidates were found; Please check image and swc file");
        return false;
    }
    progress.setValue(++progress_id);//8
    spine_obj.run_intensityGroup();
    progress.setValue(++progress_id);//9
    spine_obj.conn_comp_nb6();
    progress.setValue(++progress_id);//10
    LList_in = spine_obj.get_center_landmarks();

    label_group = spine_obj.get_group_label();
    //spine_obj.saveResult();

    //make copy for label_group
    label_group_copy=label_group;

    VISIOCYTELONG size_page=sz_img[0]*sz_img[1]*sz_img[2];
    image1Dc_spine = new unsigned char [size_page*3];
    memset(image1Dc_spine,0,size_page*3);
    memcpy(image1Dc_spine,image1Dc_in,size_page);

    for(int i=0; i<label_group.size(); i++)
    {
        GOV tmp = label_group[i];
        for (int j=0; j<tmp.size(); j++)
        {
            image1Dc_spine[tmp.at(j)->pos+size_page]=255;
        }
    }
    if (image1Dc_in!=0)
    {
        delete[] image1Dc_in; image1Dc_in=0;
    }
    progress.setValue(++progress_id);
    progress.close();
    qDebug()<<"auto spine_detect complete"<<"LList size:"<<LList_in.size();
    return true;
}

void manual_correct_dialog::reset_image_data()
{
    VISIOCYTELONG y_offset=sz_img[0];
    VISIOCYTELONG z_offset=sz_img[0]*sz_img[1];
    int mid=markers->currentIndex();//start from zero--marker 1

    int halfwindowsize=30;
    //create bounding box

    VISIOCYTELONG x,y,z;
    x=LList_in[mid].x-1;
    y=LList_in[mid].y-1;
    z=LList_in[mid].z-1;

    if (x<halfwindowsize) sz[0]=x+halfwindowsize+1;
    else if (x+halfwindowsize>sz_img[0]) sz[0]=sz_img[0]-x+halfwindowsize;
    else sz[0]=2*halfwindowsize+1;

    if (y<halfwindowsize) sz[1]=y+halfwindowsize+1;
    else if (y+halfwindowsize>sz_img[1]) sz[1]=sz_img[1]-y+halfwindowsize;
    else sz[1]=2*halfwindowsize+1;

    if (z<halfwindowsize) sz[2]=z+halfwindowsize+1;
    else if (z+halfwindowsize>sz_img[2]) sz[2]=sz_img[2]-z+halfwindowsize;
    else sz[2]=2*halfwindowsize+1;

    sz[3]=3;

    if (image_trun!=0)
    {
        delete [] image_trun;
        image_trun=0;
    }
    image_trun=new unsigned char[sz[0]*sz[1]*sz[2]*sz[3]];
    memset(image_trun,0,sz[0]*sz[1]*sz[2]*sz[3]);
    qDebug()<<"image_trun allocated";
    //Store the area around the marker in image_trun
    x_start=MAX(0,x-halfwindowsize);
    y_start=MAX(0,y-halfwindowsize);
    z_start=MAX(0,z-halfwindowsize);
    x_end=MIN(sz_img[0],x+halfwindowsize+1);
    y_end=MIN(sz_img[1],y+halfwindowsize+1);
    z_end=MIN(sz_img[2],z+halfwindowsize+1);
    qDebug()<<"halfsize:"<<halfwindowsize<<" x,y,z:"<<x<<":"<<y<<":"<<z;
    qDebug()<<"sz0,sz1,sz2:"<<sz[0]<<":"<<sz[1]<<":"<<sz[2];

    for (VISIOCYTELONG dx=x_start;dx<x_end;dx++){
        for (VISIOCYTELONG dy=y_start;dy<y_end;dy++){
            for (VISIOCYTELONG dz=z_start;dz<z_end;dz++){
                VISIOCYTELONG pos=xyz2pos(dx,dy,dz,y_offset,z_offset);
                VISIOCYTELONG pos1=xyz2pos(dx-x_start,dy-y_start,dz-z_start,sz[0],sz[0]*sz[1]);
                image_trun[pos1]=image1Dc_spine[pos];
            }
        }
    }

    GOV tmp_group=label_group[mid];

    for (int i=0;i<tmp_group.size();i++)
    {
        VISIOCYTELONG new_x=tmp_group[i]->x;
        VISIOCYTELONG new_y=tmp_group[i]->y;
        VISIOCYTELONG new_z=tmp_group[i]->z;
        VISIOCYTELONG trun_pos=xyz2pos(new_x-x_start,new_y-y_start,new_z-z_start,sz[0],sz[0]*sz[1]);
        image_trun[trun_pos+sz[0]*sz[1]*sz[2]]=255;
    }

}

void manual_correct_dialog::reset_label_group()
{
    int mid=markers->currentIndex();
    GOV tmp_group=label_group_copy[mid];
    label_group[mid].clear();
    label_group[mid]=tmp_group;
    //need to update the intensity label???
}

void manual_correct_dialog::adjust_LList_to_imagetrun()
{
    LList_adj.clear();
    for (int i=0;i<LList_in.size();i++)
    {
        LocationSimple tmp;
        tmp.x=LList_in[i].x-x_start;
        tmp.y=LList_in[i].y-y_start;
        tmp.z=LList_in[i].z-z_start;
        tmp.color.r=LList_in[i].color.r;
        tmp.color.g=LList_in[i].color.g;
        tmp.color.b=LList_in[i].color.b;
        LList_adj.append(tmp);
    }
}

void manual_correct_dialog::marker_roi()
{
    qDebug()<<"in marker roi";
    if(markers->count()==0) return;
    edit_status->clear();
    VISIOCYTELONG size_tmp=sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3];
    VISIOCYTELONG size_page=sz_img[0]*sz_img[1]*sz_img[2];
    int mid=markers->currentIndex();

    //check whether previous editing needs saving
    if (edit_flag) save_edit();
    edit_flag=false;

    //step 1: check whether main-triview is open
    open_main_triview();

    //step 2:set data ready and open local tri-view/3d window
    reset_image_data();
    adjust_LList_to_imagetrun();

    //step 3: check whether local triview is open
    check_window();

    //Step 4: Focus on this marker on tri-view
    TriviewControl * p_control = callback->getTriviewControl(curwin);
    p_control->setFocusLocation((long)LList_adj.at(mid).x,
                                (long)LList_adj.at(mid).y,(long)LList_adj.at(mid).z);

    //Step 5: update marker in 2 tri-view and one 3D
    // if this markers is not determined,Landmark color change
    if (QString::fromStdString(LList_in[mid].comments).contains("0"))
    {
        //LList_in[mid].color.r=LList_in[mid].color.b=255;
        LList_adj[mid].color.r=LList_adj[mid].color.b=255;
        //LList_in[mid].color.g=70;
        LList_adj[mid].color.g=70;
    }
    Image4DSimple image4d;
    unsigned char * image_input=new unsigned char [sz[0]*sz[1]*sz[2]*sz[3]];
    memcpy(image_input,image_trun,sz[0]*sz[1]*sz[2]*sz[3]);
    image4d.setData(image_input,sz[0],sz[1],sz[2],sz[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d);
    callback->setLandmark(curwin,LList_adj);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    callback->setLandmark(main_win,LList_in);

    //Step 6: reset marker color back to original color
    //LList_in[mid].color.r=LList_in[mid].color.b=LList_in[mid].color.g=255;
    LList_adj[mid].color.r=LList_adj[mid].color.b=LList_in[mid].color.g=0;
    //qDebug()<<"~~~~marker roi finished";
}

void manual_correct_dialog::skip_marker()
{
    open_main_triview();
    check_window();
    if (edit_flag)
    {
        int ret;
        ret=save_edit();
        if (ret!=3)  //not discard
            return;
    }
    int i=markers->currentIndex();
    if(i+1<markers->count())
        markers->setCurrentIndex(i+1);
}

void manual_correct_dialog::accept_marker()
{
    qDebug()<<"in accept marker";
    open_main_triview();
    check_window();

    //update landmarks color and status
    int mid=markers->currentIndex();
    LList_in[mid].color.r=LList_in[mid].color.b=0;
    LList_in[mid].color.g=255;
    LList_adj[mid].color.r=LList_in[mid].color.b=0;
    LList_adj[mid].color.g=255;

    QString tmp=QString::number(1);
    LList_in[mid].comments=tmp.toStdString(); //accepted marker
    callback->setLandmark(curwin,LList_in);

    //update Qcombobox
    markers->setItemText(mid, "Marker: " + QString::number(mid+1) + " accepted");
    //edit_status->setPlainText("Marker: " + QString::number(mid+1) + " accepted");

    edit_flag=false;
    //go to next marker
    if(mid+1<markers->count()){
        markers->setCurrentIndex(mid+1);
    }
}

void manual_correct_dialog::delete_marker()
{
    open_main_triview();
    check_window();
    qDebug()<<"delete marker";
    //update landmarks color and status
    int mid=markers->currentIndex();

    LList_in[mid].color.g=LList_in[mid].color.b=0;
    LList_in[mid].color.r=255;
    LList_adj[mid].color.g=LList_in[mid].color.b=0;
    LList_adj[mid].color.r=255;

    LList_in[mid].comments=QString::number(2).toStdString(); //rejected marker

    //update combobox
    markers->setItemText(mid, "Marker: " + QString::number(mid+1) + " rejected");
    //edit_status->setPlainText("Marker: " + QString::number(mid+1) + " rejected");
    edit_flag=false;

    if(mid+1<markers->count()){
        markers->setCurrentIndex(mid+1);
    }
}

void manual_correct_dialog::open_main_triview()
{
    qDebug()<<"open main_triview window";
    bool window_open_flag=false;
    visiocytehandleList list_triwin = callback->getImageWindowList();
    for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
        if(callback->getImageName(list_triwin.at(i)).contains(main_win_name))
        {
            window_open_flag=true;
            main_win=list_triwin[i];
            return;
        }
    }
    if(!window_open_flag)
    {
        main_win = callback->newImageWindow(main_win_name);
        unsigned char *image_input=new unsigned char [sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]];
        memcpy(image_input,image1Dc_spine,sz_img[0]*sz_img[1]*sz_img[2]*sz_img[3]);
        Image4DSimple image_main;
        image_main.setData(image_input,sz_img[0],sz_img[1],sz_img[2],sz_img[3],VISIOCYTE_UINT8);
        callback->setImage(main_win,&image_main);
        callback->updateImageWindow(main_win);
    }
}

void manual_correct_dialog::check_window()
{
    bool window_open_flag=false;
    visiocytehandleList list_triwin = callback->getImageWindowList();
    for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
        if(callback->getImageName(list_triwin.at(i)).contains(fname_image))
        {
            window_open_flag=true;
            curwin=list_triwin[i];
            break;
        }
    }
    //qDebug()<<"check window: window_open_flag:"<<window_open_flag;
    if(!window_open_flag)
    {
        curwin=callback->newImageWindow(fname_image);
        unsigned char *image_input=new unsigned char [sz[0]*sz[1]*sz[2]*sz[3]];
        memcpy(image_input,image_trun,sz[0]*sz[1]*sz[2]*sz[3]);
        Image4DSimple image_tmp;
        image_tmp.setData(image_input,sz[0],sz[1],sz[2],sz[3],VISIOCYTE_UINT8);
        callback->setImage(curwin,&image_tmp);
        callback->updateImageWindow(curwin);
    }
}

void manual_correct_dialog::check_window_seg()
{
    bool window_open_flag=false;
    visiocytehandleList list_triwin = callback->getImageWindowList();
    for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
        if(callback->getImageName(list_triwin.at(i)).contains(fname_image_seg))
        {
            window_open_flag=true;
            curwin=list_triwin[i];
            break;
        }
    }
    //qDebug()<<"check window: window_open_flag:"<<window_open_flag;
    if(!window_open_flag)
    {
        curwin=callback->newImageWindow(fname_image_seg);
        unsigned char *image_input=new unsigned char [sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]];
        memcpy(image_input,image_seg,sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]);
        Image4DSimple image_tmp;
        image_tmp.setData(image_input,sz_seg[0],sz_seg[1],sz_seg[2],sz_seg[3],VISIOCYTE_UINT8);
        callback->setImage(curwin,&image_tmp);
        callback->updateImageWindow(curwin);
    }
}

void manual_correct_dialog::check_local_3d_window()
{
    bool window_open_flag=false;
    QList <VisiocyteR_MainWindow *> allWindowList = callback->getListAll3DViewers();
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++)
    {
        if(callback->getImageName(allWindowList.at(i)).contains(fname_image) &&
               callback->getImageName((allWindowList.at(i))).contains("Local"))
        {
            window_open_flag=true;
            break;
        }
    }

    if (!window_open_flag)
    {
        callback->openROI3DWindow(curwin);
        View3DControl * visiocytelocalcontrol = callback->getLocalView3DControl(curwin);
        visiocytelocalcontrol->updateLandmark();
    }
}

int manual_correct_dialog::save(QString window_name) //need further work
{
    open_main_triview();
    //prepare Landmarkers and image
    LandmarkList LList_new;

    VISIOCYTELONG size_page=sz_img[0]*sz_img[1]*sz_img[2];
    //prepare label
    unsigned short *label = new unsigned short [size_page*3];
    memset(label,0,size_page*3);
    memset(image1Dc_spine+size_page,0,size_page);
//    vector<GOV> visual_group;

//    if (small_remover->isChecked())
//    {
//        vector<GOV> tmp_label_group;
//        QString tmp;
//        tmp=QString::fromStdString(LList_in[i].comments);
//        for (int i=0;i<LList_in.size();i++)
//        {
//            if (tmp.contains(("1")))
//                tmp_label_group.push_back(label_group[i]);
//        }

//    }

    vector<int> keep_spine;
    keep_spine.resize(LList_in.size());
    for (int i=0;i<LList_in.size();i++)
    {
        QString tmp;
        tmp=QString::fromStdString(LList_in[i].comments);

        if (tmp.contains("1"))
        {
            keep_spine[i]=1;
            GOV tmp_group=label_group[i];
            GetColorRGB(rgb,i);
            int sum_x,sum_y,sum_z;
            sum_x=sum_y=sum_z=0;
            for (int j=0;j<tmp_group.size();j++)
            {
                image1Dc_spine[tmp_group[j]->pos+size_page]=255;
                sum_x+=tmp_group[j]->x;
                sum_y+=tmp_group[j]->y;
                sum_z+=tmp_group[j]->z;

                label[tmp_group.at(j)->pos] = rgb[0];
                label[tmp_group.at(j)->pos+size_page]=rgb[1];
                label[tmp_group.at(j)->pos+2*size_page]=rgb[2];
            }
            LocationSimple tmp;
            tmp.x=sum_x/tmp_group.size();
            tmp.y=sum_y/tmp_group.size();
            tmp.z=sum_z/tmp_group.size();
            tmp.color.r=tmp.color.b=0;
            tmp.color.g=255;
            LList_new.append(tmp);
        }
//        else
//            keep_spine[i]=-1;
        else
            label_group[i].clear();
    }

    write_spine_profile("auto_proofread_spine_profile.csv");  //for using svm
    //write_svm_file("training.txt",keep_spine);
    //need to close all image windows //check 3D window
    visiocytehandleList list_triwin = callback->getImageWindowList();
    for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
        if(callback->getImageName(list_triwin.at(i)).contains(window_name))
        {
            callback->close3DWindow(list_triwin[i]);
        }
    }

    mydialog->close();
    qDebug()<<"before image set"<<sz_img[3];
    //visualize
    Image4DSimple image4d;
    unsigned char *input_image=new unsigned char [size_page*sz_img[3]];
    memcpy(input_image,image1Dc_spine,size_page*sz_img[3]);
    image4d.setData(input_image,sz_img[0],sz_img[1],sz_img[2],sz_img[3],VISIOCYTE_UINT8);
    QString final_name="proofread_image";
    callback->setImage(curwin,&image4d);
    callback->setImageName(curwin,final_name);
    callback->setLandmark(curwin,LList_new);
    callback->updateImageWindow(curwin);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    qDebug()<<"before label";
    Image4DSimple image_label;
    image_label.setData((unsigned char*)label,sz_img[0],sz_img[1],sz_img[2],sz_img[3],VISIOCYTE_UINT16);
    QString name="proofread_image_label";
    visiocytehandle new_win2=callback->newImageWindow(name);
    callback->setImage(new_win2,&image_label);
    callback->setLandmark(new_win2,LList_new);
    callback->updateImageWindow(new_win2);
    callback->open3DWindow(new_win2);
    callback->pushObjectIn3DWindow(new_win2);
    return LList_new.size();

}


bool manual_correct_dialog::maybe_save()
{
    QMessageBox mybox;
    mybox.setText("Do you want to exit without saving?");

    QPushButton *save_button = mybox.addButton(QMessageBox::Save);
    QPushButton *cancel_button=mybox.addButton(QMessageBox::Cancel);
    QPushButton *discard_button=mybox.addButton(QMessageBox::Discard);

    mybox.setDefaultButton(QMessageBox::Save);
    mybox.exec();

     if (mybox.clickedButton() == save_button) {
         save(fname_image);
         return true;
     } else if (mybox.clickedButton() == cancel_button) {
         return false;
     }
     else if (mybox.clickedButton()== discard_button) {
         //need to close all image windows //check 3D window
         visiocytehandleList list_triwin = callback->getImageWindowList();
         for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
             if(callback->getImageName(list_triwin.at(i)).contains(fname_image))
             {
                 callback->close3DWindow(list_triwin[i]);
             }
         }
         mydialog->close();
         return false;
     }
}

void manual_correct_dialog::write_svm_file(QString filename,vector<int> keep)
{
    qDebug()<<"writing svm file";
    QString outfile=edit_csv->text()+"/"+filename;
    FILE *fp2=fopen(outfile.toAscii(),"wt");
    //fprintf(fp2,"volume,max_dis,min_dis,center_dis,center_intensity,ave_nb,ave_intensity\n");
    qDebug()<<"1"<<"label group size:"<<label_group.size();
    for (int i=0;i<label_group.size();i++)
    {
        GOV tmp=label_group[i];
        //qDebug()<<"I:"<<i<<"size:"<<tmp.size();
        if (tmp.size()<=0) continue;
        fprintf(fp2,"%d ",keep[i]);
        sort(tmp.begin(),tmp.end(),sortfunc_dst);

        float max_dis=tmp.front()->dst; ///all_para.max_dis;
        float min_dis=tmp.back()->dst; ///all_para.max_dis;
        float volume=tmp.size();///4000;

        VISIOCYTELONG sum_x,sum_y,sum_z,sum_dis,sum_nb,sum_int;
        sum_x=sum_y=sum_z=sum_dis=sum_nb=sum_int=0;

        for (int j=0;j<tmp.size();j++)
        {
            sum_x+=tmp[j]->x;
            sum_y+=tmp[j]->y;
            sum_z+=tmp[j]->z;
            sum_dis+=tmp[j]->dst;
            sum_nb+=tmp[j]->neighbors_6.size();
            sum_int+=image1Dc_spine[tmp[j]->pos];
        }
        float center_x=sum_x/tmp.size();
        float center_y=sum_y/tmp.size();
        float center_z=sum_z/tmp.size();
        int center_dis=sum_dis/tmp.size();
        float nb_ave=sum_nb/(float)tmp.size();
        int int_ave=sum_int/tmp.size();
        VISIOCYTELONG pos=xyz2pos((VISIOCYTELONG)(center_x+0.5),(VISIOCYTELONG)(center_y+0.5),(VISIOCYTELONG)(center_z+0.5),
                            sz_img[0],sz_img[0]*sz_img[1]);
        int intensity=image1Dc_spine[pos];

        fprintf(fp2,"1:%d ",volume);
        fprintf(fp2,"2:%d ",max_dis);
        fprintf(fp2,"3:%d ",min_dis);
        fprintf(fp2,"4:%d ",center_dis);
        fprintf(fp2,"5:%d ",intensity);
        fprintf(fp2,"6:%.1f ",nb_ave);
        fprintf(fp2,"7:%d\n",int_ave);
    }
    fclose(fp2);
    qDebug()<<"finish writing svm file";
}

void manual_correct_dialog::write_spine_profile(QString filename)
{
    qDebug()<<"in write spine center profile"<<"eswc flag:"<<eswc_flag;

    QString outfile=edit_csv->text()+"/"+filename;

    FILE *fp2=fopen(outfile.toStdString().c_str(),"wt");
    if (eswc_flag)
        fprintf(fp2,"##marker_id,auto_detect_id,volume,max_dis,min_dis,center_dis,center_x,center_y,center_z,skel_node,skel_type,skel_node_seg,skel_node_branch,dis_to_root,tree_id\n");
    else
        fprintf(fp2,"##auto_detect_id,volume,max_dis,min_dis,center_dis,center_x,center_y,center_z,skel_node\n");
    int marker_id=0;
    for (int i=0;i<label_group.size();i++)
    {
        GOV tmp=label_group[i];
        if (tmp.size()<=0) continue;
        marker_id++;
        sort(tmp.begin(),tmp.end(),sortfunc_dst);
        int group_id=tmp.front()->intensity_label;
        int max_dis=tmp.front()->dst;
        int min_dis=tmp.back()->dst;
        int volume=tmp.size();

        VISIOCYTELONG sum_x,sum_y,sum_z,sum_dis;
        sum_x=sum_y=sum_z=sum_dis=0;

        for (int j=0;j<tmp.size();j++)
        {
            sum_x+=tmp[j]->x;
            sum_y+=tmp[j]->y;
            sum_z+=tmp[j]->z;
            sum_dis+=tmp[j]->dst;
        }
        float center_x=sum_x/tmp.size();
        float center_y=sum_y/tmp.size();
        float center_z=sum_z/tmp.size();
        int center_dis=sum_dis/tmp.size();
        //qDebug()<<"size:"<<tmp.size()<<" skel_id size:"<<skel_id_vector.size();

        int skel_id=calc_nearest_node(neuron,center_x,center_y,center_z);
        //qDebug()<<"skel_id_size:"<<skel_id_size<<" skel_id:"<<skel_id;
        if (eswc_flag)
        {
            fprintf(fp2,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.2f,%d\n",marker_id,group_id,volume,max_dis,
                    min_dis,center_dis,(int)center_x,(int)center_y,(int)center_z,skel_id,
                    neuron.listNeuron.at(skel_id).type,(int)neuron.listNeuron.at(skel_id).seg_id,
                    (int)neuron.listNeuron.at(skel_id).level, neuron.listNeuron.at(skel_id).fea_val[1],
                    (int)neuron.listNeuron.at(skel_id).fea_val[0]);
        }
        else
            fprintf(fp2,"%d,%d,%d,%d,%d,%d,%d,%d,%d\n",group_id,volume,max_dis,min_dis
                    ,center_dis,(int)center_x,(int)center_y,(int)center_z,skel_id);
    }
    fclose(fp2);
    qDebug()<<"file complete wrriting, outfile path:"<<outfile;
}

//int manual_correct_dialog::calc_nearest_node(float center_x,float center_y,float center_z)
//{
//    float distance=1e6;
//    int nearest_node_id=0;
//    for (int i=0;i<neuron.listNeuron.size();i++)
//    {
//        float tmp_dis=(center_x-neuron.listNeuron[i].x)*(center_x-neuron.listNeuron[i].x)+
//           (center_y-neuron.listNeuron[i].y)*(center_y-neuron.listNeuron[i].y)+
//                (center_z-neuron.listNeuron[i].z)*(center_z-neuron.listNeuron[i].z);
//        if (tmp_dis<distance)
//        {
//            distance=tmp_dis;
//            nearest_node_id=i;
//        }
//    }
//    return nearest_node_id;
//}

void manual_correct_dialog::dilate()
{
    open_main_triview();
    check_window();
    //qDebug()<<"in dilate now";
    int bg_thr=all_para.bgthr;
    VISIOCYTELONG size_page=sz[0]*sz[1]*sz[2];
    GOV seeds_next;

    int mid=markers->currentIndex();
    LList_adj[mid].color.r=LList_adj[mid].color.b=255;
    LList_adj[mid].color.g=70;
    GOV tmp_group = label_group[mid];

    for (int sid=0;sid<tmp_group.size();sid++)
    {
        VOI * single_voi = tmp_group[sid];
        int label_id=single_voi->intensity_label;
        for (int neid=0; neid<single_voi->neighbors_6.size();neid++)
        {
            VOI *nb=single_voi->neighbors_6[neid];
            if (image1Dc_spine[nb->pos]>bg_thr && nb->intensity_label<1&&nb->x>=x_start &&nb->y>=y_start
                    &&nb->z>=z_start && nb->x<x_end && nb->y<y_end && nb->z<z_end)
            {
                nb->intensity_label=label_id;
                seeds_next.push_back(nb);
            }
        }
    }

    if (seeds_next.size()==0)
    {
        edit_status->setPlainText("No more voxels available. Cannot dilate");
        return;
    }
    else if (seeds_next.size()>0)
    {
        edit_status->setPlainText(QString::number(seeds_next.size())+" voxels added in this round of dilation");
        tmp_group.insert(tmp_group.end(),seeds_next.begin(),seeds_next.end());
        label_group[mid].clear();
        label_group[mid]=tmp_group;
    }

    for (int i=0;i<seeds_next.size();i++)
    {
        VOI * single_voi=seeds_next[i];
        VISIOCYTELONG pos_trun=xyz2pos(single_voi->x-x_start,single_voi->y-y_start,single_voi->z-z_start
                                 ,sz[0],sz[0]*sz[1]);
        image_trun[pos_trun+size_page]=255;
        //image_trun[pos_trun+2*size_page]=255;
    }

    unsigned char *dilate_tmp =new unsigned char [sz[0]*sz[1]*sz[2]*sz[3]];
    memcpy(dilate_tmp,image_trun,sz[0]*sz[1]*sz[2]*sz[3]);
    edit_flag=true;

    Image4DSimple image4d_tmp;
    image4d_tmp.setData(dilate_tmp,sz[0],sz[1],sz[2],sz[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d_tmp);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->setLandmark(curwin,LList_adj);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);

    LList_adj[mid].color.r=LList_adj[mid].color.b=LList_adj[mid].color.g=0;
}

void manual_correct_dialog::erode()
{
    open_main_triview();
    check_window();
    //qDebug()<<"in erode now";
    VISIOCYTELONG size_page=sz[0]*sz[1]*sz[2];

    int mid=markers->currentIndex();
    GOV tmp_group = label_group[mid];

    LList_adj[mid].color.b=LList_adj[mid].color.r=255;
    LList_adj[mid].color.g=70;

    if (tmp_group.size()==0)
    {
        edit_status->setPlainText("No more voxels left. Cannot erode");
        return;
    }
    sort(tmp_group.begin(),tmp_group.end(),sortfunc_dst_ascend); //ascending
    //qDebug()<<"sort done:"<<"size:"<<tmp_group.size();

    int min_dis=tmp_group.front()->dst;
    int vid_begin, vid_end;
    vid_begin=vid_end=0;

    while(vid_end<tmp_group.size() && tmp_group[vid_end]->dst==min_dis){
        vid_end++;
        continue;
    }
    if (vid_begin==vid_end)
    {
       edit_status->setPlainText("No more voxels left. Cannot erode");
       return;
    }

    for (int i=vid_begin;i<vid_end;i++)
    {
        VOI * single_voi= tmp_group[i];
        single_voi->intensity_label=-1;
        VISIOCYTELONG trun_pos= xyz2pos(single_voi->x-x_start ,single_voi->y-y_start,
                                  single_voi->z-z_start,sz[0],sz[0]*sz[1]);
        image_trun[trun_pos+size_page]=0;
    }
    edit_status->setPlainText(QString::number(vid_end-vid_begin)+" voxels deleted in this round of erosion");
    //qDebug()<<"~~~erode~~~~decrease:"<<vid_end-vid_begin;

    GOV update_group;
    for (int i=vid_end;i<tmp_group.size();i++)
    {
        update_group.push_back(tmp_group[i]);
    }

    label_group[mid].clear();
    label_group[mid]=update_group;
    //qDebug()<<"new group update";
    unsigned char *erode_tmp =new unsigned char [sz[0]*sz[1]*sz[2]*sz[3]];
    memcpy(erode_tmp,image_trun,sz[0]*sz[1]*sz[2]*sz[3]);
    edit_flag=true;

    Image4DSimple image4d_tmp;
    image4d_tmp.setData(erode_tmp,sz[0],sz[1],sz[2],sz[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d_tmp);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->setLandmark(curwin,LList_adj);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    LList_adj[mid].color.r=LList_adj[mid].color.b=LList_adj[mid].color.g=0;
}

void manual_correct_dialog::reset_edit()
{
    reset_label_group();
    reset_image_data();
    int mid=markers->currentIndex();
    edit_flag=false;
    LList_adj[mid].color.b=LList_adj[mid].color.r=255;
    LList_adj[mid].color.g=70;
    unsigned char *reset_tmp =new unsigned char [sz[0]*sz[1]*sz[2]*sz[3]];
    memcpy(reset_tmp,image_trun,sz[0]*sz[1]*sz[2]*sz[3]);
    edit_status->setPlainText("Image reset");

    Image4DSimple image4d_tmp;
    image4d_tmp.setData(reset_tmp,sz[0],sz[1],sz[2],sz[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d_tmp);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->setLandmark(curwin,LList_adj);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    LList_adj[mid].color.r=LList_adj[mid].color.b=LList_adj[mid].color.g=0;
}


int manual_correct_dialog::save_edit()
{
    QMessageBox mybox;
    mybox.setText("<b>The spine has been edited.<\b>");
    QString info="-Accept: save edit and keep the spine<br> -Delete: delete the spine<br>"
            "-Discard: discard the edit<br> -Cancel: Do nothing";
    mybox.setInformativeText(info);

    QPushButton *accept_button=mybox.addButton(tr("Accept"),QMessageBox::ActionRole);
    QPushButton *delete_button=mybox.addButton(tr("Delete"),QMessageBox::ActionRole);
    QPushButton *discard_button=mybox.addButton(QMessageBox::Discard);
    QPushButton *cancel_button=mybox.addButton(QMessageBox::Cancel);

    mybox.setDefaultButton(accept_button);
    mybox.exec();

     if (mybox.clickedButton() == accept_button) {
         accept_marker();
         return 1;
     } else if (mybox.clickedButton() == cancel_button) {
         return 4;
     } else if (mybox.clickedButton() == discard_button) {
         reset_image_data();
         reset_label_group();
         edit_flag=false;
         return 3;
     } else if (mybox.clickedButton()==delete_button)
     {
         delete_marker();
         return 2;
     }
}

void manual_correct_dialog::check_neuron_tree()
{
    qDebug()<<"check neuron tree"<<sz_img[0];
    map<int,bool> outlier_flag;
    QList<NeuronSWC> tmp_list;
    QHash <int, int>  hashNeuron;
    tmp_list.clear();

    for (int i=0;i<neuron.listNeuron.size();i++)
    {
        if (outlier_flag[i]>0)
            continue;

        if (neuron.listNeuron[i].x<0 ||neuron.listNeuron[i].x>sz_img[0]-1 ||
                neuron.listNeuron[i].y<0 ||neuron.listNeuron[i].y>sz_img[1]-1
                ||neuron.listNeuron[i].z<0 || neuron.listNeuron[i].z>sz_img[2]-1)
        {
            outlier_flag[i]=1;
            continue;
        }
        NeuronSWC tmp_swc;
        tmp_swc.x=neuron.listNeuron[i].x;
        tmp_swc.y=neuron.listNeuron[i].y;
        tmp_swc.z=neuron.listNeuron[i].z;
        tmp_swc.n=neuron.listNeuron[i].n;

        int parent=neuron.listNeuron[i].parent;
        int parent_node=neuron.hashNeuron.value(parent);

        if (neuron.listNeuron[parent_node].x<0 || neuron.listNeuron[parent_node].x>sz_img[0]-1
           || neuron.listNeuron[parent_node].y<0 ||neuron.listNeuron[parent_node].y>sz_img[1]-1
           || neuron.listNeuron[parent_node].z<0||neuron.listNeuron[parent_node].z>sz_img[2]-1)
        {
            tmp_swc.parent=-1;
            outlier_flag[parent_node]=1;
        }
        else
            tmp_swc.parent=parent;

        tmp_list.append(tmp_swc);
        hashNeuron.insert(tmp_swc.n, tmp_list.size()-1);
    }
    checked_neuron.listNeuron=tmp_list;
    checked_neuron.hashNeuron=hashNeuron;
    //writeSWC_file("test.swc",checked_neuron);
    //qDebug()<<"before check:"<<neuron.listNeuron.size()<<"after check:"<<tmp_list.size();
    return;
}

//vector<vector<int> > manual_correct_dialog::build_parent_LUT(NeuronTree neuron_tmp)
//{
//    int size=neuron_tmp.listNeuron.size();
//    vector<vector<int> > parent_LUT;
//    parent_LUT.clear();
//    parent_LUT.resize(size);
//    for (int i=0;i<size;i++)
//    {
//        int parent=neuron_tmp.listNeuron.at(i).parent;
//        if (parent==-1)
//        {
//            continue;
//        }
//        parent_LUT[neuron_tmp.hashNeuron.value(parent)].push_back(i);
//    }

//    qDebug()<<"building new parnet LUT";
////    for (int i=0;i<parent_LUT.size();i++)
////    {
////        qDebug()<<"parent_LUT size:"<<i<<":"<<parent_LUT[i].size();
////    }
//    return parent_LUT;
//}

//vector<vector<int> > manual_correct_dialog::neurontree_divide_swc(NeuronTree neuron_tmp)
//{
//    float distance_thresh=all_para.max_dis*5;
//    qDebug()<<"in swc divide"<<distance_thresh;
//    vector<int> leaf_nodes_id;
//    vector<vector <int> > parent_LUT = build_parent_LUT(neuron_tmp);
//    for (int i=0;i<neuron_tmp.listNeuron.size();i++)
//    {
//        if (parent_LUT[i].size()==0)
//        {
//           leaf_nodes_id.push_back(i);
//        }
//    }
//    qDebug()<<"leaf nodes:"<<leaf_nodes_id.size();
//    map<int,bool> used_flag; //use the idex starting from 0
//    vector<vector<int> > nt_seg;

//    for (int i=0;i<leaf_nodes_id.size();i++)
//    {
//        //qDebug()<<"i:"<<i;

//        float between_distance,accu_distance;
//        int start_node,parent_node,parent,next_parent_node;

//        start_node=leaf_nodes_id[i];
//        parent=neuron_tmp.listNeuron[start_node].parent;
//        parent_node=neuron_tmp.hashNeuron.value(parent);
//        used_flag[start_node]=1;
//        vector<int> one_nt;
//        one_nt.push_back(start_node);
//        accu_distance=between_distance=calc_between_dis(start_node,parent_node);

//        while (true)
//        {
//            while(accu_distance<distance_thresh && parent!=-1 && used_flag[parent_node]<=0)
//            {
//                one_nt.push_back(parent_node);
//                used_flag[parent_node]=1;
//                parent=neuron_tmp.listNeuron[parent_node].parent;
//                next_parent_node=neuron_tmp.hashNeuron.value(parent);
//                between_distance=calc_between_dis(parent_node,next_parent_node);
//                accu_distance+=between_distance;
//                parent_node=next_parent_node;

////                if (nt_seg.size()==5)
////                    qDebug()<<"5:accu_distance:"<<accu_distance<<"start_node:"<<start_node
////                     <<"x:"<<neuron.listNeuron[start_node].x<<"parent node:"<<parent_node<<"x:"<<neuron.listNeuron[parent_node].x;
//            }
//            if (parent==-1||used_flag[parent_node]>0)
//            {
//               nt_seg.push_back(one_nt);
//               break;
//            }
//            else
//            {
//               nt_seg.push_back(one_nt);
//               one_nt.clear();
//               start_node=parent_node;
//               one_nt.push_back(start_node);
//               used_flag[start_node]=1;
//               parent=neuron_tmp.listNeuron[start_node].parent;
//               parent_node=neuron_tmp.hashNeuron.value(parent);
//               accu_distance=between_distance=calc_between_dis(start_node,parent_node);
//            }
//        }
//           //qDebug()<<"nt_seg size:"<<nt_seg.size();
//    }
//    qDebug()<<"After division swc. We have "<<nt_seg.size() <<" windows!";
////    for (int i=0;i<nt_seg.size();i++)
////    {
////        qDebug()<<"size:"<<nt_seg[i].size()<<" start:"<<nt_seg[i].front()<<"x:"<<neuron_tmp.listNeuron.at(nt_seg[i].front()).x-neuron_tmp.listNeuron.at(nt_seg[i].front()).r<<
////                 " end:"<<nt_seg[i].back()<<"x:"<<neuron_tmp.listNeuron.at(nt_seg[i].back()).x+neuron_tmp.listNeuron.at(nt_seg[i].back()).r;
////        vector<VISIOCYTELONG> coord(6,0);
////        coord=image_seg_plan(nt_seg[i],neuron_tmp);
////        qDebug()<<"xyz min:"<<coord[0]<<":"<<coord[1]<<":"<<coord[2]<<"xyz max:"<<coord[3]<<":"
////               <<coord[4]<<":"<<coord[5];
////    }
//    return nt_seg;
//}

float manual_correct_dialog::calc_between_dis(int node1_id,int node2_id)
{
    float distance=0;
    distance=(neuron.listNeuron[node1_id].x-neuron.listNeuron[node2_id].x)*(neuron.listNeuron[node1_id].x-neuron.listNeuron[node2_id].x)
            +(neuron.listNeuron[node1_id].y-neuron.listNeuron[node2_id].y)*(neuron.listNeuron[node1_id].y-neuron.listNeuron[node2_id].y)
            +(neuron.listNeuron[node1_id].z-neuron.listNeuron[node2_id].z)*(neuron.listNeuron[node1_id].z-neuron.listNeuron[node2_id].z);
    return (sqrt(distance));
}

void manual_correct_dialog::set_visualize_image_marker(vector<int> one_seg,int seg_id)
{
    vector<VISIOCYTELONG> coord(6,0);
    coord=image_seg_plan(one_seg,checked_neuron);

    x_min=coord[0];
    y_min=coord[1];
    z_min=coord[2];
    x_max=coord[3];
    y_max=coord[4];
    z_max=coord[5];

    qDebug()<<"xyz_min:"<<x_min<<":"<<y_min<<":"<<z_min;
    qDebug()<<"xyz max:"<<x_max<<":"<<y_max<<":"<<z_max;

    sz_seg[0]=x_max-x_min+1;
    sz_seg[1]=y_max-y_min+1;
    sz_seg[2]=z_max-z_min+1;
    sz_seg[3]=3;
    qDebug()<<"sz:"<<sz_seg[0]<<":"<<sz_seg[1]<<":"<<sz_seg[2];
    if (image_seg!=0)
    {
        delete [] image_seg;
        image_seg=0;
    }
    image_seg=new unsigned char[sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]];
    memset(image_seg,0,sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]);

    for (VISIOCYTELONG dx=x_min;dx<x_max;dx++){
        for (VISIOCYTELONG dy=y_min;dy<y_max;dy++){
            for (VISIOCYTELONG dz=z_min;dz<z_max;dz++){
                VISIOCYTELONG pos=xyz2pos(dx,dy,dz,sz_img[0],sz_img[0]*sz_img[1]);
                VISIOCYTELONG pos1=xyz2pos(dx-x_min,dy-y_min,dz-z_min,sz_seg[0],sz_seg[0]*sz_seg[1]);
                image_seg[pos1]=image1Dc_spine[pos];
            }
        }
    }

    LList_seg.clear();
    //find all the spines in this set.
    //also need to check if the whole spine is displayed
    for (int j=0;j<LList_in.size();j++)
    {

        if (LList_in[j].x-1<=x_min || LList_in[j].x-1>=x_max || LList_in[j].y-1<=y_min
            || LList_in[j].y-1>=y_max || LList_in[j].z-1<=z_min ||LList_in[j].z-1>=z_max )
            continue;

        GOV tmp_spine= label_group [j];
        if (LList_in.at(j).category!=seg_id)  //need to check if the spine is complete
        {
            bool incomplete_flag=false;
            for (int k=0;k<tmp_spine.size();k++)
            {
                if (tmp_spine[k]->x<x_min || tmp_spine[k]->x > x_max || tmp_spine[k]->y <y_min
                        || tmp_spine[k]->y>y_max || tmp_spine[k]->z<z_min || tmp_spine[k]->z>z_max)
                {
                    incomplete_flag=true;
                    break;
                }
            }
            if (incomplete_flag) //if incomplete, not to include the spine
                continue;
            if (LList_in.at(j).category==0)
                LList_in[j].category=seg_id;
        }


        for (int k=0;k<tmp_spine.size();k++)
        {
            VISIOCYTELONG x=tmp_spine[k]->x-x_min;
            VISIOCYTELONG y=tmp_spine[k]->y-y_min;
            VISIOCYTELONG z=tmp_spine[k]->z-z_min;
            VISIOCYTELONG pos=xyz2pos(x,y,z,sz_seg[0],sz_seg[0]*sz_seg[1]);
            image_seg[pos+sz_seg[0]*sz_seg[1]*sz_seg[2]]=255;
        }
        LocationSimple tmp;
        tmp.x=LList_in[j].x-x_min;
        tmp.y=LList_in[j].y-y_min;
        tmp.z=LList_in[j].z-z_min;
        tmp.color.r=LList_in[j].color.r;
        tmp.color.g=LList_in[j].color.g;
        tmp.color.b=LList_in[j].color.b;
        tmp.name=LList_in[j].name;
        tmp.comments=LList_in[j].comments;
        LList_seg.append(tmp);
    }

//    QString filename=QString::number(one_seg[0])+"_"+QString::number(one_seg[1])+".visiocyteraw";
//    unsigned char *image_copy=new unsigned char[sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]];
//    memcpy(image_copy,image_seg,sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]);
//    simple_saveimage_wrapper(*callback,filename.toStdString().c_str(),image_copy,sz_seg,VISIOCYTE_UINT8);

    qDebug()<<"set visualize window."<<LList_seg.size()<<"markers have been found";
}


void manual_correct_dialog::standing_segment_dialog()
{
    mydialog=new QDialog();
    mydialog->setWindowTitle("spineDetector_proofread_by_segment");
    qDebug()<<"size:"<<mydialog->size();
    mydialog->setFixedWidth(500);
    QGridLayout *layout2=new QGridLayout;

    segments=new QComboBox;
    for (int i=0;i<segment_neuronswc.size();i++)
        segments->addItem(QString("Segment ")+QString::number(i+1));
    //segments->setFixedWidth(250);
    segments->setCurrentIndex(0);
    layout2->addWidget(segments,0,0,1,3);

    QPushButton *reset_seg=new QPushButton(tr("Reset segment"));
    QPushButton *finish_seg=new QPushButton(tr("Finish segment"));
    QPushButton *skip_seg=new QPushButton(tr("Next segment"));
    layout2->addWidget(finish_seg,1,0,1,2);
    layout2->addWidget(skip_seg,1,2,1,2);
    layout2->addWidget(reset_seg,1,4,1,2);

    QFrame *line_1 = new QFrame();
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    layout2->addWidget(line_1,2,0,1,6);

    QLabel *spine_groups=new QLabel(tr("Spine groups:"));
    layout2->addWidget(spine_groups,3,0,1,6);
    QLabel *multiple=new QLabel(tr("Press ctrl/shift to select multiple markers"));
    layout2->addWidget(multiple,4,0,1,6);

    list_markers=new QListWidget();
    layout2->addWidget(list_markers,5,0,8,4);
    list_markers->setSelectionMode(QAbstractItemView::ExtendedSelection);
    list_markers->setFocusPolicy(Qt::NoFocus);

    QPushButton *accept=new QPushButton(tr("Accept"));
    QPushButton *reject=new QPushButton(tr("Reject"));

    layout2->addWidget(accept,5,4,1,2);
    layout2->addWidget(reject,6,4,1,2);

    QPushButton *dilate = new QPushButton(tr("Dilate"));
    QPushButton *erode =new QPushButton (tr("Erode"));
    QPushButton *reset = new QPushButton (tr("Reset"));
    layout2->addWidget(dilate,7,4,1,2);
    layout2->addWidget(erode,8,4,1,2);
    layout2->addWidget(reset,9,4,1,2);

    QLabel *infobox=new QLabel(tr("Info:"));
    layout2->addWidget(infobox,13,0,1,2);
    edit_seg = new QPlainTextEdit;
    edit_seg->setReadOnly(true);
    edit_seg->setFixedHeight(60);
    layout2->addWidget(edit_seg,14,0,1,6);

    QPushButton *button_save=new QPushButton;
    button_save->setText("Finish proofreading");
    layout2->addWidget(button_save,15,0,1,2);
//    small_remover=new QCheckBox;
//    small_remover->setText(QObject::tr("Remove groups < min spine pixel"));
//    small_remover->setChecked(false);
//    layout2->addWidget(small_remover,6,2,1,4);

    mydialog->setLayout(layout2);

    connect(button_save,SIGNAL(clicked()),this,SLOT(finish_proof_dialog_seg_view()));
    connect(segments,SIGNAL(currentIndexChanged(int)),this,SLOT(segment_change()));
    connect(accept,SIGNAL(clicked()),this,SLOT(accept_marker_for_seg_view()));
    connect(reject,SIGNAL(clicked()),this,SLOT(reject_marker_for_seg_view()));
    connect(dilate,SIGNAL(clicked()),this,SLOT(dilate_for_seg_view()));
    connect(erode,SIGNAL(clicked()),this,SLOT(erode_for_seg_view()));
    connect(reset,SIGNAL(clicked()),this,SLOT(reset_marker_clicked_for_seg_view()));
    connect(skip_seg,SIGNAL(clicked()),this,SLOT(skip_segment_clicked()));
    connect(finish_seg,SIGNAL(clicked()),this,SLOT(finish_seg_clicked()));
    connect(reset_seg,SIGNAL(clicked()),this,SLOT(reset_segment_clicked()));
    segment_change();
    mydialog->show();
}


void manual_correct_dialog::finish_seg_clicked()
{
    for (int i=0;i<LList_seg.size();i++)
    {
        QString tmp_comment= QString::fromStdString(LList_seg[i].comments);
        QString tmp_name=QString::fromStdString(LList_seg[i].name);
        int idx=tmp_name.toInt()-1;
        if(tmp_comment.contains("1")||tmp_comment.contains("0"))
        {
          LList_in[idx].comments=QString::number(1).toStdString();
          LList_in[idx].color.r=LList_in[idx].color.b=0;
          LList_in[idx].color.g=255;
          LList_seg[i].color.r=LList_seg[i].color.b=0;
          LList_seg[i].color.g=255;
         }
    }
    int seg_idx=segments->currentIndex();
    segments->setItemText(seg_idx,"Segment "+ QString::number(seg_idx+1)+" finished");
    seg_edit_flag=false;
    callback->setLandmark(curwin,LList_seg);
    callback->pushObjectIn3DWindow(curwin);
    return;
}

void manual_correct_dialog::reset_segment_clicked()
{
    reset_segment();
    seg_edit_flag=false;
    int seg_idx=segments->currentIndex();
    segments->setItemText(seg_idx,"Segment "+ QString::number(seg_idx+1));
    segment_change();
}

void manual_correct_dialog::skip_segment_clicked()
{
    int seg_id=segments->currentIndex();
    if (seg_id+1<segment_neuronswc.size())
        segments->setCurrentIndex(seg_id+1);
    else
    {
        edit_seg->clear();
        edit_seg->setPlainText("This is the last segment.");
        return;
    }
}

void manual_correct_dialog::segment_change()
{
    if (seg_edit_flag)
    {
        if (!save_seg_edit_for_seg_view())
        {
            return;
        }
    }
    //check whether previous editing needs saving
    edit_flag=false;
    if(segments->count()==0) return;
    edit_seg->clear();
    int seg_id=segments->currentIndex();

    //step 1: check whether main-triview is open
    open_main_triview();

    //step 2:set data ready and open local tri-view/3d window
    set_visualize_image_marker(segment_neuronswc[seg_id],seg_id+1);

    //step 3: check whether local triview is open
    check_window_seg();

    //step 4: get the list of markers
    qDebug()<<"LList seg size;"<<LList_seg.size();
    list_markers->clear();
    disconnect(list_markers,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(marker_in_one_seg()));
    for (int i=0;i<LList_seg.size();i++)
    {
//        QString curstr="marker "+ QString::number(i+1);
//        list_markers->addItem(new QListWidgetItem(curstr));
        QString tmp_comment=QString::fromStdString(LList_seg[i].comments);
        if (tmp_comment.contains("1"))
            list_markers->addItem("marker "+ QString::number(i+1)+ " accepted");
        else if (tmp_comment.contains("2"))
            list_markers->addItem("marker "+QString::number(i+1)+ " rejected");
        else
            list_markers->addItem("marker "+ QString::number(i+1));
    }
    list_markers->setCurrentItem(list_markers->item(0));

    connect(list_markers,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(marker_in_one_seg()));

    //step 5: get the image ready
    //qDebug()<<"getting the image ready";
    Image4DSimple image4d;
    unsigned char * image_input=new unsigned char [sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]];
    memcpy(image_input,image_seg,sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]);
    qDebug()<<"sz_seg size:"<<sz_seg[0]<<":"<<sz_seg[1]<<":"<<sz_seg[2];
    qDebug()<<"xyz_min:"<<x_min<<":"<<y_min<<":"<<z_min;

    image4d.setData(image_input,sz_seg[0],sz_seg[1],sz_seg[2],sz_seg[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d);
    callback->setLandmark(curwin,LList_seg);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    prev_seg=seg_id;
}


void manual_correct_dialog::marker_in_one_seg()
{
    qDebug()<<"in markerin one seg";
    edit_seg->clear();
    qDebug()<<"edit_flag:"<<edit_flag;
    int mid=list_markers->currentRow();
    if (edit_flag)
    {
       int ret;
       ret=save_edit_for_seg_view();
       if (ret==4)
       {
           list_markers->setCurrentItem(list_markers->item(prev_idx));
           return;
       }
       else
        list_markers->setCurrentItem(list_markers->item(mid));
    }

//    //Focus on this marker on tri-view
    TriviewControl * p_control = callback->getTriviewControl(curwin);
    p_control->setFocusLocation((long)LList_seg.at(mid).x+1,
                                (long)LList_seg.at(mid).y+1,(long)LList_seg.at(mid).z+1);

    //update marker in 2 tri-view and one 3D
    // if this markers is not determined,Landmark color change
    QString tmp_name=QString::fromStdString(LList_seg.at(mid).name);
    int idx=tmp_name.toInt()-1;//idx in LList_in starting from 1
    QString status=QString::fromStdString(LList_in[idx].comments);
    qDebug()<<"mid:"<<mid<<"idx"<<idx<<"comments:"<<status;
    if (status.contains("0"))
    {
//        LList_in[idx].color.r=LList_in[idx].color.b=255;
        LList_seg[mid].color.r=LList_seg[mid].color.b=255;
//        LList_in[idx].color.g=70;
        LList_seg[mid].color.g=70;
    }

    qDebug()<<"before setting up image";
    Image4DSimple image4d;
    unsigned char * image_input=new unsigned char [sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]];
    memcpy(image_input,image_seg,sz_seg[0]*sz_seg[1]*sz_seg[2]*sz_seg[3]);

    //highlight the selected group
    GOV tmp_spine=label_group[idx];
    qDebug()<<"number of voxels in this spine:"<<tmp_spine.size();
    qDebug()<<"xyz_min:"<<x_min<<":"<<y_min<<":"<<z_min;
    for (int k=0;k<tmp_spine.size();k++)
    {
        VISIOCYTELONG x=tmp_spine[k]->x-x_min;
        VISIOCYTELONG y=tmp_spine[k]->y-y_min;
        VISIOCYTELONG z=tmp_spine[k]->z-z_min;
        VISIOCYTELONG pos=xyz2pos(x,y,z,sz_seg[0],sz_seg[0]*sz_seg[1]);
        image_input[pos+2*sz_seg[0]*sz_seg[1]*sz_seg[2]]=255;
    }
    qDebug()<<"before image4d setdata"<<sz_seg[0]<<":"<<sz_seg[1];
    image4d.setData(image_input,sz_seg[0],sz_seg[1],sz_seg[2],sz_seg[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d);
    callback->setLandmark(curwin,LList_seg);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    callback->setLandmark(main_win,LList_in);

    //Step 6: reset marker color back to original color
//    LList_in[idx].color.r=LList_in[idx].color.b=LList_in[idx].color.g=0;
    LList_seg[mid].color.r=LList_seg[mid].color.b=LList_seg[mid].color.g=0;
    //qDebug()<<"~~~~marker roi finished";
    prev_idx=mid;

}

void manual_correct_dialog::reject_marker_for_seg_view()
{
    open_main_triview();
    check_window_seg();
    qDebug()<<"delete marker";
    //update landmarks color and status
    QList<QListWidgetItem*> sel_list=list_markers->selectedItems();
    qDebug()<<"this many selected:"<<sel_list.size();
    edit_seg->clear();
    for (int i=0;i<sel_list.size();i++)
    {
        int mid= list_markers->row(sel_list[i]);

        LList_seg[mid].color.g=LList_seg[mid].color.b=0;
        LList_seg[mid].color.r=255;

        QString tmp_name=QString::fromStdString(LList_seg.at(mid).name);
        int idx=tmp_name.toInt()-1;//idx in LList_in starting from 1

        LList_in[idx].color.g=LList_in[idx].color.b=0;
        LList_in[idx].color.r=255;
        LList_in[idx].comments=QString::number(2).toStdString(); //rejected marker
        LList_seg[mid].comments=QString::number(2).toStdString();

        sel_list[i]->setText("marker " + QString::number(mid+1) + " rejected");
        edit_seg->appendPlainText("marker: " + QString::number(mid+1) + " rejected");
    }

    edit_flag=false;
    seg_edit_flag=true;
    callback->setLandmark(curwin,LList_seg);
    callback->pushObjectIn3DWindow(curwin);

}

void manual_correct_dialog::accept_marker_for_seg_view()
{
    open_main_triview();
    check_window_seg();
    qDebug()<<"accept marker";
    //update landmarks color and status
    QList<QListWidgetItem*> sel_list=list_markers->selectedItems();
    edit_seg->clear();
    for (int i=0;i<sel_list.size();i++)
    {
        int mid= list_markers->row(sel_list[i]);

        LList_seg[mid].color.r=LList_seg[mid].color.b=0;
        LList_seg[mid].color.g=255;

        QString tmp_name=QString::fromStdString(LList_seg.at(mid).name);
        int idx=tmp_name.toInt()-1;

        LList_in[idx].color.r=LList_in[idx].color.b=0;
        LList_in[idx].color.g=255;
        LList_in[idx].comments=QString::number(1).toStdString(); //accept marker
        LList_seg[mid].comments=QString::number(1).toStdString();

        sel_list[i]->setText("marker " + QString::number(mid+1) + " accepted");
        edit_seg->appendPlainText("marker: " + QString::number(mid+1) + " accepted");
    }

    edit_flag=false;
    seg_edit_flag=true;
    callback->setLandmark(curwin,LList_seg);
    callback->pushObjectIn3DWindow(curwin);
}

void manual_correct_dialog::dilate_for_seg_view()
{
    open_main_triview();
    check_window_seg();
    edit_seg->clear();
    int bg_thr=all_para.bgthr;
    qDebug()<<"in dilate now"<<sz_seg[0]<<":"<<sz_seg[1]<<":"<<sz_seg[2]<<"bg_thr"<<bg_thr;
    VISIOCYTELONG size_page=sz_seg[0]*sz_seg[1]*sz_seg[2];
    GOV seeds_next;
    seeds_next.clear();

    int mid=list_markers->currentRow();
    list_markers->item(mid)->setText("marker " + QString::number(mid+1));
    LList_seg[mid].color.r=LList_seg[mid].color.b=255;
    LList_seg[mid].color.g=70;
    QString tmp_name=QString::fromStdString(LList_seg[mid].name);
    int idx=tmp_name.toInt()-1;

    GOV tmp_group = label_group[idx];
    qDebug()<<"before tmp_group"<<tmp_group.size()<<"idx:"<<idx;
    for (int sid=0;sid<tmp_group.size();sid++)
    {
        VOI * single_voi = tmp_group[sid];
        int label_id=single_voi->intensity_label;
        for (int neid=0; neid<single_voi->neighbors_6.size();neid++)
        {
            VOI *nb=single_voi->neighbors_6[neid];
            if (image1Dc_spine[nb->pos]>bg_thr && nb->intensity_label<1&&nb->x>=x_min &&nb->y>=y_min
                    &&nb->z>=z_min && nb->x<x_max && nb->y<y_max && nb->z<z_max)
            {
                nb->intensity_label=label_id;
                seeds_next.push_back(nb);
                //qDebug()<<"newvoxel:"<<nb->pos;
            }
        }
    }

    if (seeds_next.size()<=0)
    {
        edit_seg->setPlainText("No more voxels available. Cannot dilate");
        return;
    }

    edit_seg->setPlainText(QString::number(seeds_next.size())+" voxels added in this round of dilation");
    tmp_group.insert(tmp_group.end(),seeds_next.begin(),seeds_next.end());

    label_group[idx].clear();
    label_group[idx]=tmp_group;

    for (int i=0;i<seeds_next.size();i++)
    {
        VOI * single_voi=seeds_next[i];
        VISIOCYTELONG pos_trun=xyz2pos(single_voi->x-x_min,single_voi->y-y_min,single_voi->z-z_min
                                 ,sz_seg[0],sz_seg[0]*sz_seg[1]);
        image_seg[pos_trun+size_page]=255;
    }
    unsigned char *dilate_tmp =new unsigned char [size_page*sz_seg[3]];
    memcpy(dilate_tmp,image_seg,size_page*2);
    memset(dilate_tmp+2*size_page,0,size_page);

    for (int i=0;i<tmp_group.size();i++)
    {
        VISIOCYTELONG pos_trun=xyz2pos(tmp_group[i]->x-x_min,tmp_group[i]->y-y_min,tmp_group[i]->z-z_min
                                 ,sz_seg[0],sz_seg[0]*sz_seg[1]);
        dilate_tmp[pos_trun+2*size_page]=255;
    }
    qDebug()<<"tmp_group size:"<<tmp_group.size();
    Image4DSimple image4d_tmp;
    image4d_tmp.setData(dilate_tmp,sz_seg[0],sz_seg[1],sz_seg[2],sz_seg[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d_tmp);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->setLandmark(curwin,LList_seg);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    edit_flag=true;
    LList_seg[mid].color.r=LList_seg[mid].color.b=LList_seg[mid].color.g=0;;
}

void manual_correct_dialog::erode_for_seg_view()
{
    open_main_triview();
    check_window_seg();
    edit_seg->clear();
    qDebug()<<"in erode now"<<sz_seg[0]<<":"<<sz_seg[1]<<":"<<sz_seg[2];
    VISIOCYTELONG size_page=sz_seg[0]*sz_seg[1]*sz_seg[2];

    int mid=list_markers->currentRow();

    LList_seg[mid].color.r=LList_seg[mid].color.b=255;
    LList_seg[mid].color.g=0;
    list_markers->item(mid)->setText("marker " + QString::number(mid+1));

    QString tmp_name=QString::fromStdString(LList_seg[mid].name);
    int idx=tmp_name.toInt()-1;//idx in LList_in starting from 1
    GOV tmp_group = label_group[idx];

    if (tmp_group.size()==0)
    {
        edit_seg->setPlainText("No more voxels left. Cannot erode");
        return;
    }
    sort(tmp_group.begin(),tmp_group.end(),sortfunc_dst_ascend); //ascending
    //qDebug()<<"sort done:"<<"size:"<<tmp_group.size();

    int min_dis=tmp_group.front()->dst;
    int vid_begin, vid_end;
    vid_begin=vid_end=0;

    while(vid_end<tmp_group.size() && tmp_group[vid_end]->dst==min_dis){
        vid_end++;
        continue;
    }
    if (vid_begin==vid_end)
    {
       edit_seg->setPlainText("No more voxels left. Cannot erode");
       return;
    }

    edit_seg->setPlainText(QString::number(vid_end-vid_begin)+" voxels deleted in this round of erosion. "
                           +QString::number(tmp_group.size()-vid_end)+ " voxels left");
    //qDebug()<<"~~~erode~~~~decrease:"<<vid_end-vid_begin;

    for (int i=vid_begin;i<vid_end;i++)
    {
        VISIOCYTELONG trun_pos= xyz2pos(tmp_group[i]->x-x_min ,tmp_group[i]->y-y_min,
                                  tmp_group[i]->z-z_min,sz_seg[0],sz_seg[0]*sz_seg[1]);
        image_seg[trun_pos+size_page]=0;
    }

    GOV update_group;
    for (int i=vid_end;i<tmp_group.size();i++)
    {
        tmp_group[i]->intensity_label=-1;
        update_group.push_back(tmp_group[i]);
    }

    label_group[idx].clear();
    label_group[idx]=update_group;

    unsigned char *erode_tmp =new unsigned char [size_page*sz_seg[3]];
    memcpy(erode_tmp,image_seg,size_page*2);
    memset(erode_tmp+2*size_page,0,size_page);

    for (int i=0;i<update_group.size();i++)
    {
        VISIOCYTELONG trun_pos= xyz2pos(update_group[i]->x-x_min ,update_group[i]->y-y_min,
                                  update_group[i]->z-z_min,sz_seg[0],sz_seg[0]*sz_seg[1]);
        erode_tmp[trun_pos+2*size_page]=255;
    }

    Image4DSimple image4d_tmp;
    image4d_tmp.setData(erode_tmp,sz_seg[0],sz_seg[1],sz_seg[2],sz_seg[3],VISIOCYTE_UINT8);
    callback->setImage(curwin,&image4d_tmp);
    callback->updateImageWindow(curwin);
    callback->close3DWindow(curwin);
    callback->setLandmark(curwin,LList_seg);
    callback->open3DWindow(curwin);
    callback->pushObjectIn3DWindow(curwin);
    edit_flag=true;
    LList_seg[mid].color.g=LList_seg[mid].color.b=LList_seg[mid].color.r=0;
}

void manual_correct_dialog::reset_marker_clicked_for_seg_view()
{
    int mid=list_markers->currentRow();
    reset_edit_for_seg_view(true,mid);
}

void manual_correct_dialog::reset_edit_for_seg_view(bool visual_flag,int mid)
{
    qDebug()<<"in reset_edit for seg view"<<"trun image size:"<<sz_seg[1]<<":"<<sz_seg[3]<<"visual flag:"<<visual_flag;
    qDebug()<<"xyz_min:"<<x_min<<":"<<y_min<<":"<<z_min;
    edit_seg->clear();

    if (visual_flag)
    {
        LList_seg[mid].color.r=LList_seg[mid].color.b=255;
        LList_seg[mid].color.g=0;
        edit_seg->setPlainText("marker: "+ QString::number(mid+1)+ " reset");
    }

    QString tmp_name=QString::fromStdString(LList_seg.at(mid).name);
    int idx=tmp_name.toInt()-1;//idx in LList_in starting from 1
    LList_in[idx].comments=QString("0").toStdString().c_str();
    LList_in[idx].color.r=LList_in[idx].color.g=LList_in[idx].color.b=0;
    list_markers->item(mid)->setText("marker " + QString::number(mid+1));

    VISIOCYTELONG size_page=sz_seg[0]*sz_seg[1]*sz_seg[2];
    GOV tmp_group=label_group[idx];
    //update image_seg
    for (int i=0;i<tmp_group.size();i++)
    {
        VISIOCYTELONG pos_trun=xyz2pos(tmp_group[i]->x-x_min,tmp_group[i]->y-y_min,tmp_group[i]->z-z_min
                                 ,sz_seg[0],sz_seg[0]*sz_seg[1]);
        image_seg[pos_trun+size_page]=0;
        tmp_group[i]->intensity_label=-1;
    }

    tmp_group.clear();
    tmp_group=label_group_copy[idx];
    int label_id=idx+1;

    for (int i=0;i<tmp_group.size();i++)
    {
        VISIOCYTELONG pos_trun=xyz2pos(tmp_group[i]->x-x_min,tmp_group[i]->y-y_min,tmp_group[i]->z-z_min
                                 ,sz_seg[0],sz_seg[0]*sz_seg[1]);
        image_seg[pos_trun+size_page]=255;
        tmp_group[i]->intensity_label=label_id;
    }

    label_group[idx].clear();
    label_group[idx]=tmp_group;

    unsigned char *reset_tmp =new unsigned char [size_page*sz_seg[3]];
    memcpy(reset_tmp,image_seg,size_page*2);
    memset(reset_tmp+2*size_page,0,size_page);
    for (int i=0;i<tmp_group.size();i++)
    {
        VISIOCYTELONG pos_trun=xyz2pos(tmp_group[i]->x-x_min,tmp_group[i]->y-y_min,tmp_group[i]->z-z_min
                                 ,sz_seg[0],sz_seg[0]*sz_seg[1]);
        reset_tmp[pos_trun+2*size_page]=255;
    }

    edit_flag=false;
    seg_edit_flag=true;

    if (!visual_flag)
        return;
    else
    {
        Image4DSimple image4d_tmp;
        image4d_tmp.setData(reset_tmp,sz_seg[0],sz_seg[1],sz_seg[2],sz_seg[3],VISIOCYTE_UINT8);
        callback->setImage(curwin,&image4d_tmp);
        callback->updateImageWindow(curwin);
        callback->close3DWindow(curwin);
        callback->setLandmark(curwin,LList_seg);
        callback->open3DWindow(curwin);
        callback->pushObjectIn3DWindow(curwin);
    }
    LList_seg[mid].color.r=LList_seg[mid].color.b=LList_seg[mid].color.g=0;
}

int manual_correct_dialog::save_edit_for_seg_view()
{
    QMessageBox mybox;
    mybox.setText("<b>The spine has been edited.<\b>");
    QString info="-Accept: save edit and keep the spine<br> -Delete: delete the spine<br>"
            "-Discard: discard the edit";
    mybox.setInformativeText(info);

    QPushButton *accept_button=mybox.addButton(tr("Accept"),QMessageBox::ActionRole);
    QPushButton *delete_button=mybox.addButton(tr("Delete"),QMessageBox::ActionRole);
    QPushButton *discard_button=mybox.addButton(QMessageBox::Discard);
    QPushButton *cancel_button=mybox.addButton(QMessageBox::Cancel);

    mybox.setDefaultButton(accept_button);
    mybox.exec();

    list_markers->setCurrentItem(list_markers->item(prev_idx));

     if (mybox.clickedButton() == accept_button) {
         accept_marker_for_seg_view();
         return 1;
     } else if (mybox.clickedButton() == cancel_button) {
         return 4;
     }
     else if (mybox.clickedButton() == discard_button) {
         reset_edit_for_seg_view(false,prev_idx);
         edit_flag=false;

         return 3;
     } else if (mybox.clickedButton()==delete_button)
     {
         reject_marker_for_seg_view();
         return 2;
     }
}

bool manual_correct_dialog::save_seg_edit_for_seg_view()
{
    QMessageBox mybox;
    mybox.setText("<b>Have you finished editing this segment? <\b>");
    QString info="-Yes: all spines not rejected will be accepted<br> -Discard: all changes to segment will be discarded<br>"
            "-Cancel: return to proofreading dialog";
    mybox.setInformativeText(info);
    mybox.addButton(QMessageBox::Yes);
    QPushButton *skip_button=mybox.addButton(tr("Discard"),QMessageBox::ActionRole);
    //QPushButton *reset_button=mybox.addButton(tr("reset"),QMessageBox::ActionRole);
    mybox.addButton(QMessageBox::Cancel);

    mybox.setDefaultButton(QMessageBox::Yes);
    int ret=mybox.exec();
    if (ret==QMessageBox::Yes)
    {
        for (int i=0;i<LList_seg.size();i++)
        {
            QString tmp_comment= QString::fromStdString(LList_seg[i].comments);
            QString tmp_name=QString::fromStdString(LList_seg[i].name);
            int idx=tmp_name.toInt()-1;
            if(tmp_comment.contains("1")||tmp_comment.contains("0"))
            {
              LList_in[idx].comments=QString::number(1).toStdString();
              LList_in[idx].color.r=LList_in[idx].color.b=0;
              LList_in[idx].color.g=255;
             }
        }
        segments->setItemText(prev_seg,"Segment "+ QString::number(prev_seg+1)+" finished");
        return true;
    }
    else if (ret==QMessageBox::Cancel)
    {
        disconnect(segments,SIGNAL(currentIndexChanged(int)),this,SLOT(segment_change()));
        segments->setCurrentIndex(prev_seg);
        connect(segments,SIGNAL(currentIndexChanged(int)),this,SLOT(segment_change()));
        return false;
    }

    else if (mybox.clickedButton()==skip_button)
    {
        reset_segment();
        return true;
    }
}

bool manual_correct_dialog::finish_proof_dialog_seg_view()
{
    QMessageBox mybox;
    mybox.setText("Have you finished proofreading?");
    mybox.setInformativeText("Only the accpeted markers will be kept");

    QPushButton *save_button = mybox.addButton(tr("Finish and save"),QMessageBox::ActionRole);
    QPushButton *cancel_button=mybox.addButton(QMessageBox::Cancel);
    QPushButton *discard_button=mybox.addButton(QMessageBox::Discard);

    mybox.setDefaultButton(save_button);
    mybox.exec();

     if (mybox.clickedButton() == save_button) {
         int final_landmarks_num;
         final_landmarks_num = save(fname_image_seg);
         QMessageBox mynewbox;
         QString info="After proofreading "+ QString::number(final_landmarks_num)+" spines were found\n";
         info+="The spine csv profile is saved at "+ edit_csv->text();
         mynewbox.information(0,"spine detector",info,QMessageBox::Ok);
         return true;

     } else if (mybox.clickedButton() == cancel_button) {
         return false;
     }
     else if (mybox.clickedButton()== discard_button) {
         //need to close all image windows //check 3D window
         visiocytehandleList list_triwin = callback->getImageWindowList();
         for(VISIOCYTELONG i=0; i<list_triwin.size(); i++){
             if(callback->getImageName(list_triwin.at(i)).contains(fname_image_seg))
             {
                 callback->close3DWindow(list_triwin[i]);
             }
         }
         mydialog->close();
         return false;
     }
}

void manual_correct_dialog::reset_segment()
{
    for (int i=0;i<LList_seg.size();i++)
    {
        QString tmp_name=QString::fromStdString(LList_seg.at(i).name);
        int idx=tmp_name.toInt()-1;//idx in LList_in starting from 1
        LList_in[idx].comments=QString("0").toStdString().c_str();
        LList_in[idx].color.r=LList_in[idx].color.g=LList_in[idx].color.b=LList_in[idx].category=0;
        list_markers->item(i)->setText("marker " + QString::number(i+1));
        GOV tmp_group=label_group[idx];
        for (int j=0;j<tmp_group.size();j++)
        {
            tmp_group[j]->intensity_label=-1;
        }
        tmp_group.clear();
        tmp_group=label_group_copy[idx];
        int label_id=idx+1;
        for (int j=0;j<tmp_group.size();j++)
        {
            tmp_group[j]->intensity_label=label_id;
        }
        label_group[idx].clear();
        label_group[idx]=tmp_group;
    }
    seg_edit_flag=false;
    qDebug()<<"segment reset";
}


//void manual_correct_dialog::GetColorRGB(int* rgb, int idx)
//{
//    idx=idx+1;
//    if(idx>=0){
//    idx = idx > 0 ? idx % 128 : 128 - abs(idx % 128);
//    int colorLib[128][3] = {
//            {39,16,2}, //{55, 173, 188},
//            {3, 91, 61},
//            {237, 195, 97},
//            {175, 178, 151},
//            {245, 199, 137},
//            {24, 226, 66},
//            {118, 84, 193},
//            {205, 205, 224},
//            {22, 170, 128},
//            {86, 150, 90},
//            {53, 67, 25},
//            {38, 82, 152},
//            {55,173,188}, //{39, 16, 2},
//            {197, 174, 31},
//            {183, 41, 31},
//            {174, 37, 15},
//            {34, 4, 48},
//            {18, 10, 79},
//            {132, 100, 254},
//            {112, 39, 198},
//            {189, 22, 120},
//            {75, 104, 110},
//            {222, 180, 4},
//            {6, 60, 153},
//            {236, 85, 113},
//            {25, 182, 184},
//            {200, 240, 12},
//            {204, 119, 71},
//            {11, 201, 229},
//            {198, 214, 88},
//            {49, 97, 65},
//            {5, 75, 61},
//            {52, 89, 137},
//            {32, 49, 125},
//            {104, 187, 76},
//            {206, 233, 195},
//            {85, 183, 9},
//            {157, 29, 13},
//            {14, 7, 244},
//            {134, 193, 150},
//            {188, 81, 216},
//            {126, 29, 207},
//            {170, 97, 183},
//            {107, 14, 149},
//            {76, 104, 149},
//            {80, 38, 253},
//            {27, 85, 10},
//            {235, 95, 252},
//            {139, 144, 48},
//            {55, 124, 56},
//            {239, 71, 243},
//            {208, 89, 6},
//            {87, 98, 24},
//            {247, 48, 42},
//            {129, 130, 13},
//            {94, 149, 254},
//            {111, 177, 4},
//            {39, 229, 12},
//            {7, 146, 87},
//            {56, 231, 174},
//            {95, 102, 52},
//            {61, 226, 235},
//            {199, 62, 149},
//            {51, 32, 175},
//            {232, 191, 210},
//            {57, 99, 107},
//            {239, 27, 135},
//            {158, 71, 50},
//            {104, 92, 41},
//            {228, 112, 171},
//            {54, 120, 13},
//            {126, 69, 174},
//            {191, 100, 143},
//            {187, 156, 148},
//            {18, 95, 9},
//            {104, 168, 147},
//            {249, 113, 198},
//            {145, 5, 131},
//            {104, 56, 59},
//            {112, 235, 81},
//            {73, 93, 127},
//            {207, 60, 6},
//            {77, 76, 211},
//            {35, 208, 220},
//            {141, 5, 129},
//            {182, 178, 228},
//            {179, 239, 3},
//            {209, 9, 132},
//            {167, 192, 71},
//            {154, 227, 164},
//            {200, 125, 103},
//            {224, 181, 211},
//            {3, 24, 139},
//            {218, 67, 198},
//            {163, 185, 228},
//            {196, 53, 11},
//            {39, 183, 33},
//            {56, 116, 119},
//            {23, 129, 20},
//            {42, 191, 85},
//            {78, 209, 250},
//            {247, 147, 60},
//            {74, 172, 146},
//            {51, 244, 86},
//            {38, 84, 29},
//            {197, 202, 150},
//            {20, 49, 198},
//            {90, 214, 34},
//            {178, 49, 101},
//            {86, 235, 214},
//            {161, 221, 55},
//            {17, 173, 136},
//            {132, 65, 217},
//            {210, 4, 121},
//            {241, 117, 217},
//            {137, 111, 6},
//            {129, 224, 232},
//            {73, 34, 0},
//            {81, 135, 211},
//            {172, 233, 193},
//            {43, 246, 89},
//            {153, 187, 222},
//            {85, 118, 43},
//            {119, 116, 33},
//            {163, 229, 109},
//            {45, 75, 15},
//            {15, 7, 140},
//            {144, 78, 192}
//    };
//    for(int k = 0; k < 3; k++)
//        rgb[k] = colorLib[idx][k];
//    }
//    else{
//        rgb[0]=0;
//        rgb[1]=0;
//        rgb[2]=0;
//    }
//}



void manual_correct_dialog::big_image_pipeline_start()
{
    this->close();
    get_para();
    check_neuron_tree();
    vector<vector <int> > nt_segs;
    nt_segs=neurontree_divide_swc(&checked_neuron,all_para.max_dis*6);
    if(!check_image_size())
        return;

    bool cancel_flag=false;
    QProgressDialog progress("Automatic spine detection...", "Abort", 0, nt_segs.size(), this);
    progress.setWindowModality(Qt::WindowModal);

    for (int i=0;i<nt_segs.size();i++)
    {
        //prepare for x_start,y_start,z_start,x_end...
        progress.setValue(i);
        if (progress.wasCanceled())
        {
            visiocyte_msg("Automatic spine detection CANCELLED!");
            cancel_flag=true;
            break;
        }
        vector<VISIOCYTELONG> coord(6,0);

        coord=image_seg_plan(nt_segs[i],checked_neuron);

        unsigned char * data1d = 0;
        VISIOCYTELONG *in_zz = 0;
        VISIOCYTELONG *in_sz = 0;
        int datatype;
        if (!loadRawRegion(const_cast<char *>(fname.toStdString().c_str()),data1d,in_zz,in_sz,datatype,
                           coord[0],coord[1],coord[2],coord[3],coord[4],coord[5]))
        {
            printf("can not load the region");
            if(data1d) {delete []data1d; data1d = 0;}
            return;
        }
        if (datatype!=1)
        {
            printf("cannot handle datatype other than uint8");
            if(data1d) {delete []data1d; data1d = 0;}
            return;
        }
        qDebug()<<"loading done:"<<in_zz[0]<<":"<<in_zz[1]<<":"<<in_zz[2]<<" trunc:"
               <<in_sz[0]<<":"<<in_sz[1]<<":"<<in_sz[2]<<" datatype"<<datatype;
        //prepare new neurontree maybe
        NeuronTree this_tree;
        this_tree=prep_seg_neurontree(coord);   //adj neuron to segmented view
        //qDebug()<<"loading new nt done:"<<this_tree.listNeuron.size();

        if (!auto_spine_detect_seg_image(data1d,in_sz,this_tree,i+1,coord[0],coord[1],coord[2],spine_map))
        {
            if (data1d!=0)
            {
                delete[] data1d; data1d=0;
            }
            continue;
        }

        if (data1d!=0)
        {
            delete[] data1d; data1d=0;
        }
//        //need to store the results somewhere...
        if (cancel_flag)
            return;
    }
    sz_img[3]=1;
    progress.setValue(nt_segs.size());
    QMessageBox::information(0,"Automatic spine detection finished.","Results are stored at "+folder_output);

//    unsigned char * outfile= new unsigned char [sz_img[0]*sz_img[1]*sz_img[2]];
//    memset(outfile,0,sz_img[0]*sz_img[1]*sz_img[2]);
//    for (QSet <VISIOCYTELONG>::Iterator mi=spine_map.begin();mi!=spine_map.end();mi++)
//    {
//        outfile[*mi]=255;
//    }
//    simple_saveimage_wrapper(*callback,"outfile.visiocyteraw",outfile,sz_img,VISIOCYTE_UINT8);
    return;
}

bool manual_correct_dialog::auto_spine_detect_seg_image
    (unsigned char *data1d, VISIOCYTELONG *sz, NeuronTree nt_seg, int image_id,
     VISIOCYTELONG x_start,VISIOCYTELONG y_start,VISIOCYTELONG z_start,QSet<VISIOCYTELONG> &spine_map)
{
    spine_fun spine_obj(callback,all_para,0);
    if (!spine_obj.pushImageData(data1d,sz))
        return false;
    spine_obj.pushSWCData(nt_seg);

    if(!spine_obj.init()){
        visiocyte_msg("No spine candidates were found. Please check image and swc file");
        return false;
    }

    if(!spine_obj.reverse_dst_grow())
    {
        visiocyte_msg("No spines candidates were found; Please check image and swc file");
        return false;
    }

    spine_obj.run_intensityGroup();

    spine_obj.conn_comp_nb6();

    //LList_in = spine_obj.get_center_landmarks();
    vector<GOV> label_record;
    label_record = spine_obj.get_group_label();
//    //spine_obj.saveResult();

//    //make copy for label_group
//    label_group_copy=label_group;

    VISIOCYTELONG size_page=sz[0]*sz[1]*sz[2];
    unsigned short *seg_label=new unsigned short[size_page*sizeof(unsigned short)];
    unsigned char *seg_result = new unsigned char [size_page*3];
    memset(seg_result,0,size_page*3);
    memcpy(seg_result,data1d,size_page);
    memset(seg_label,0,size_page*2*sizeof(unsigned short));

    for(int i=0; i<label_record.size(); i++)
    {
        GOV tmp = label_record[i];
        for (int j=0; j<tmp.size(); j++)
        {
            seg_result[tmp.at(j)->pos+size_page]=255;
            seg_label[tmp.at(j)->pos]=tmp.at(j)->intensity_label;
            seg_label[tmp.at(j)->pos+size_page]=tmp.at(j)->dst;
            VISIOCYTELONG x=tmp[j]->x+x_start;
            VISIOCYTELONG y=tmp[j]->y+y_start;
            VISIOCYTELONG z=tmp[j]->z+z_start;
            //VISIOCYTELONG new_pos=xyz2pos(x,y,z,sz_img[0],sz_img[0]*sz_img[1]);
            //spine_map.insert(new_pos);
        }
    }
    sz[3]=3;

    QString fname_out="result_"+ QString::number(image_id)+".visiocyteraw";
    QString img_complete=QDir(folder_output).filePath(fname_out);
    simple_saveimage_wrapper(*callback,img_complete.toStdString().c_str(),seg_result,sz,VISIOCYTE_UINT8);
    sz[3]=2;
    QString fname_label="result_label_"+QString::number(image_id)+".visiocyteraw";
    QString label_complete=QDir(folder_output).filePath(fname_label);
    simple_saveimage_wrapper(*callback,label_complete.toStdString().c_str(),(unsigned char *)seg_label,sz,VISIOCYTE_UINT16);

    QString swc_fname=QString::number(image_id)+".swc";
    QString swc_complete=QDir(folder_output).filePath(swc_fname);
    writeSWC_file(swc_complete,nt_seg);

    //output ano

    QString fname_ano ="sd_result_"+ QString::number(image_id)+".ano";
    QString ano_complete=QDir(folder_output).filePath(fname_ano);
    QFile qf_anofile(ano_complete);
    if(!qf_anofile.open(QIODevice::WriteOnly))
    {
        visiocyte_msg("Cannot open file for writing!");
        return false;
    }
    QTextStream out(&qf_anofile);
    out<<"RAWIMG="<<img_complete<<endl;
    out<<"SWCFILE="<<swc_complete<<endl;
    qf_anofile.close();

    qDebug()<<"auto spine_detect complete";
    return true;
}

NeuronTree manual_correct_dialog::prep_seg_neurontree(vector<VISIOCYTELONG> coord)
{
    NeuronTree seg_nt;
    QList<NeuronSWC> tmp_list;
    tmp_list.clear();
    for (int i=0;i<neuron.listNeuron.size();i++)
    {
//        if (neuron.listNeuron[i].x>=coord[0] && neuron.listNeuron[i].x<coord[3]
//           && neuron.listNeuron[i].y>=coord[1] && neuron.listNeuron[i].y<coord[4]
//           && neuron.listNeuron[i].z>=coord[2] && neuron.listNeuron[i].z<coord[5])
//        {
            NeuronSWC S;
            S.x=neuron.listNeuron.at(i).x-coord[0];
            S.y=neuron.listNeuron.at(i).y-coord[1];
            S.z=neuron.listNeuron.at(i).z-coord[2];
            S.r=neuron.listNeuron.at(i).r;
            S.parent=neuron.listNeuron.at(i).parent;
            S.n=neuron.listNeuron.at(i).n;
            tmp_list.append(S);
//        }
        //qDebug()<<"s_nt:"<<s_nt[i];
    }
    seg_nt.listNeuron=tmp_list;
    return seg_nt;
}

bool manual_correct_dialog::get_big_image_name()
{
    fname = QFileDialog::getOpenFileName(0, QObject::tr("Choose the input image "),
             image_load_path,QObject::tr("Images (*.raw *.tif *.lsm *.visiocytepbd *.visiocyteraw);;All(*)"));
    if (fname.isEmpty())
    {
        qDebug()<<"fname not valid";
        return false;
    }
    else
    {
        edit_load->setText(fname);
        qDebug()<<"fname:"<<fname;
        QSettings settings("VISIOCYTE plugin","spine_detector");
        settings.setValue("fname_img",edit_load->text());
        check_button();
        return true;
    }
}


vector<vector<int> > manual_correct_dialog::neurontree_divide_big_img_eswc()
{
    float distance_thresh=all_para.max_dis*5;
    qDebug()<<"in nt_divide_big_img"<<distance_thresh;
    vector<int> leaf_nodes_id;
    vector<vector <int> > parent_LUT = build_parent_LUT(&neuron);
    for (int i=0;i<neuron.listNeuron.size();i++)
    {
        if (parent_LUT[i].size()==0)
        {
           leaf_nodes_id.push_back(i);
        }
    }
    //qDebug()<<"leaf nodes:"<<leaf_nodes_id.size();
    map<int,bool> used_flag; //use the idex starting from 0
    vector<vector<int> > nt_seg;

    for (int i=0;i<leaf_nodes_id.size();i++)
    {
        //qDebug()<<"i:"<<i;

        float start_distance,accu_distance;
        int start_node,parent_node,parent;

        start_node=leaf_nodes_id[i];
        start_distance=neuron.listNeuron[start_node].fea_val[1];
        parent=neuron.listNeuron[start_node].parent;
        parent_node=neuron.hashNeuron.value(parent);
        accu_distance=start_distance-neuron.listNeuron[parent_node].fea_val[1];
        used_flag[start_node]=1;
        vector<int> one_nt;
        one_nt.push_back(start_node);

        while (true)
        {
            while(accu_distance<distance_thresh && parent!=-1 && used_flag[parent_node]<=0)
            {
                one_nt.push_back(parent_node);
                used_flag[parent_node]=1;
                accu_distance=start_distance-neuron.listNeuron[parent_node].fea_val[1];
                parent=neuron.listNeuron[parent_node].parent;
                parent_node=neuron.hashNeuron.value(parent);

//                if (nt_seg.size()==5)
//                    qDebug()<<"5:accu_distance:"<<accu_distance<<"start_node:"<<start_node
//                     <<"x:"<<neuron.listNeuron[start_node].x<<"parent node:"<<parent_node<<"x:"<<neuron.listNeuron[parent_node].x;
            }
            if (parent==-1||used_flag[parent_node]>0)
            {
               nt_seg.push_back(one_nt);
               break;
            }
            else
            {
               nt_seg.push_back(one_nt);
               one_nt.clear();
               start_node=parent_node;
               start_distance=neuron.listNeuron[start_node].fea_val[1];
               one_nt.push_back(start_node);
               used_flag[start_node]=1;
               parent=neuron.listNeuron[start_node].parent;
               parent_node=neuron.hashNeuron.value(parent);
               accu_distance=start_distance-neuron.listNeuron[parent_node].fea_val[1];
            }
        }
           qDebug()<<"nt_seg size:"<<nt_seg.size();
    }

    qDebug()<<"After division. We have "<<nt_seg.size() <<" windows!";
//    for (int i=0;i<nt_seg.size();i++)
//    {
//        if (i==4)
//        qDebug()<<"size:"<<nt_seg[i].size()<<" start:"<<nt_seg[i].front()<<"x:"<<neuron.listNeuron.at(nt_seg[i].front()).x-neuron.listNeuron.at(nt_seg[i].front()).r<<
//                 " end:"<<nt_seg[i].back()<<"x:"<<neuron.listNeuron.at(nt_seg[i].back()).x+neuron.listNeuron.at(nt_seg[i].back()).r;
//    }
    return nt_seg;
}
vector<VISIOCYTELONG> manual_correct_dialog::image_seg_plan(vector<int> seg, NeuronTree neuron_tmp)
{
    float extra_length=(float)all_para.max_dis;
    float start_x,start_y,start_z,end_x,end_y,end_z;
    start_x=start_y=start_z=1e8;
    end_x=end_y=end_z=0;

    for (int i=0;i<seg.size();i++)
    {
        NeuronSWC tmp=neuron_tmp.listNeuron.at(seg[i]);
        if (tmp.x-tmp.r<start_x)
            start_x=tmp.x-tmp.r;
        if (tmp.x+tmp.r>end_x)
            end_x=tmp.x+tmp.r;
        if (tmp.y-tmp.r<start_y)
            start_y=tmp.y-tmp.r;
        if (tmp.y+tmp.r>end_y)
            end_y=tmp.y+tmp.r;
        if (tmp.z-tmp.r<start_z)
            start_z=tmp.z-tmp.r;
        if (tmp.z+tmp.r>end_z)
            end_z=tmp.z+tmp.r;
    }
    start_x=MAX(start_x-extra_length,0);
    start_y=MAX(start_y-extra_length,0);
    start_z=MAX(start_z-extra_length,0);
    end_x=MIN(end_x+extra_length,sz_img[0]-1);
    end_y=MIN(end_y+extra_length,sz_img[1]-1);
    end_z=MIN(end_z+extra_length,sz_img[2]-1);

//    qDebug()<<"xyz_min:"<<start_x<<":"<<start_y<<":"<<start_z;
//    qDebug()<<"xyz max:"<<end_x<<":"<<end_y<<":"<<end_z;
//    qDebug()<<"size:"<<end_x-start_x+1<<":"<<end_y-start_y+1<<":"<<end_z-start_z+1;
    vector<VISIOCYTELONG> coord(6,0);
    coord[0]=(VISIOCYTELONG)start_x;
    coord[1]=(VISIOCYTELONG)start_y;
    coord[2]=(VISIOCYTELONG)start_z;
    coord[3]=(VISIOCYTELONG)end_x;
    coord[4]=(VISIOCYTELONG)end_y;
    coord[5]=(VISIOCYTELONG)end_z;

    return coord;
}

bool manual_correct_dialog::check_image_size()
{
    unsigned char * data1d = 0;
    VISIOCYTELONG *in_zz = 0;
    VISIOCYTELONG *in_sz = 0;
    int datatype;
    if (!loadRawRegion(const_cast<char *>(fname.toStdString().c_str()),data1d,in_zz,in_sz,datatype,
                               0,0,0,1,1,1))
    {
        delete []data1d;

        return false;
    }
    sz_img[0]=in_zz[0];
    sz_img[1]=in_zz[1];
    sz_img[2]=in_zz[2];
    sz_img[3]=in_zz[3];
    qDebug()<<"size check:"<<sz_img[0]<<":"<<sz_img[1]<<":"<<sz_img[2]<<":"<<sz_img[3];
    return true;

}
void manual_correct_dialog::create_big_image()
{
    QGridLayout *mygridLayout = new QGridLayout;
    QLabel* label_load = new QLabel(QObject::tr("Load Image:"));
    mygridLayout->addWidget(label_load,0,0,1,1);
    edit_load = new QLineEdit;
    edit_load->setText(""); edit_load->setReadOnly(true);
    mygridLayout->addWidget(edit_load,0,1,1,6);
    btn_load = new QPushButton("...");
    mygridLayout->addWidget(btn_load,0,7,1,1);

    QLabel* label_swc = new QLabel(QObject::tr("Load swc:"));
    mygridLayout->addWidget(label_swc,1,0,1,1);
    edit_swc = new QLineEdit;
    edit_swc->setText(""); edit_swc->setReadOnly(true);
    mygridLayout->addWidget(edit_swc,1,1,1,6);
    btn_swc = new QPushButton("...");
    mygridLayout->addWidget(btn_swc,1,7,1,1);

    QLabel* label_csv = new QLabel(QObject::tr("Output directory:"));
    mygridLayout->addWidget(label_csv,2,0,1,1);
    edit_csv = new QLineEdit;
    edit_csv->setText(""); edit_csv->setReadOnly(true);
    mygridLayout->addWidget(edit_csv,2,1,1,6);
    btn_csv = new QPushButton("...");
    mygridLayout->addWidget(btn_csv,2,7,1,1);

    QLabel *channel = new QLabel(tr("Which channel to use?"));
    channel_menu = new QComboBox;
    channel_menu->addItem("red");
    channel_menu->addItem("green");
    channel_menu->addItem("blue");
    mygridLayout->addWidget(channel,3,0,1,2);
    mygridLayout->addWidget(channel_menu,3,3,1,5);

    //para setting
    QFrame *line_1 = new QFrame();
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    mygridLayout->addWidget(line_1,4,0,1,8);

    QLabel *bg_thr = new QLabel(tr("Background threshold:"));
    mygridLayout->addWidget(bg_thr,5,0,1,6);
    QLabel *max_pixel=new QLabel (tr("Max spine volume:"));
    mygridLayout->addWidget(max_pixel,6,0,1,6);
    QLabel *min_pixel=new QLabel (tr("Min spine volume:"));
    mygridLayout->addWidget(min_pixel,7,0,1,6);
    QLabel *max_dis=new QLabel(tr("Max spine distance to surface:"));
    mygridLayout->addWidget(max_dis,8,0,1,6);
    QLabel *width_thr=new QLabel(tr("Max spine width:"));
    mygridLayout->addWidget(width_thr,9,0,1,6);

    spin_max_dis=new QSpinBox;
    spin_max_dis->setRange(5,80);
    spin_max_dis->setValue(40);
    spin_min_pixel=new QSpinBox;
    spin_min_pixel->setRange(10,100);
    spin_min_pixel->setValue(30);
    spin_bg_thr=new QSpinBox;
    spin_bg_thr->setRange(1,255);
    spin_bg_thr->setValue(90);
    spin_max_pixel=new QSpinBox;
    spin_max_pixel->setRange(2000,8000);
    spin_max_pixel->setValue(7000);
    spin_width_thr=new QSpinBox;
    spin_width_thr->setRange(10,100);
    spin_width_thr->setValue(35);
    mygridLayout->addWidget(spin_bg_thr,5,6,1,2);
    mygridLayout->addWidget(spin_max_pixel,6,6,1,2);
    mygridLayout->addWidget(spin_min_pixel,7,6,1,2);
    mygridLayout->addWidget(spin_max_dis,8,6,1,2);
    mygridLayout->addWidget(spin_width_thr,9,6,1,2);

    QFrame *line_2 = new QFrame();
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    mygridLayout->addWidget(line_2,15,0,1,8);
    btn_run = new QPushButton("Run");
    QPushButton *cancel = new QPushButton("Cancel");
    mygridLayout->addWidget(btn_run,16,2,1,2);
    mygridLayout->addWidget(cancel,16,5,1,2);

    //operation zone
    QFrame *line_3 = new QFrame();
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    mygridLayout->addWidget(line_3,17,0,1,8);


    this->setLayout(mygridLayout);
    this->setWindowTitle("Spine_detector_big_image");
    this->show();

    connect(btn_run,SIGNAL(clicked()),this,SLOT(big_image_pipeline_start()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_load, SIGNAL(clicked()), this, SLOT(get_big_image_name()));
    connect(btn_swc,SIGNAL(clicked()),this,SLOT(load_swc()));
    connect(btn_csv,SIGNAL(clicked()),this,SLOT(csv_out()));

}
