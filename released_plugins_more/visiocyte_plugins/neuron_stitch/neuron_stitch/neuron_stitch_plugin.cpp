/* neuron_stitch_plugin.cpp
 * This plugin is for link neuron segments across stacks.
 * 2014-10-07 : by Hanbo Chen
 */

#include "visiocyte_message.h"
#include <vector>
#include "neuron_stitch_plugin.h"
#include "neuron_stitch_func.h"
#include "neuron_match_clique.h"
#include "neuron_geometry_dialog.h"
#include "neuron_stitch/neuron_match_gmhash.h"
#include "../../../visiocyte_main/neuron_editing/neuron_xforms.h"
#include <iostream>
#include <fstream>
#include "performance_timer.h"

using namespace std;

Q_EXPORT_PLUGIN2(neuron_stitch, neuron_stitch)

QList<NeuronGeometryDialog* > dialogList;

QStringList neuron_stitch::menulist() const
{
	return QStringList() 
        <<tr("auto_stitch_neuron_SWC")
        <<tr("live_stitch_neuron_SWC")
        <<tr("manually_affine_neuron_SWC")
		<<tr("about");
}

QStringList neuron_stitch::funclist() const
{
	return QStringList()
        <<tr("neuron_stitch_auto")
      //  <<tr("tmp_test")
		<<tr("help");
}

void neuron_stitch::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("auto_stitch_neuron_SWC"))
    {
        domatch(callback, parent);
    }
    else if (menu_name == tr("live_stitch_neuron_SWC"))
    {
        dostitch(callback, parent);
    }
    else if (menu_name == tr("manually_affine_neuron_SWC"))
    {
        doadjust(callback, parent);
    }
    else
	{
		visiocyte_msg(tr("This plugin is for link neuron segments across stacks.. "
			"Developed by Hanbo Chen, 2014-10-07"));
	}
}

bool neuron_stitch::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    if (func_name == tr("neuron_stitch_auto"))
	{
        cout<<"==== neuron stack auto stitcher ===="<<endl;
        if(infiles.size()!=2 || outfiles.size()!=1)
        {
            qDebug("ERROR: please set input and output!");
            printHelp();
            return false;
        }

        //load neurons
        QString fname_nt0 = ((vector<char*> *)(input.at(0).p))->at(0);
        QString fname_nt1 = ((vector<char*> *)(input.at(0).p))->at(1);
        QString fname_output = ((vector<char*> *)(output.at(0).p))->at(0);
        NeuronTree* nt0 = new NeuronTree();
        if (fname_nt0.toUpper().endsWith(".SWC") || fname_nt0.toUpper().endsWith(".ESWC")){
            *nt0 = readSWC_file(fname_nt0);
        }
        if(nt0->listNeuron.size()<=0){
            qDebug()<<"failed to read SWC file: "<<fname_nt0;
            return false;
        }
        NeuronTree* nt1 = new NeuronTree();
        if (fname_nt1.toUpper().endsWith(".SWC") || fname_nt1.toUpper().endsWith(".ESWC")){
            *nt1 = readSWC_file(fname_nt1);
        }
        if(nt1->listNeuron.size()<=0){
            qDebug()<<"failed to read SWC file: "<<fname_nt1;
            return false;
        }

        neuron_match_clique matchfunc(nt0, nt1);

        //get parameters
        vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
        if(paras.size()>0){
            int tmp=atoi(paras.at(0));
            if(tmp>=0 && tmp<=2)
                matchfunc.direction=tmp;
            else
                cerr<<"error: wrong stack direction: "<<tmp<<"; use default value 2 (z direction)"<<endl;
        }
        if(paras.size()>1){
            double tmp=atof(paras.at(1));
            if(tmp>1e-10)
                matchfunc.zscale=tmp;
            else
                cerr<<"error: wrong rescale value: "<<tmp<<"; use default value 1"<<endl;
        }
        if(paras.size()>2){
            double tmp=atof(paras.at(2));
            if(tmp>0&&tmp<180)
                matchfunc.angThr_match=cos(tmp/180*M_PI);
            else
                cerr<<"error: wrong angular threshold: "<<tmp<<"; use default value 60"<<endl;
        }
        if(paras.size()>3){
            double tmp=atof(paras.at(3));
            if(tmp>=0)
                matchfunc.pmatchThr=tmp;
            else
                cerr<<"error: wrong point match distance threshold: "<<tmp<<"; use default value 100"<<endl;
        }
        if(paras.size()>4){
            double tmp=atof(paras.at(4));
            if(tmp>0)
                matchfunc.cmatchThr=tmp;
            else
                cerr<<"error: wrong triangle match distance threshold: "<<tmp<<"; use default value 100"<<endl;
        }
        if(paras.size()>5){
            int tmp=atoi(paras.at(5));
            if(tmp>0)
                matchfunc.maxClique3Num=tmp;
            else
                cerr<<"error: wrong max number of triangles: "<<tmp<<"; use default value 10000"<<endl;
        }
        if(paras.size()>6){
            double tmp=atof(paras.at(6));
            if(tmp>0)
                matchfunc.spanCand=tmp;
            else
                cerr<<"error: wrong border tips seraching span: "<<tmp<<"; use default value 20"<<endl;
        }
        if(paras.size()>7){
            double tmp=atof(paras.at(7));
            if(tmp>0)
                matchfunc.gapThr=tmp;
            else
                cerr<<"error: wrong border tips gap filter: "<<tmp<<"; use default value 0, no tips will be filtered by gap"<<endl;
        }
        if(paras.size()>8){
            double tmp=atof(paras.at(8));
            if(tmp>0)
                matchfunc.segmentThr=tmp;
            else
                cerr<<"error: wrong border tips segment filter: "<<tmp<<"; use default value 0, no tips will be filtered by segment"<<endl;
        }
        if(paras.size()>9){
            int tmp=atoi(paras.at(9));
            if(tmp>0)
                matchfunc.spineLengthThr=tmp;
            else
                cerr<<"error: wrong border tips spine size filter: "<<tmp<<"; use default value 0, no tips will be considered as spine"<<endl;
        }
        if(paras.size()>10){
            double tmp=atof(paras.at(10));
            if(tmp>=0 & tmp<=180)
                matchfunc.spineAngThr=cos(tmp/180*M_PI);
            else
                cerr<<"error: wrong border tips spine angle filter: "<<tmp<<"; use default value 180, no spine will be filtered by angle"<<endl;
        }
        if(paras.size()>11){
            double tmp=atof(paras.at(11));
            if(tmp>=0)
                matchfunc.spineRadiusThr=tmp;
            else
                cerr<<"error: wrong border tips spine radius filter: "<<tmp<<"; use default value 0, no tips will be considered as spine"<<endl;
        }
        if(paras.size()>12){
            int tmp=atoi(paras.at(12));
            if(tmp==0 || tmp==1)
                matchfunc.typeConstrain=(tmp==1);
            else
                cerr<<"error: wrong type constrain mask: "<<tmp<<"; use default value 0, no constrain will be given"<<endl;
        }

        //do match
        //init clique and candidate
        matchfunc.init();
        //global match
        cout<<"start global search"<<endl;
        matchfunc.globalmatch();
        //stitch
        cout<<"stitch matched points"<<endl;
        matchfunc.stitch();

        //save results
        matchfunc.output_stitch(fname_output);
        matchfunc.output_candMatchScore(fname_output + "_matchscore.txt");
        matchfunc.output_affine(fname_output,fname_nt0);
        matchfunc.output_matchedMarkers_orgspace(fname_output+"_nt0_matched.marker",fname_output+"_nt1_matched.marker");
        matchfunc.output_parameter(fname_output+"_param.txt");

        cout<<("matching finished")<<endl;
    }
    else if (func_name == tr("tmp_test"))
    {
        doperformancetest(input, output, callback);
    }
    else if (func_name == tr("gmhash_test"))
    {
        dogmhashtest(input, output, callback);
    }
	else if (func_name == tr("help"))
	{
        printHelp();
    }
	else return false;

	return true;
}

void neuron_stitch::printHelp()
{
    cout<<"\nUsage: visiocyte -x neuron_stitch -f neuron_stitch_auto -i <input_first.swc> <input_second.swc> -o <output_base> "
       <<"-p <stack dir=2(x/y/z=0/1/2)> <stack rescale=1> <match angle thr=60> <match distance thr=100> <triangle match thr=100> <max number of triangles=10000> "<<
            "<border tips serch span=20> <border tips gap filter=0> <border tips segment filter=0> <border tips spine filter_size=0> "<<
            "<border tips spine filter_angle=180> <border tips spine filter_radius=0> <match branch by type=0>"<<endl;
    cout<<"\n";
}

void neuron_stitch::domatch(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    NeuronMatchDialog * myDialog = NULL;
    //myDialog = new NeuronMatchDialog(&callback, visiocytewin);
    myDialog = new NeuronMatchDialog();
    myDialog->exec();
}

void neuron_stitch::dostitch(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    //select the window to operate
    QList <VisiocyteR_MainWindow *> allWindowList = callback.getListAll3DViewers();
    QList <VisiocyteR_MainWindow *> selectWindowList;
    VisiocyteR_MainWindow * visiocytewin;
    QList<NeuronTree> * ntTreeList;
    int winid;
    qDebug("search for 3D windows");
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++)
    {
        ntTreeList = callback.getHandleNeuronTrees_Any3DViewer(allWindowList[i]);
        if(ntTreeList->size()==2)
            selectWindowList.append(allWindowList[i]);
    }
    qDebug("match and select 3D windows");
    if(selectWindowList.size()<1){
        visiocyte_msg("Cannot find 3D view with only 2 SWC file. Please load the two SWC file you want to stitch in the same 3D view");
        return;
    }else if(selectWindowList.size()>1){
        //pop up a window to select

        QStringList items;
        for(int i=0; i<selectWindowList.size(); i++){
            items.append(callback.getImageName(selectWindowList[i]));
        }
        bool ok;
        QString selectitem = QInputDialog::getItem(parent, QString::fromUtf8("Neuron Stitcher"), QString::fromUtf8("Select A Window to Operate"), items, 0, false, &ok);
        if(!ok) return;
        for(int i=0; i<selectWindowList.size(); i++){
            if(selectitem==callback.getImageName(selectWindowList[i]))
            {
                winid=i;
                break;
            }
        }
    }else{
        winid=0;
    }
    visiocytewin = selectWindowList[winid];

    NeuronLiveMatchDialog * myDialog = NULL;
    myDialog = new NeuronLiveMatchDialog(&callback, visiocytewin);
    myDialog->show();
}

void neuron_stitch::doadjust(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    //select the window to operate
    QList <VisiocyteR_MainWindow *> allWindowList = callback.getListAll3DViewers();
    QList <VisiocyteR_MainWindow *> selectWindowList;
    VisiocyteR_MainWindow * visiocytewin;
    QList<NeuronTree> * ntTreeList;
    int winid;
    qDebug("search for 3D windows");
    for (VISIOCYTELONG i=0;i<allWindowList.size();i++)
    {
        ntTreeList = callback.getHandleNeuronTrees_Any3DViewer(allWindowList[i]);
        if(ntTreeList->size()==2)
            selectWindowList.append(allWindowList[i]);
    }
    qDebug("match and select 3D windows");
    if(selectWindowList.size()<1){
        visiocyte_msg("Cannot find 3D view with only 2 SWC file. Please load the two SWC file you want to stitch in the same 3D view");
        return;
    }else if(selectWindowList.size()>1){
        //pop up a window to select

        QStringList items;
        for(int i=0; i<selectWindowList.size(); i++){
            items.append(callback.getImageName(selectWindowList[i]));
        }
        bool ok;
        QString selectitem = QInputDialog::getItem(parent, QString::fromUtf8("Neuron Stitcher"), QString::fromUtf8("Select A Window to Operate"), items, 0, false, &ok);
        if(!ok) return;
        for(int i=0; i<selectWindowList.size(); i++){
            if(selectitem==callback.getImageName(selectWindowList[i]))
            {
                winid=i;
                break;
            }
        }
    }else{
        winid=0;
    }
    visiocytewin = selectWindowList[winid];

    NeuronGeometryDialog * myDialog = NULL;
    myDialog = new NeuronGeometryDialog(&callback, visiocytewin);
    myDialog->show();
}

//temporary function for paper writting purpose
void neuron_stitch::doperformancetest(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback)
{
    vector<char*> infiles, inparas, outfiles;
    if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
    if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
    if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    cout<<"==== neuron stack auto stitcher ===="<<endl;
    if(infiles.size()!=2 || outfiles.size()!=1 || inparas.size()!=1)
    {
        qDebug("ERROR: please set input and output!");
        qDebug()<<"visiocyte -x neuron_stitch -f tmp_test -i nt_lower.swc nt_upper.swc -p nt_upper_truth.swc -o output";
        return;
    }

    //load neurons
    QString fname_nt0 = ((vector<char*> *)(input.at(0).p))->at(0);
    QString fname_nt1 = ((vector<char*> *)(input.at(0).p))->at(1);
    QString fname_nt1_truth = ((vector<char*> *)(input.at(1).p))->at(0);
    QString fname_output = ((vector<char*> *)(output.at(0).p))->at(0);
    NeuronTree* nt0 = new NeuronTree();
    if (fname_nt0.toUpper().endsWith(".SWC") || fname_nt0.toUpper().endsWith(".ESWC")){
        *nt0 = readSWC_file(fname_nt0);
    }
    if(nt0->listNeuron.size()<=0){
        qDebug()<<"failed to read SWC file: "<<fname_nt0;
        return;
    }
    NeuronTree* nt1 = new NeuronTree();
    if (fname_nt1.toUpper().endsWith(".SWC") || fname_nt1.toUpper().endsWith(".ESWC")){
        *nt1 = readSWC_file(fname_nt1);
    }
    if(nt1->listNeuron.size()<=0){
        qDebug()<<"failed to read SWC file: "<<fname_nt1;
        return;
    }
    NeuronTree* nt1_truth = new NeuronTree();
    if (fname_nt1_truth.toUpper().endsWith(".SWC") || fname_nt1_truth.toUpper().endsWith(".ESWC")){
        *nt1_truth = readSWC_file(fname_nt1_truth);
    }
    if(nt1_truth->listNeuron.size()<=0){
        qDebug()<<"failed to read SWC file: "<<fname_nt1_truth;
        return;
    }

    double _spanCand = 40;
    int _direction = 2;
    double _angThr_match = cos(70.0/180.0*M_PI); //50 degree
    double _cmatchThr = 100;
    double _pmatchThr = 100;
    double _zscale = 2;
    double _segmentThr = 0;
    double _gapThr = 0;
    int _spineLengthThr = 0;
    double _spineAngThr = -1;
    double _spineRadiusThr = 0;
    int _maxClique3Num = 6400;

    vector<double> walltimes;
    vector<double> cputimes;
    vector<int> num_TP, num_FP, num_FN, num_truth, num_bp0, num_bp1, num_tri0, num_tri1, num_trimatch;
    vector<int> tri_number;
    vector<double> pmatchThr,pmatchAng,tmatchThr;
    vector<double> distance, xydistance;

    //triangle number
    for(int i=0; i<10; i++){
        int tri=100*pow(2.0,i);
        //test triangle number and running time
        //init matching function
        double tmpnum[11];
        NeuronTree nt0_run;
        backupNeuron(*nt0, nt0_run);
        NeuronTree nt1_run;
        backupNeuron(*nt1, nt1_run);
        neuron_match_clique matchfunc(&nt0_run,&nt1_run);

        //parameters
        matchfunc.direction=_direction;
        matchfunc.zscale=_zscale;
        matchfunc.angThr_match=_angThr_match;
        matchfunc.pmatchThr = _pmatchThr;
        matchfunc.spanCand = _spanCand;
        matchfunc.cmatchThr = _cmatchThr;
        matchfunc.segmentThr = _segmentThr;
        matchfunc.gapThr = _gapThr;
        matchfunc.spineLengthThr = _spineLengthThr;
        matchfunc.spineAngThr = _spineAngThr;
        matchfunc.spineRadiusThr = _spineRadiusThr;

        matchfunc.maxClique3Num = tri;

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

        matchfunc.examineMatchingResult(tmpnum, nt1_truth);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<(int)tmpnum[0]<<":"<<(int)tmpnum[1]<<":"<<(int)tmpnum[2]<<":"<<(int)tmpnum[3]<<":"<<(int)tmpnum[4]<<":"<<(int)tmpnum[5]<<":"
               <<(int)tmpnum[6]<<":"<<(int)tmpnum[7]<<":"<<(int)tmpnum[8]<<":"<<tmpnum[9]<<":"<<tmpnum[10];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back((int)tmpnum[0]);
        num_FP.push_back((int)tmpnum[1]);
        num_FN.push_back((int)tmpnum[2]);
        num_truth.push_back((int)tmpnum[3]);
        num_bp0.push_back((int)tmpnum[4]);
        num_bp1.push_back((int)tmpnum[5]);
        num_tri0.push_back((int)tmpnum[6]);
        num_tri1.push_back((int)tmpnum[7]);
        num_trimatch.push_back((int)tmpnum[8]);
        distance.push_back(tmpnum[9]);
        xydistance.push_back(tmpnum[10]);
    }

    //point match distance
    for(double param=20; param<=300; param+=20){
        //test triangle number and running time
        //init matching function
        double tmpnum[11];
        NeuronTree nt0_run;
        backupNeuron(*nt0, nt0_run);
        NeuronTree nt1_run;
        backupNeuron(*nt1, nt1_run);
        neuron_match_clique matchfunc(&nt0_run,&nt1_run);

        //parameters
        matchfunc.direction=_direction;
        matchfunc.zscale=_zscale;
        matchfunc.angThr_match=_angThr_match;
        matchfunc.spanCand = _spanCand;
        matchfunc.cmatchThr = _cmatchThr;
        matchfunc.segmentThr = _segmentThr;
        matchfunc.maxClique3Num = _maxClique3Num;
        matchfunc.gapThr = _gapThr;
        matchfunc.spineLengthThr = _spineLengthThr;
        matchfunc.spineAngThr = _spineAngThr;
        matchfunc.spineRadiusThr = _spineRadiusThr;

        matchfunc.pmatchThr = param;

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

        matchfunc.examineMatchingResult(tmpnum, nt1_truth);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<(int)tmpnum[0]<<":"<<(int)tmpnum[1]<<":"<<(int)tmpnum[2]<<":"<<(int)tmpnum[3]<<":"<<(int)tmpnum[4]<<":"<<(int)tmpnum[5]<<":"
               <<(int)tmpnum[6]<<":"<<(int)tmpnum[7]<<":"<<(int)tmpnum[8]<<":"<<tmpnum[9]<<":"<<tmpnum[10];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back((int)tmpnum[0]);
        num_FP.push_back((int)tmpnum[1]);
        num_FN.push_back((int)tmpnum[2]);
        num_truth.push_back((int)tmpnum[3]);
        num_bp0.push_back((int)tmpnum[4]);
        num_bp1.push_back((int)tmpnum[5]);
        num_tri0.push_back((int)tmpnum[6]);
        num_tri1.push_back((int)tmpnum[7]);
        num_trimatch.push_back((int)tmpnum[8]);
        distance.push_back(tmpnum[9]);
        xydistance.push_back(tmpnum[10]);
    }

    //point match angular threshold
    for(double param=10; param<181; param+=10){
        //test triangle number and running time
        //init matching function
        double tmpnum[11];
        NeuronTree nt0_run;
        backupNeuron(*nt0, nt0_run);
        NeuronTree nt1_run;
        backupNeuron(*nt1, nt1_run);
        neuron_match_clique matchfunc(&nt0_run,&nt1_run);

        //parameters
        matchfunc.direction=_direction;
        matchfunc.zscale=_zscale;
        matchfunc.pmatchThr = _pmatchThr;
        matchfunc.spanCand = _spanCand;
        matchfunc.cmatchThr = _cmatchThr;
        matchfunc.segmentThr = _segmentThr;
        matchfunc.maxClique3Num = _maxClique3Num;
        matchfunc.gapThr = _gapThr;
        matchfunc.spineLengthThr = _spineLengthThr;
        matchfunc.spineAngThr = _spineAngThr;
        matchfunc.spineRadiusThr = _spineRadiusThr;

        matchfunc.angThr_match=cos(param/180*M_PI);

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

        matchfunc.examineMatchingResult(tmpnum, nt1_truth);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<(int)tmpnum[0]<<":"<<(int)tmpnum[1]<<":"<<(int)tmpnum[2]<<":"<<(int)tmpnum[3]<<":"<<(int)tmpnum[4]<<":"<<(int)tmpnum[5]<<":"
               <<(int)tmpnum[6]<<":"<<(int)tmpnum[7]<<":"<<(int)tmpnum[8]<<":"<<tmpnum[9]<<":"<<tmpnum[10];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back((int)tmpnum[0]);
        num_FP.push_back((int)tmpnum[1]);
        num_FN.push_back((int)tmpnum[2]);
        num_truth.push_back((int)tmpnum[3]);
        num_bp0.push_back((int)tmpnum[4]);
        num_bp1.push_back((int)tmpnum[5]);
        num_tri0.push_back((int)tmpnum[6]);
        num_tri1.push_back((int)tmpnum[7]);
        num_trimatch.push_back((int)tmpnum[8]);
        distance.push_back(tmpnum[9]);
        xydistance.push_back(tmpnum[10]);
    }

    //triangle match distance
    for(double param=20; param<=300; param+=20){
        //test triangle number and running time
        //init matching function
        double tmpnum[11];
        NeuronTree nt0_run;
        backupNeuron(*nt0, nt0_run);
        NeuronTree nt1_run;
        backupNeuron(*nt1, nt1_run);
        neuron_match_clique matchfunc(&nt0_run,&nt1_run);

        //parameters
        matchfunc.direction=_direction;
        matchfunc.zscale=_zscale;
        matchfunc.angThr_match=_angThr_match;
        matchfunc.pmatchThr = _pmatchThr;
        matchfunc.spanCand = _spanCand;
        matchfunc.segmentThr = _segmentThr;
        matchfunc.maxClique3Num = _maxClique3Num;
        matchfunc.gapThr = _gapThr;
        matchfunc.spineLengthThr = _spineLengthThr;
        matchfunc.spineAngThr = _spineAngThr;
        matchfunc.spineRadiusThr = _spineRadiusThr;

        matchfunc.cmatchThr = param;

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

        matchfunc.examineMatchingResult(tmpnum, nt1_truth);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<(int)tmpnum[0]<<":"<<(int)tmpnum[1]<<":"<<(int)tmpnum[2]<<":"<<(int)tmpnum[3]<<":"<<(int)tmpnum[4]<<":"<<(int)tmpnum[5]<<":"
               <<(int)tmpnum[6]<<":"<<(int)tmpnum[7]<<":"<<(int)tmpnum[8]<<":"<<tmpnum[9]<<":"<<tmpnum[10];
        walltimes.push_back(wall1-wall0);
        cputimes.push_back(cpu1-cpu0);
        num_TP.push_back((int)tmpnum[0]);
        num_FP.push_back((int)tmpnum[1]);
        num_FN.push_back((int)tmpnum[2]);
        num_truth.push_back((int)tmpnum[3]);
        num_bp0.push_back((int)tmpnum[4]);
        num_bp1.push_back((int)tmpnum[5]);
        num_tri0.push_back((int)tmpnum[6]);
        num_tri1.push_back((int)tmpnum[7]);
        num_trimatch.push_back((int)tmpnum[8]);
        distance.push_back(tmpnum[9]);
        xydistance.push_back(tmpnum[10]);
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
        fp<<num_trimatch[i]<<"\t";
        fp<<distance[i]<<"\t";
        fp<<xydistance[i]<<endl;
    }
    fp.close();

    neuron_match_clique matchfunc(nt0,nt1);
    matchfunc.direction=_direction;
    matchfunc.zscale=_zscale;
    matchfunc.angThr_match=_angThr_match;
    matchfunc.pmatchThr = _pmatchThr;
    matchfunc.spanCand = _spanCand;
    matchfunc.cmatchThr = _cmatchThr;
    matchfunc.segmentThr = _segmentThr;
    matchfunc.maxClique3Num = _maxClique3Num;
    matchfunc.gapThr = _gapThr;
    matchfunc.spineLengthThr = _spineLengthThr;
    matchfunc.spineAngThr = _spineAngThr;
    matchfunc.spineRadiusThr = _spineRadiusThr;
    matchfunc.output_parameter(fname_output+"_param.txt");

    qDebug()<<"testing finished";
}


//temporary function for paper writting purpose
void neuron_stitch::dogmhashtest(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback)
{
    vector<char*> infiles, inparas, outfiles;
    if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
    if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
    if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    cout<<"==== neuron stack auto stitcher ===="<<endl;
    if(infiles.size()!=2 || outfiles.size()!=1 || inparas.size()!=1)
    {
        qDebug("ERROR: please set input and output!");
        qDebug()<<"visiocyte -x neuron_stitch -f gmhash_test -i nt_lower.swc nt_upper.swc -p nt_upper_truth.swc -o output";
        return;
    }

    //load neurons
    QString fname_nt0 = ((vector<char*> *)(input.at(0).p))->at(0);
    QString fname_nt1 = ((vector<char*> *)(input.at(0).p))->at(1);
    QString fname_nt1_truth = ((vector<char*> *)(input.at(1).p))->at(0);
    QString fname_output = ((vector<char*> *)(output.at(0).p))->at(0);
    NeuronTree* nt0 = new NeuronTree();
    if (fname_nt0.toUpper().endsWith(".SWC") || fname_nt0.toUpper().endsWith(".ESWC")){
        *nt0 = readSWC_file(fname_nt0);
    }
    if(nt0->listNeuron.size()<=0){
        qDebug()<<"failed to read SWC file: "<<fname_nt0;
        return;
    }
    NeuronTree* nt1 = new NeuronTree();
    if (fname_nt1.toUpper().endsWith(".SWC") || fname_nt1.toUpper().endsWith(".ESWC")){
        *nt1 = readSWC_file(fname_nt1);
    }
    if(nt1->listNeuron.size()<=0){
        qDebug()<<"failed to read SWC file: "<<fname_nt1;
        return;
    }
    NeuronTree* nt1_truth = new NeuronTree();
    if (fname_nt1_truth.toUpper().endsWith(".SWC") || fname_nt1_truth.toUpper().endsWith(".ESWC")){
        *nt1_truth = readSWC_file(fname_nt1_truth);
    }
    if(nt1_truth->listNeuron.size()<=0){
        qDebug()<<"failed to read SWC file: "<<fname_nt1_truth;
        return;
    }

    double _spanCand = 20;
    int _direction = 2;
    double _pmatchThr = 100;
    double _zscale = 2;

    QString fname_out=fname_output+"_testlog.txt";
    ofstream fp;
    fp.open(fname_out.toStdString().c_str());

    for(int pmatchthr=20; pmatchthr<=300; pmatchthr+=20){
        NeuronTree nt0_run;
        backupNeuron(*nt0, nt0_run);
        NeuronTree nt1_run;
        backupNeuron(*nt1, nt1_run);
        neuron_match_gmhash matchfunc(&nt0_run,&nt1_run);

        matchfunc.hashbin=pmatchthr;
        matchfunc.pmatchThr=pmatchthr;
        matchfunc.zscale=_zscale;
        matchfunc.spanCand=_spanCand;
        matchfunc.direction=_direction;

        double wall0 = get_wall_time();
        double cpu0  = get_cpu_time();
        //init clique and candidate
        matchfunc.init();

        //global match
        matchfunc.globalmatch();
        //  Stop timers
        double wall1 = get_wall_time();
        double cpu1  = get_cpu_time();

        double tmpnum[11];
        matchfunc.examineMatchingResult(tmpnum, nt1_truth);

        qDebug()<<"cojoc: "<<wall1-wall0<<":"<<cpu1-cpu0<<":"<<(int)tmpnum[0]<<":"<<(int)tmpnum[1]<<":"<<(int)tmpnum[2]<<":"<<(int)tmpnum[3]<<":"<<(int)tmpnum[4]<<":"<<(int)tmpnum[5]<<":"
               <<(int)tmpnum[6]<<":"<<(int)tmpnum[7]<<":"<<(int)tmpnum[8]<<":"<<tmpnum[9]<<":"<<tmpnum[10];
        //output
        matchfunc.output(fname_output);
        fp<<pmatchthr<<"\t";
        fp<<pmatchthr<<"\t";
        fp<<pmatchthr<<"\t";
        fp<<pmatchthr<<"\t";
        fp<<wall1-wall0<<"\t";
        fp<<cpu1-cpu0<<"\t";
        fp<<(int)tmpnum[0]<<"\t";
        fp<<(int)tmpnum[1]<<"\t";
        fp<<(int)tmpnum[2]<<"\t";
        fp<<(int)tmpnum[3]<<"\t";
        fp<<(int)tmpnum[4]<<"\t";
        fp<<(int)tmpnum[5]<<"\t";
        fp<<(int)tmpnum[6]<<"\t";
        fp<<(int)tmpnum[7]<<"\t";
        fp<<(int)tmpnum[8]<<"\t";
        fp<<tmpnum[9]<<"\t";
        fp<<tmpnum[10]<<endl;
    }

    qDebug()<<"testing finished";
}
