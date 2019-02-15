#include "hierachical_labeling.h"
#include "eswc_core.h"
#include <iostream>
using namespace std;

#define DIST(a,b) sqrt(((a).x-(b).x)*((a).x-(b).x)+((a).y-(b).y)*((a).y-(b).y)+((a).z-(b).z)*((a).z-(b).z))
//hierachically label the input neuron tree to generate an enhanced swc (eswc)
//12-05-04 Yinan Wan
bool hierachical_labeling(NeuronTree & nt)
{

	VISIOCYTELONG siz = nt.listNeuron.size();
	
	vector<VISIOCYTELONG> seg_id, seg_layer;
	if (!swc2eswc(nt, seg_id, seg_layer))
	{
		cerr<<"error in converting swc to eswc"<<endl;
		return false;
	}
	for (VISIOCYTELONG i=0;i<siz;i++)
	{
		nt.listNeuron[i].seg_id = seg_id[i];
		nt.listNeuron[i].level = seg_layer[i];
	}
	cout<<"converted to eswc done"<<endl;

	vector<VISIOCYTELONG> branches(siz, 0);
	
	for (VISIOCYTELONG i=0;i<siz;i++)
	{
		if (nt.listNeuron[i].pn<0) continue;
		VISIOCYTELONG pid = nt.hashNeuron.value(nt.listNeuron[i].pn);
		branches[pid]++;
	}

	vector<VISIOCYTELONG> tip(siz, -1);//the segment including i starts from tip[i]
	vector<double> seg_length(siz, 0);//the segment length
	//all these VISIOCYTELONGs are index in the listNeuron
	for (VISIOCYTELONG i=0;i<siz;i++)
	{
		if (branches[i]!=0) continue;
		VISIOCYTELONG cur = i;
		tip[i] = i;
		VISIOCYTELONG pid;
		do
		{
			NeuronSWC s = nt.listNeuron[cur];
			pid = nt.hashNeuron.value(s.pn);
			if (DIST(s, nt.listNeuron[pid]) + seg_length[cur] > seg_length[pid])
			{
				seg_length[pid] = DIST(s, nt.listNeuron[pid]) + seg_length[cur];
				tip[pid] = i;
			}
			cur = pid;
		}
		while (branches[cur]!=0 && pid>0);
	}

	float colored = 1;
	for (VISIOCYTELONG i=0;i<siz;i++)
	{
		if (branches[i]!=0) continue;
		VISIOCYTELONG cur = i;
		VISIOCYTELONG pid;
		do
		{
			NeuronSWC s = nt.listNeuron[cur];
			if (s.pn<0)
         {
          nt.listNeuron[cur].fea_val.append(-1);
          break;
         } 
			nt.listNeuron[cur].fea_val.append(colored);
			pid = nt.hashNeuron.value(s.pn);
			cur = pid;
		}
		while (tip[pid]==i);
		colored = colored + 1;
	}

	return true;
}
