/* consensus_skeleton_func.cpp
 * a plugin to merge multiple neurons by generating a consensus skeleton
 * 2012-05-02 : by Yinan Wan
 */

#include <visiocyte_interface.h>
#include "basic_surf_objs.h"
#include "visiocyte_message.h"
#include "consensus_skeleton_func.h"
#include "consensus_skeleton.h"
#include <vector>
#include <iostream>
using namespace std;

const QString title = QObject::tr("Consensus Skeleton");

int consensus_skeleton_io(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	QString fileOpenName;
	fileOpenName = QFileDialog::getOpenFileName(0, QObject::tr("Open File"),
			"",
			QObject::tr("Supported file (*.ano)"
				";;Neuron structure	(*.ano)"
				));
	if(fileOpenName.isEmpty()) 
		return -1;
		
	P_ObjectFileType linker_object;
	if (!loadAnoFile(fileOpenName,linker_object))
	{
		fprintf(stderr,"Error in reading the linker file.\n");
		return -1;
	}
	
	QStringList nameList = linker_object.swc_file_list;
	VISIOCYTELONG neuronNum = nameList.size();
	VISIOCYTELONG avg_node_num = 0;
	VISIOCYTELONG max_node_num = -1;
	vector<NeuronTree> nt_list;
	
	for (VISIOCYTELONG i=0;i<neuronNum;i++)
	{
		NeuronTree tmp = readSWC_file(nameList.at(i));
		nt_list.push_back(tmp);
		avg_node_num += tmp.listNeuron.size();
		if (tmp.listNeuron.size()>max_node_num) max_node_num = tmp.listNeuron.size();
	}
	avg_node_num /= neuronNum;

	bool ok;
	VISIOCYTELONG n_sampling = QInputDialog::getInt(parent, "sample number", "Please specify the node number of your merged skeleton:", avg_node_num, 1, max_node_num, 1, &ok);
	
	if (!ok)
		return 0;
	

	QList<NeuronSWC> result_lN;
	if (!consensus_skeleton(nt_list, result_lN, n_sampling, 0))
	{
		visiocyte_msg("Error in consensus skeleton!");
		return -1;
	}

	QString fileSaveName;
	QString defaultSaveName = fileOpenName + "_consensus.swc";
	fileSaveName = QFileDialog::getSaveFileName(0, QObject::tr("Save merged neuron to file:"),
			defaultSaveName,
			QObject::tr("Supported file (*.swc)"
				";;Neuron structure	(*.swc)"
				));
	if (!export_listNeuron_2swc(result_lN,qPrintable(fileSaveName)))
	{
		visiocyte_msg("Unable to save file");
		return -1;
	}

	return 1;
}

bool consensus_skeleton_io(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output)
{
	cout<<"Welcome to consensus_skeleton"<<endl;
	if(input.size()==0 || output.size() != 1) return true;
	char * paras = NULL;

	//parsing input
	vector<char *> * inlist =  (vector<char*> *)(input.at(0).p);
	if (inlist->size()==0)
	{
		cerr<<"You must specify input linker or swc files"<<endl;
		return false;
	}

	//parsing output
	vector<char *> * outlist = (vector<char*> *)(output.at(0).p);
	if (outlist->size()>1)
	{
		cerr<<"You cannot specify more than 1 output files"<<endl;
		return false;
	}

	//parsing parameters
	VISIOCYTELONG n_sampling = 0;
	int method_code = 0;
	if (input.size()==2)
	{
		vector<char*> * paras = (vector<char*> *)(input.at(1).p);
		if (paras->size()==1)
		{
			n_sampling = atoi(paras->at(0));
			cout<<"n_sampling = "<<n_sampling<<endl;
		}
		else
		{
			cerr<<"Too many parameters"<<endl;
			return false;
		}
	}

	vector<NeuronTree> nt_list;
	QStringList nameList;
	QString qs_linker;
	char * dfile_result = NULL;
	VISIOCYTELONG neuronNum = 0;

	for (int i=0;i<inlist->size();i++)
	{
		qs_linker = QString(inlist->at(i));
		if (qs_linker.toUpper().endsWith(".ANO"))
		{
			cout<<"(0). reading a linker file."<<endl;
			P_ObjectFileType linker_object;
			if (!loadAnoFile(qs_linker,linker_object))
			{
				fprintf(stderr,"Error in reading the linker file.\n");
				return 1;
			}
			nameList = linker_object.swc_file_list;
			neuronNum += nameList.size();
			for (VISIOCYTELONG i=0;i<neuronNum;i++)
			{
				NeuronTree tmp = readSWC_file(nameList.at(i));
				nt_list.push_back(tmp);
			}
		}
		else if (qs_linker.toUpper().endsWith(".SWC"))
		{
			cout<<"(0). reading an swc file"<<endl;
			NeuronTree tmp = readSWC_file(qs_linker);
			nt_list.push_back(tmp);
			neuronNum++;
			if (outlist->size()==0)
			{
				cerr<<"You must specify outfile name if you input a list of swcs"<<endl;
				return false;
			}
		}
	}

	QString outfileName;
	if (outlist->size()==0)
		outfileName = qs_linker + "_consensus.swc";
	else
		outfileName = QString(outlist->at(0));

	QList<NeuronSWC> merge_result;
	if (!consensus_skeleton(nt_list, merge_result, n_sampling, method_code))
	{
		cerr<<"error in consensus_skeleton"<<endl;
		return false;
	}

	export_listNeuron_2swc(merge_result,qPrintable(outfileName));
	printf("\t %s has been generated successfully, size:%d.\n",qPrintable(outfileName), merge_result.size());

	return true;
}

void printHelp()
{
	cout<<"\nConsensus Skeleton: a plugin to merge multiple neurons by generating a consensus skeleton. by Yinan Wan 12-05-02"<<endl;
	cout<<"\nUsage: visiocyte -x consensus_skeleton -f <function_name> -i <input_file(s)> -o <output_file> -p <node_number>"<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"\t-f <function_name>:  consensus_skeleton"<<endl;
	cout<<"\t-i <input_file(s)>:  input linker file (.ano) or file name list(\"a.swc b.swc\")"<<endl;
	cout<<"\t[-o] <output_file>:  output file name. If -i is followd by a linker file name, this parameter can be omitted"<<endl;
	cout<<"\t                     default result will be generated under the same directory of the ref linkerfile and has a name of 'linkerFileName_consensus.swc'"<<endl;
	cout<<"\t[-p] <node_number>:  set sampling node number, final output tree is of this size"<<endl;
	cout<<"Example: visiocyte -x consensus_skeleton -h consensus_skeleton -i mylinker.ano -p 200 -o consensus_skeleton.swc\n"<<endl;
}

