/*
 *  sort_func.cpp
 *  io functions for domenu and do func 
 *
 *  Created by Wan, Yinan, on 02/01/2012.
 */

#include <visiocyte_interface.h>
#include "visiocyte_message.h"
//#include "sort_func.h"
#include "openSWCDialog.h"
#include "customary_structs/visiocyte_neurontoolbox_para.h"
#include <vector>
#include <iostream>
#include "sort_swc.h"
using namespace std;

const QString title = QObject::tr("Sort SWC Plugin");

void sort_menu(VISIOCYTEPluginCallback2 &callback)
{
    //cout<<"callback_sort: "<<(void*)&callback<<endl;
    //callback.getImageWindowList();
    //visiocyte_msg("image window list sort", 0);

	OpenSWCDialog * openDlg = new OpenSWCDialog(0, &callback);
	if (!openDlg->exec())
		return;

	NeuronTree nt = openDlg->nt;
	QList<NeuronSWC> neuron = nt.listNeuron;

	VISIOCYTELONG rootid;
	VISIOCYTELONG thres;
	bool ok;
	rootid = QInputDialog::getInteger(0, "Would you like to specify new root number?","New root number:(If you select 'cancel', the first root in file is set as default)",1,1,neuron.size(),1,&ok);
	if (!ok)
		rootid = VOID;
	thres = QInputDialog::getDouble(0, "Would you like to set a threshold for the newly generated link?","threshold:(If you select 'cancel', all the points will be connected automated; If you set '0', no new link will be generated)",0,0,2147483647,1,&ok);
	if (!ok)
		thres = VOID;

	QList<NeuronSWC> result;
	QString fileOpenName = openDlg->file_name;
	if (SortSWC(neuron, result ,rootid, thres))
	{
		QString fileDefaultName = fileOpenName+QString("_sorted.swc");
		//write new SWC to file
		QString fileSaveName = QFileDialog::getSaveFileName(0, QObject::tr("Save File"),
				fileDefaultName,
				QObject::tr("Supported file (*.swc)"
					";;Neuron structure	(*.swc)"
					));
		if (fileSaveName.isEmpty())
			return;
		if (!export_list2file(result,fileSaveName,fileOpenName))
		{
			visiocyte_msg("fail to write the output swc file.");
			return;
		}
	}
	return;
}

void sort_toolbox(const VISIOCYTEPluginArgList & input)
{
	visiocyte_neurontoolbox_paras * paras = (visiocyte_neurontoolbox_paras *)(input.at(0).p);
	NeuronTree nt(paras->nt);
	QList<NeuronSWC> neuron = nt.listNeuron;

	VISIOCYTELONG rootid;
	VISIOCYTELONG thres;
	bool ok;
	rootid = QInputDialog::getInteger(0, "Would you like to specify new root number?","New root number:(If you select 'cancel', the first root in file is set as default)",1,1,neuron.size(),1,&ok);
	if (!ok)
		rootid = VOID;
	thres = QInputDialog::getDouble(0, "Would you like to set a threshold for the newly generated link?","threshold:(If you select 'cancel', all the points will be connected automated; If you set '0', no new link will be generated)",0,0,2147483647,1,&ok);
	if (!ok)
		thres = VOID;

	QList<NeuronSWC> result;
	QString fileOpenName = nt.file;
	if (SortSWC(neuron, result ,rootid, thres))
	{
		QString fileDefaultName = fileOpenName+QString("_sorted.swc");
		//write new SWC to file
		QString fileSaveName = QFileDialog::getSaveFileName(0, QObject::tr("Save File"),
				fileDefaultName,
				QObject::tr("Supported file (*.swc)"
					";;Neuron structure	(*.swc)"
					));
		if (fileSaveName.isEmpty())
			return;
		if (!export_list2file(result,fileSaveName,fileOpenName))
		{
			visiocyte_msg("fail to write the output swc file.");
			return;
		}
	}
	return;
}

bool sort_func(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	cout<<"==========Welcome to sort_swc function============="<<endl;
	vector<char*>* inlist = (vector<char*>*)(input.at(0).p);
	vector<char*>* outlist = NULL;
	vector<char*>* paralist = NULL;

	double thres = VOID;
	QList<NeuronSWC> neuron, result;
	bool hasPara, hasOutput;
	if (input.size()==1) 
	{
		cout<<"No new parameter specified.\n";
		hasPara = false;
	}
	else {
		hasPara = true;
		paralist = (vector<char*>*)(input.at(1).p);
	}

	if (inlist->size()!=1)
	{
		cout<<"You must specify 1 input file!"<<endl;
		return false;
	}

	if (output.size()==0){
		cout<<"No output file specified.\n";
		hasOutput = false;
	}
	else {
		hasOutput = true;
		if (output.size()>1)
		{
			cout<<"You have specified more than 1 output file.\n";
			return false;
		}
		outlist = (vector<char*>*)(output.at(0).p);
	}


	if (hasPara)
	{
		if (paralist->size()==0)
		{
			cout<<"Threshold not set. All points will connected automatically."<<endl;
			thres = VOID;
		}
		else if (paralist->size()==1)
		{
			thres = atof(paralist->at(0));
			cout<<"threshold: "<<thres<<endl;
		}
		else
		{
			cout<<"Illegal parameter list."<<endl;
			return false;
		}
	}


	QString fileOpenName = QString(inlist->at(0));
	QString fileSaveName;
	if (hasOutput) 
	{
		cout<<"outfile: "<<outlist->at(0)<<endl;
		fileSaveName = QString(outlist->at(0));
	}
	else 
	{
		fileSaveName = fileOpenName+QString("_sorted.swc");
	}

	if (fileOpenName.endsWith(".swc") || fileOpenName.endsWith(".SWC"))
		neuron = readSWC_file(fileOpenName).listNeuron;
	else if (fileOpenName.endsWith(".ano") || fileOpenName.endsWith(".ANO"))
	{
		P_ObjectFileType linker_object;
		if (!loadAnoFile(fileOpenName,linker_object))
		{
			cout<<"Error in reading the linker file."<<endl;
			return false;
		}
		QStringList nameList = linker_object.swc_file_list;
		VISIOCYTELONG neuronNum = nameList.size();
		vector<QList<NeuronSWC> > nt_list;
		for (VISIOCYTELONG i=0;i<neuronNum;i++)
		{
			QList<NeuronSWC> tmp = readSWC_file(nameList.at(i)).listNeuron;
			nt_list.push_back(tmp);
		}
		if (!combine_linker(nt_list, neuron))
		{
			cout<<"Error in combining neurons."<<endl;
			return false;
		}
	}
	else
	{
		cout<<"The file type you specified is not supported."<<endl;
		return false;
	}

	if (!SortSWC(neuron, result , VOID, thres))
	{
		cout<<"Error in sorting swc"<<endl;
		return false;
	}
	if (!export_list2file(result, fileSaveName, fileOpenName))
	{
		cout<<"Error in writing swc to file"<<endl;
		return false;
	}
	return true;
}

void printHelp(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	visiocyte_msg("(version 2.0) Reallocate id for the points in 1 neuron (.swc) or mutiple neurons (.ano). Set a new root and sort the SWC file into a new order, where the newly set root has the id of 1, and the parent's id is less than its child's. If the link threshold is set, the plugin will only generate nearest link shorter than that threshold. by Yinan Wan 2012-02-01");
}

void printHelp(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	cout<<"(version 2.0) Reallocate id for the points in 1 neuron (.swc) or mutiple neurons (.ano). Set a new root and sort the SWC file into a new order, where the newly set root has the id of 1, and the parent's id is less than its child's. If the link threshold is set, the plugin will only generate nearest link shorter than that threshold. by Yinan Wan 2012-02-01"<<endl;
	cout<<"usage:\n";
	cout<<"-f<func name>:\t\t sort_swc\n";
	cout<<"-i<file name>:\t\t input .swc or .ano file\n";
	cout<<"-o<file name>:\t\t (not required) output sorted swc file. DEFAUTL: 'inputName_sorted.swc'\n";
	cout<<"-p<link threshold>:\t (not required) the threshold of link you want the plugin to generate in case the input swc(s) are broken. if you set thres=0, no new link will be generated. DEFAULT: Infinity (all points will be connected automatically).\n";
	cout<<"Demo:\t ./visiocyte -x plugins/neuron_uitilities/sort_a_swc/libsort_neuron_swc.so -f sort_swc -i test.swc -o test_sorted.swc -p 1\n";

}


